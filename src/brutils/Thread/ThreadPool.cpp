//
// Created by Burak Ongay on 15/12/2019.
//

#include "brutils/Thread/ThreadPool.h"

#include "brutils/string_utils.h"

using namespace brutils;

ThreadPool::ThreadPool(std::chrono::milliseconds idleThreadTimeout, std::size_t maxThreadSize, br_object *parent) :
    br_object(parent),
    errorOccured(parent),
    _maxThreadCount(maxThreadSize),
    _timeoutDuration(idleThreadTimeout),
    _timerTimeoutSlot(this),
    _threadExecutionFinishedSlot(this),
    _error({
      ThreadPoolErrorCode::NoError,
      ""
    })
{
  _timerTimeoutSlot.setSlotFunction(std::bind(&ThreadPool::timerTimeout_slotFunc, this, std::placeholders::_1));
  _threadExecutionFinishedSlot.setSlotFunction(std::bind(&ThreadPool::executionFinished_slotFunc, this, std::placeholders::_1));

  _threadId_timerId_map.reserve(maxThreadSize);
  _timerId_threadId_map.reserve(maxThreadSize);
  _busyThreadMap.reserve(maxThreadSize);
}
template<typename F, typename... Args, std::enable_if_t<std::is_invocable_v<F &&, Args &&...>, int>>


void ThreadPool::execute(F &&function, Args &&... args)
{
  if (!_idleThreadMap.empty()) {
    std::thread::id idleThreadId = _idleThreadMap.begin()->first;
    auto& idleThread = _idleThreadMap.begin()->second;

    stopTimerByThreadId(idleThreadId);

    _idleThreadMap.erase(_idleThreadMap.begin());
    idleThread->execute(function, args...);
    _busyThreadMap[idleThread->getThreadId()] = std::move(idleThread);
    return;
  } else if (_busyThreadMap.size() < _maxThreadCount) {
    auto newThread = std::make_unique<Thread>();
    _busyThreadMap[newThread->getThreadId()] = std::move(newThread);
  } else {
    std::function<void()> f = std::bind(std::forward<F>(function), std::function<Args>(args)...);
    _functionBuffer.push(f);
  }
}
ThreadPoolError ThreadPool::getLastError()
{
  return _error;
}
void ThreadPool::timerTimeout_slotFunc(int16_t timerId)
{
  if (_timerId_threadId_map.end() == _timerId_threadId_map.find(timerId)) {
    _error = ThreadPoolError {
        ThreadPoolErrorCode::UnknownTimerId,
        "Timer id received from timeout timer is not known. Timer id: " + std::to_string(timerId)
    };
    errorOccured.emit(_error);
    return;
  }

  auto timedOutThreadId = _timerId_threadId_map[timerId];
  _timerId_threadId_map.erase(timerId);
  _threadId_timerId_map.erase(timedOutThreadId);
  _idleThreadMap.erase(timedOutThreadId);
}
void ThreadPool::executionFinished_slotFunc(std::thread::id finishedThreadId)
{
  if (_busyThreadMap.end() == _busyThreadMap.find(finishedThreadId)) {
    // ERROR: CRITICAL INTERNAL ERROR
    _error = ThreadPoolError {
        ThreadPoolErrorCode::UnknownThreadId,
        "Thread id received in execution finished slot is not known"
    };
    errorOccured.emit(_error);
    return;
  }
  auto& finishedThread = _busyThreadMap[finishedThreadId];
  if (finishedThread->isBusy()) {
    _error = ThreadPoolError {
        ThreadPoolErrorCode::BusyThreadResource,
        "Thread that finished execution is busy. Check Thread resource for bugs. Thread Id: " + str_threadIdToStr(finishedThreadId)
    };
    errorOccured.emit(_error);
    return;
  }

  if (!_functionBuffer.empty()) {
    auto& f = _functionBuffer.front();
    _functionBuffer.pop();
    finishedThread->execute(f);
    return;
  } else {
    _idleThreadMap[finishedThreadId] = std::move(finishedThread);
    _busyThreadMap.erase(finishedThreadId);
    startTimerByThreadId(finishedThreadId);
  }
}
void ThreadPool::startTimerByThreadId(const std::thread::id &threadId)
{
  int16_t newTimerId = _timer.addTimer(_timeoutDuration);
  _timerId_threadId_map[newTimerId] = threadId;
  _threadId_timerId_map[threadId] = newTimerId;
}
void ThreadPool::stopTimerByThreadId(const std::thread::id &threadId)
{
  auto el = _threadId_timerId_map.find(threadId);
  if (_threadId_timerId_map.end() == el) {
    // ERROR: CRITICAL INTERNAL ERROR
    return;
  }
  int16_t timerId = el->second;
  if (_timerId_threadId_map.end() == _timerId_threadId_map.find(timerId)) {
    // ERROR: CRITICAL INTERNAL ERROR
    return;
  }
  _timer.stopTimer(timerId);
  _threadId_timerId_map.erase(threadId);
  _timerId_threadId_map.erase(timerId);
}
