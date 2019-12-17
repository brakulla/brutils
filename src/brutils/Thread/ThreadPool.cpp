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
    _timerTimeout(this),
    _threadFinishedExecuting(this)
{
  _timerTimeout.setSlotFunction(std::bind(&ThreadPool::timeoutSlot, this, std::placeholders::_1));
  _threadFinishedExecuting.setSlotFunction(std::bind(&ThreadPool::executionFinished, this, std::placeholders::_1));

  _threadList.reserve(maxThreadSize);
  for (int i = 0; i < maxThreadSize; ++i) {
    _threadList.push_back(std::unique_ptr<Thread>());
  }
}
template<typename F, typename... Args, std::enable_if_t<std::is_invocable_v<F &&, Args &&...>, int>>

void ThreadPool::execute(F &&function, Args &&... args)
{
  bool executed = false;
  for (auto& it: _threadList) {
    if (!it.second->isBusy()) {

      // TODO: find the timerId of this thread and reset the timer of it

      it.second->execute(std::forward<F>(function), std::forward<Args>(args)...);
      executed = true;
      break;
    }
  }

  if (!executed) {
    if (_threadList.size() <= _maxThreadCount) {
      // TODO: emit error, there is no size
    } else { // if not executed, push it to queue to be executed when there is a free slot
      std::function<void()> f = std::bind(std::forward<F>(function), std::forward<Args>(args)...);
      _functionBuffer.push(f);
    }
  }
}
ThreadPoolError ThreadPool::getError()
{
  return _error;
}
void ThreadPool::timeoutSlot(int16_t timerId)
{
  auto it_timer = _timerThreadMap.find(timerId);
  if (_timerThreadMap.end() == it_timer) {
    errorOccured.emit({
      ThreadPoolErrorCode::UnknownTimerId,
      "Timeout timer is not known: " + std::to_string(timerId)
    });
    return;
  }
  auto it_thread = _threadList.find(it_timer->second);
  if (_threadList.end() == it_thread) {
    errorOccured.emit({
      ThreadPoolErrorCode::UnknownThreadId,
      "Thread id retrieved from timer id is not known in timeout slot"
    });
    return;
  }
  auto& thread = it_thread->second;
  if (thread->isBusy()) {
    errorOccured.emit({
      ThreadPoolErrorCode::BusyThreadResource,
      "Thread is busy (timeout idle thread). Check Thread resource for bugs. Thread Id: " + str_threadIdToStr(it_timer->second);
    });
    return;
  }

  thread.reset();
}
void ThreadPool::executionFinished(std::thread::id threadId)
{
  auto it = _threadList.find(threadId);
  if (_threadList.end() == it) {
    errorOccured.emit({
      ThreadPoolErrorCode::UnknownThreadId,
      "Thread id received in execution finished slot is not known"
    });
    return;
  }
  auto& thread = it->second;
  if (thread->isBusy()) {
    errorOccured.emit({
      ThreadPoolErrorCode::BusyThreadResource,
      "Thread is busy (that finished execution). Check Thread resource for bugs. Thread Id: " + str_threadIdToStr(threadId);
    });
    return;
  }

  if (_functionBuffer.empty()) {
    // create a timeout timer for this thread
    // if a new execution is received, it will be run in idle threads if there is any
    // if timeout occurs, idle thread will be deleted
    int16_t timerId = _timer.addTimer(_timeoutDuration);
    _timerThreadMap[timerId] = threadId;
  } else {
    // run the waiting execution
    auto func = std::move(_functionBuffer.front());
    _functionBuffer.pop();
    thread->execute(func);
  }
}
