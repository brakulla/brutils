/**
 * @file br_object.h
 *
 * Description
 */

#ifndef BRUTILS_BR_OBJECT_H
#define BRUTILS_BR_OBJECT_H

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

class br_object
{
public:
    br_object(const br_object &) = delete;
    br_object(br_object &&) = delete;
    br_object &operator=(const br_object &) = delete;

    explicit br_object(br_object *parent)
        : _parent(parent)
    {}
    virtual ~br_object() = default;

public:
    virtual std::thread::id getThreadId() const
    {
        if (nullptr == _parent)
            return std::this_thread::get_id();
        else return _parent->getThreadId();
    }

    virtual void addEvent(const std::function<void()> &event)
    {
        if (nullptr == _parent)
            throw std::runtime_error("Object has not parent");
        _parent->addEvent(event);
    }

    virtual void addEvent(const std::function<void()> &event,
                          bool &ec) noexcept // TODO: create error object and use it instead of bool
    {
        if (nullptr == _parent)
            ec = true;
        else _parent->addEvent(event, ec);
    }

    virtual br_object *getRootObject()
    {
        return _parent;
    }

private:
    br_object *_parent;
};

/**
 * @class br_threaded_object
 * @brief A base class for objects that are to be run in a seperate thread.
 */
class br_threaded_object: public br_object
{
public:
    /**
     * @brief Constructor of br_threaded_object. In this constructor, the object is run on a newly created thread.
     */
    br_threaded_object()
        : _running(true), br_object(this)
    {
        _thread = std::thread(&br_threaded_object::run, this);
    };

    virtual ~br_threaded_object() override
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
    /**
     * @brief Returns thread id of br_threaded_object instance.
     * @return std::thread:id Thread id of br_threaded_object instance.
     */
    std::thread::id getThreadId() const override
    {
        return _thread.get_id();
    }

    /**
     * @brief Add an event to event loop of br_threaded_object instance.
     * @param [in] event const std::function<void()> - This event function will be run in the thread of br_threaded_object instance.
     */
    void addEvent(const std::function<void()> &event) override
    {
        _eventList.push(event);
        _cond.notify_one();
    }

    void addEvent(const std::function<void()> &event, bool &ec) noexcept override
    {
        ec = true;
        _eventList.push(event);
        _cond.notify_one();
    }

    br_object *getRootObject() override
    {
        return dynamic_cast<br_object *>(this);
    }

protected:
    std::thread _thread;

private:
    std::atomic_bool _running;

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
    explicit slot(br_object *parent)
        : _init(false), _parent(parent)
    {}
    ~slot() = default;

    /**
     * @brief If this overloaded constructor is used, then this instance is ready to make connections.
     * @param [in] function
     * @param [in] parent
     */
    explicit slot(std::function<void(Args...)> function, br_object *parent = nullptr)
        : _init(true), _parent(parent)
    {
        _function = function;
    }

public:
    /**
     * @brief Returns thread of signal.
     *          If parent is given, returns parent thread; returns caller thread id, otherwise
     * @return std::thread::id
     */
    std::thread::id getThreadId()
    {
        if (nullptr != _parent)
            return _parent->getRootObject()->getThreadId();
        else return std::this_thread::get_id();
    }

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
    br_object *_parent;

private:
    /**
     * @brief Adds the wrapped function to parent object's event loop. Internally, used for queued connections.
     * @param [in] args
     */
    void addEventToParent(Args... args) const
    {
        if (nullptr == _parent)
            return;
        _parent->getRootObject()->addEvent([=]
                          {
                              _function(args...);
                          });
    }

    bool hasParent()
    {
        return nullptr != _parent;
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

    explicit signal(br_object *parent)
        : _parent(parent)
    {};
    ~signal() = default;

public:
    /**
     * @brief Returns thread of signal.
     *          If parent is given, returns parent thread; returns caller thread id, otherwise
     * @return std::thread::id
     */
    std::thread::id getThreadId()
    {
        if (nullptr != _parent)
            return _parent->getRootObject()->getThreadId();
        else return std::this_thread::get_id();
    }

    /**
     * @brief Creates a connection between the instance and input slot instance in given `ConnectionType`.
     * @param [in] slot
     * @param [in] type
     * @return id Returns created connection identifier.
     */
    void connect(slot<Args...> &slot, ConnectionType type = ConnectionType::Auto)
    {
        ConnectionType connectionType = type;
        if (nullptr == _parent) {
            connectionType = ConnectionType::Direct;
        }
        else if (nullptr == _parent->getRootObject()) {
            connectionType = ConnectionType::Direct;
        }
        else if (!slot.hasParent()) {
            connectionType = ConnectionType::Direct;
        }
        else if (type == ConnectionType::Auto) {
            if (getThreadId() == slot.getThreadId())
                connectionType = ConnectionType::Direct;
            else
                connectionType = ConnectionType::Queued;
        }

        if (connectionType == ConnectionType::Direct)
            _directConnections.push_back(&slot);
        else _queuedConnections.push_back(&slot);
    }

    /**
     * @brief Destroys all connections of this signal.
     */
    void disconnect()
    {
        _directConnections.clear();
        _queuedConnections.clear();
    }

    /**
     * @brief Destroys the connection previously created by `connect` function.
     * @param slot Slot the connection will be disconnected with.
     */
    void disconnect(slot<Args...> &slot)
    {
        for (auto it = _directConnections.begin(); it != _directConnections.end(); ++it) {
            if (*it == slot)
                it = _directConnections.erase(it);
            else ++it;
        }
        for (auto it = _queuedConnections.begin(); it != _queuedConnections.end(); ++it) {
            if (*it == slot)
                it = _queuedConnections.erase(it);
            else ++it;
        }
    }

    /**
     * @brief Emits the signal. Passes the parameters to slot function in a thread-safe way.
     * @param [in] parameters
     */
    void emit(Args... parameters)
    {
        if (nullptr == _parent) {
            callDirectConnections(parameters...);
        }
        else {
            if (std::this_thread::get_id() == _parent->getThreadId()) {
                callDirectConnections(parameters...);
                callQueuedConnections(parameters...);
            }
            else {
                _parent->getRootObject()->addEvent([=]
                                  {
                                      callDirectConnections(parameters...);
                                      callQueuedConnections(parameters...);
                                  });
            }
        }
    }

private:
    static int _lastId;
    std::vector<slot<Args...> *> _directConnections;
    std::vector<slot<Args...> *> _queuedConnections;
    br_object *_parent;

private:
    void callDirectConnections(Args... parameters)
    {
        for (auto slot: _directConnections) {
            slot->call(parameters...);
        }
    }

    void callQueuedConnections(Args... parameters)
    {
        for (auto slot: _queuedConnections) {
            slot->addEventToParent(parameters...);
        }
    }
};

}

#endif //BRUTILS_BR_OBJECT_H