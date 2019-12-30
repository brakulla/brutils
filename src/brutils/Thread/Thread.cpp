//
// Created by congay on 12/12/2019.
//

#include "brutils/Thread/Thread.h"

using namespace brutils;

Thread::Thread() :
    finishedExecuting(this),
    _newExecutionReceived(this),
    _executor(this),
    _busy(true)
{
  _executor.setSlotFunction(std::bind(&Thread::executor_slot, this, std::placeholders::_1));
  _newExecutionReceived.connect(_executor);
}
bool Thread::isBusy()
{
  return _busy;
}
template<typename F, typename... Args, std::enable_if_t<std::is_invocable_v<F&&, Args&&...>, int>>
void Thread::execute(F&& function, Args&&... args)
{
  std::function<void()> bindedFunction = std::bind(std::forward<F>(function), std::forward<Args>(args)...);
  _newExecutionReceived.emit(bindedFunction);
}
void Thread::executor_slot(std::function<void()>&& executee)
{
  _busy = true;
  executee();
  _busy = false;
  finishedExecuting.emit(std::this_thread::get_id());
}
