//
// Created by congay on 12/12/2019.
// a wrapper around br_threaded_object to simplify its meaning

#ifndef BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_
#define BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_

#include <brutils//br_object.h>

namespace brutils
{

class Thread : public br_threaded_object
{
 public:
  explicit Thread(int threadId = -1);
  virtual ~Thread();

 public:
  bool isBusy();
  template <typename F, typename... Args>
  void execute(F function, Args...);

 private:
  signal<std::function<void()>> _newExecutionReceived;
  slot<std::function<void()>> _executor;

 private:

};

};

#endif //BRUTILS_INCLUDE_BRUTILS_THREAD_THREAD_H_
