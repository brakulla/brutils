/*
 * @file Thread.h
 * @class Thread
 *
 * This class is a RAII thread resource.
 * It inherits br_threaded_object to run itself in a new thread with RAII mechanism.
 * The "execute()" function is expected to be called from outside of this class, meaning from another thread.
 * Signal/slot mechanism provided by br_object and br_threaded_object will provide the slot function running in
 * the thread of this class. As a result, function given to "execute()" will be run within this class' thread.
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_
#define BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_

#include <brutils//br_object.h>

namespace brutils
{

class Thread : public br_threaded_object
{
 public:
  explicit Thread();
  virtual ~Thread() = default;

 public:
  bool isBusy();
  template<typename F, typename... Args, std::enable_if_t<std::is_invocable_v<F&&, Args&&...>, int> = 0>
  void execute(F&& function, Args&&...);

  signal<std::thread::id> finishedExecuting;

 private:
  signal<std::function<void()>> _newExecutionReceived;
  slot<std::function<void()>> _executor;

 private:
  std::atomic_bool _busy;
  void executor_slot(std::function<void()>&& executee);

};

};

#endif //BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_
