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
  ThreadPoolError getError();

 private:
  std::chrono::milliseconds _timeoutDuration;
  size_t _maxThreadCount;
  std::unordered_map<std::thread::id, std::unique_ptr<Thread>> _threadList;
  std::queue<std::function<void()>> _functionBuffer;

 private:
  combined_timer _timer;
  slot<int16_t> _timerTimeout;
  std::map<int16_t, std::thread::id> _timerThreadMap;
  void timeoutSlot(int16_t timerId);

  slot<std::thread::id> _threadFinishedExecuting;
  void executionFinished(std::thread::id threadId);

 private:
  ThreadPoolError _error;
};

}

#endif //BRUTILS_INCLUDE_BRUTILS_THREAD_THREADPOOL_H_
