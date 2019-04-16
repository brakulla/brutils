/**
 * @file br_object.h
 *
 * Description
 */

#ifndef BRUTILS_BR_OBJECT_HPP
#define BRUTILS_BR_OBJECT_HPP

#include <functional>
#include <condition_variable>
#include <map>
#include <atomic>
#include <thread>
#include <queue>

namespace brutils
{
enum class ConnectionType
{
    Auto,
    Direct,
    Queued
};

template<typename ...Args>
class signal;
template<typename ...Args>
class slot;

/**
 * @class br_threaded_object
 * @brief A base class for objects that are to be run in a seperate thread.
 */
class br_threaded_object
{
public:
    br_threaded_object(const br_threaded_object &) = delete;
    br_threaded_object(br_threaded_object &&) = delete;
    br_threaded_object &operator=(const br_threaded_object &) = delete;

    br_threaded_object()
        : _running(true)
    {
        _thread = std::thread(&br_threaded_object::run, this);
    };

    virtual ~br_threaded_object()
    {
        if (_running) {
            _running = false;
            _cond.notify_all();
        }
        if (_thread.joinable()) {
            _thread.join();
        }
    };

public:
    std::thread::id getThreadId() const
    {
        return _thread.get_id();
    }

    /**
     * @brief Add an event to event loop of br_threaded_object instance.
     * @param [in] event const std::function<void()> - This event function will be run in the thread of br_threaded_object instance.
     */
    void addEvent(const std::function<void()> &event)
    {
        _eventList.push(event);
        _cond.notify_one();
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
    void run()
    {
        while (_running) {
            std::unique_lock lock(_condMutex);
            _cond.wait(lock, [&]
            { return !_eventList.empty() || !_running; });
            if (!_eventList.empty()) {
                auto &event = _eventList.front();
                event();
                _eventList.pop();
            }
        }
    }
};

/**
 * @class slot. Wrapper class for slot functions.
 *          If no parent object is given (nullptr), then direct connections forced.
 * @tparam Args
 */
template<typename ...Args>
class slot
{
    template<typename ...S>
    friend
    class signal;
public:
    slot(const slot &) = delete;
    slot(slot &&) = delete;
    slot &operator=(const slot &) = delete;

    /**
     * @brief If this overloaded constructor is used,
     *          then `setSlofFunction` function should be called after initialization, before making any connection.
     * @param [in] parent
     */
    explicit slot(br_threaded_object *parent)
        : _init(false), _parent(parent)
    {}
    ~slot() = default;

    /**
     * @brief If this overloaded constructor is used, then this instance is ready to make connections.
     * @param [in] function
     * @param [in] parent
     */
    explicit slot(std::function<void(Args...)> function, br_threaded_object *parent = nullptr)
        : _init(true), _parent(parent)
    {
        _function = function;
    }

public:
    /**
     * @brief Used to set the function to be called when a connected signal is emitted.
     * @param [in] function
     */
    void setSlotFunction(std::function<void(Args...)> function)
    {
        _function = function;
        _init = true;
    }

    /**
     * @brief Calls the wrapped function. Added for convenient usage. Internally, calls `call` function.
     * @param parameters
     */
    void operator()(Args... parameters)
    {
        call(parameters...);
    }

    /**
     * @brief Calls wrapped function. Internally, used for direct connections.
     * @param parameters
     */
    void call(Args... parameters)
    {
        if (!_init) {
            throw std::runtime_error("slot is not initialized with a function");
        }
        _function(parameters...);
    }

private:
    bool _init;
    std::function<void(Args...)> _function;
    br_threaded_object *_parent;

private:
    /**
     * @brief Adds the wrapped function to parent object's event loop. Internally, used for queued connections.
     * @param [in] args
     */
    void addEventToParent(Args... args) const
    {
        if (nullptr == _parent)
            throw std::runtime_error("slot has not parent");
        _parent->addEvent([=]
                          {
                              _function(args...);
                          });
    }
    std::thread::id getThreadId() const
    {
        return _parent->getThreadId();
    }
};

/**
 * @class signal. Wrapper for sending data to slots.
 *          If no parent object is given (nullptr), then direct connections forced.
 * @tparam Args
 */
template<typename ...Args>
class signal
{
public:
    signal(const signal &) = delete;
    signal(signal &&) = delete;
    signal &operator=(const signal &) = delete;

    explicit signal(br_threaded_object *parent)
        : _parent(parent)
    {};
    ~signal() = default;

public:
    /**
     * @brief Creates a connection between the instance and input slot instance in given `ConnectionType`.
     * @param [in] slot
     * @param [in] type
     * @return id Returns created connection identifier.
     */
    int connect(slot<Args...> &slot, ConnectionType type = ConnectionType::Auto)
    {
        if (nullptr == _parent)
            type = ConnectionType::Direct;
        if (type == ConnectionType::Auto) {
            if (_parent->getThreadId() == slot.getThreadId())
                type = ConnectionType::Direct;
            else
                type = ConnectionType::Queued;
        }
        _connectionList.insert(std::make_pair(++_lastId, Connection{&slot, type}));
        return _lastId;
    }

    /**
     * @brief Destroys the connection previously created by `connect` function.
     * @param id Connection identifier. If not given, all connections are destroyed.
     */
    void disconnect(int id = -1)
    {
        if (-1 == id)
            _connectionList.clear();
        else _connectionList.erase(id);
    }

    /**
     * @brief Emits the signal. Passes the parameters to slot function in a thread-safe way.
     * @param [in] parameters
     */
    void emit(Args... parameters)
    {
        for (auto &slot: _connectionList) {
            ConnectionType type = slot.second._type;
            if (ConnectionType::Direct == type) {
                if (nullptr != _parent) {
                    _parent->addEvent([=]
                                      {
                                          (*slot.second._slot)(parameters...);
                                      });
                }
            }
            else if (ConnectionType::Queued == type) {
                if (nullptr == _parent)
                    throw std::runtime_error("signal has no parent");
                _parent->addEvent([=]
                                  {
                                      std::cout << "Emitting signal from: " << std::this_thread::get_id() << std::endl;
                                      (*slot.second._slot).addEventToParent(parameters...);
                                  });
            }
        }
    }

private:
    struct Connection
    {
        slot<Args...> *_slot;
        ConnectionType _type;
    };

private:
    static int _lastId;
    mutable std::map<int, Connection> _connectionList;
    br_threaded_object *_parent;
};

template<typename ...S>
int signal<S...>::_lastId = 0;
}

#endif //BRUTILS_BR_OBJECT_HPP
