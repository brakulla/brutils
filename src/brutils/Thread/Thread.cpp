//
// Created by congay on 12/12/2019.
//

#include "brutils/Thread/Thread.h"

using namespace brutils;

Thread::Thread(int threadId)
{

}
Thread::~Thread()
{

}
bool Thread::isBusy()
{
  return false;
}
template<typename F, typename... Args>
void Thread::execute(F function, Args...)
{

}
