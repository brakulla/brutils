/*
 * @file ThreadPool.h
 * @class ThreadPool
 *
 * This class provides a thread pool structure. It defaults the thread size to hardware_concurrency().
 * Normally, Thread class has API to specify the arguments for a function to execute. In thread pool, that is not
 * used. The arguments are bound to the function so make them the same type (std::function<void()>) to be able to
 * store them in a vector. If all the threads are busy, the functions are stored to be executed when one of the
 * threads are finishes execution.
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_THREAD_THREADPOOL_H_
#define BRUTILS_INCLUDE_BRUTILS_THREAD_THREADPOOL_H_

#include "brutils/br_object.h"

#include <chrono>
#include <unordered_map>

#include "Thread.h"
#include "brutils/timers/combined_timer.h"

namespace brutils
{

enum class ThreadPoolErrorCode
{
  NoError = 0,
  UnknownThreadId,
  UnknownTimerId,
  BusyThreadResource
};

struct ThreadPoolError
{
  ThreadPoolErrorCode errorCode;
  std::string errorStr;
};

class ThreadPool : public br_object
{
 public:
  explicit ThreadPool(std::chrono::milliseconds idleThreadTimeout = std::chrono::milliseconds(30000),
                      std::size_t maxThreadSize = std::thread::hardware_concurrency(),
                      br_object* parent = nullptr);

  template <typename F, typename... Args, std::enable_if_t<std::is_invocable_v<F&&, Args&&...>, int> = 0>
  void execute(F&& function, Args&&... args);

  signal<ThreadPoolError> errorOccured;
  ThreadPoolError getLastError();

 private:
  std::chrono::milliseconds _timeoutDuration;
  size_t _maxThreadCount;
  std::queue<std::function<void()>> _functionBuffer;

  std::unordered_map<std::thread::id, int> _threadId_timerId_map;
  std::unordered_map<int, std::thread::id> _timerId_threadId_map;
  std::unordered_map<std::thread::id, std::unique_ptr<Thread>> _busyThreadMap;
  std::unordered_map<std::thread::id, std::unique_ptr<Thread>> _idleThreadMap;

 private:
  combined_timer _timer;
  slot<int16_t> _timerTimeoutSlot;
  std::map<int16_t, std::thread::id> _timerThreadMap;
  void timerTimeout_slotFunc(int16_t timerId);

  slot<std::thread::id> _threadExecutionFinishedSlot;
  void executionFinished_slotFunc(std::thread::id finishedThreadId);

  // utility functions
  void startTimerByThreadId(const std::thread::id& threadId);
  void stopTimerByThreadId(const std::thread::id& threadId);

 private:
  ThreadPoolError _error;
};

}

#endif //BRUTILS_INCLUDE_BRUTILS_THREAD_THREADPOOL_H_
