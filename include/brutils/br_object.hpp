/**
 * @file br_object.h
 *
 * Description
 */

#ifndef BRUTILS_BR_OBJECT_HPP
#define BRUTILS_BR_OBJECT_HPP

#include <functional>
#include <condition_variable>
#include <vector>
#include <map>
#include <atomic>
#include <thread>
#include <any>
#include <queue>

namespace brutils {
enum class ConnectionType {
  Auto,
  Direct,
  Queued
};

template<typename ...Args>
class signal;
template<typename ...Args>
class slot;

class br_threaded_object {
  template<typename ...S>
  friend
  class slot;
  template<typename ...S>
  friend
  class signal;
 public:
  br_threaded_object() : _running(true) {
    _thread = std::thread(&br_threaded_object::run, this);
  };
  br_threaded_object(const br_threaded_object &) = delete;
  br_threaded_object(br_threaded_object &&) = delete;
  virtual ~br_threaded_object() {
    if (_running) {
      _running = false;
      _cond.notify_all();
    }
    if (_thread.joinable()) {
      _thread.join();
    }
  };

 public:
  template<typename ...Args>
  static void connect(signal<Args...> &sig, slot<Args...> &slot, ConnectionType type = ConnectionType::Auto) {
    sig.connect(slot, type);
  }

 private:
  std::atomic_bool _running;
  std::thread _thread;

 private:
  std::mutex _condMutex;
  std::condition_variable _cond;

 private:
  std::queue<std::function<void()>> _eventList;

 private:
  void addEventToSlot(const std::function<void()> &event) {
    _eventList.push(event);
    _cond.notify_one();
  }

  std::thread::id getThreadId() const {
    return _thread.get_id();
  }

 private:
  void run() {
    while (_running) {
      std::unique_lock lock(_condMutex);
      _cond.wait(lock, [&] { return !_eventList.empty() || !_running; });
      if (!_eventList.empty()) {
        auto &event = _eventList.front();
        event();
        _eventList.pop();
      }
    }
  }
};

template<typename ...Args>
class slot {
  template<typename ...S>
  friend
  class signal;
 public:
  slot(const slot &) = delete;
  slot(slot &&) = delete;
  slot &operator=(const slot &) = delete;
  ~slot() = default;

  explicit slot(br_threaded_object *parent = nullptr) : _init(false), _parent(parent) {}
  explicit slot(std::function<void(Args...)> function, br_threaded_object *parent = nullptr)
      : _init(true), _parent(parent) {
    _function = function;
  }

 public:
  void setSlotFunction(std::function<void(Args...)> function) {
    _function = function;
    _init = true;
  }
  void operator()(Args... parameters) {
    call(parameters...);
  }
  void call(Args... parameters) {
    if (!_init) {
      throw std::runtime_error("Slot not initialized with a function, yet!");
    }
    _function(parameters...);
  }

 private:
  bool _init;
  std::function<void(Args...)> _function;
  br_threaded_object *_parent;

 private:
  void addEventToParent(Args... args) const {
    if (nullptr == _parent)
      throw std::runtime_error("Slot has no parent!");
    _parent->addEventToSlot([=] {
      _function(args...);
    });
  }
  std::thread::id getThreadId() const {
    return _parent->getThreadId();
  }
};

template<typename ...Args>
class signal {
 public:
  signal(const signal &) = delete;
  signal(signal &&) = delete;
  signal &operator=(const signal &) = delete;
  ~signal() = default;

  signal() = default;

 public:
  int connect(slot<Args...> &slot, ConnectionType type = ConnectionType::Auto) {
    if (type == ConnectionType::Auto) {
      if (std::this_thread::get_id() == slot.getThreadId())
        type = ConnectionType::Direct;
      else
        type = ConnectionType::Queued;
    }
    _connectionList.insert(std::make_pair(++_lastId, Connection{&slot, type}));
    return _lastId;
  }

  void disconnect(int id = -1) {
    if (-1 == id)
      _connectionList.clear();
    else _connectionList.erase(id);
  }

  void emit(Args... parameters) {
    for (auto &slot: _connectionList) {
      ConnectionType type = slot.second._type;
      if (type == ConnectionType::Auto) {
        if (std::this_thread::get_id() == slot.second._slot->getThreadId())
          type = ConnectionType::Direct;
        else type = ConnectionType::Queued;
      }

      if (ConnectionType::Direct == type)
        (*slot.second._slot)(parameters...);
      else if (ConnectionType::Queued == type)
        (*slot.second._slot).addEventToParent(parameters...);
    }
  }

 private:
  struct Connection {
    slot<Args...> *_slot;
    ConnectionType _type;
  };

 private:
  static int _lastId;
  mutable std::map<int, Connection> _connectionList;
};
template<typename ...S>
int signal<S...>::_lastId = 0;
}

#endif //BRUTILS_BR_OBJECT_HPP
