//
// Created by Burak Ongay on 15/12/2019.
//

#include "brutils/Thread/ThreadPool.h"

using namespace brutils;

ThreadPool::ThreadPool(std::chrono::milliseconds idleThreadTimeout, std::size_t maxThreadSize, br_object *parent) :
    br_object(parent),
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
  for (auto &thread : _threadList) {
    if (thread) { // check if thread exists in the slot (deferences to std::unique_ptr<Thread>)
      if (!thread->isBusy()) {
        thread->execute(std::forward<F>(function), std::forward<Args>(args)...);
        executed = true;
        break;
      }
    } else {
      thread = std::make_unique<Thread>();
      thread->finishedExecuting.connect(_threadFinishedExecuting);
      thread->execute(std::forward<F>(function), std::forward<Args>(args)...);
      executed = true;
      break;
    }
  }
  if (!executed) { // if not executed, push it to queue to be executed when there is a free slot
    std::function<void()> f = std::bind(std::forward<F>(function), std::forward<Args>(args)...);
    _functionBuffer.push(f);
  }
}
void ThreadPool::timeoutSlot(int16_t timerId)
{
  std::thread::id threadId = _timerThreadMap.at(timerId);
  for (auto& thread: _threadList) {
    if (thread->getThreadId() == threadId) {
      thread.reset();
    }
  }
}
void ThreadPool::executionFinished(std::thread::id threadId)
{
  for (auto& thread: _threadList) {
    if (thread->getThreadId() == threadId) {
      if (!_functionBuffer.empty()) {
        if (!thread->isBusy()) {
          auto& front = _functionBuffer.front();
          _functionBuffer.pop();
          execute(front);
        } else {
          // this should not be happening, do something about it
        }
      } else {
        int16_t timerId = _timer.addTimer(_timeoutDuration);
        _timerThreadMap[timerId] = threadId;
      }
      break;
    }
  }
}
