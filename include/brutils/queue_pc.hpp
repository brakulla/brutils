/**
 * @file queue_pc.h
 *
 * Description
 */

#ifndef BRUTILS_QUEUE_PC_H
#define BRUTILS_QUEUE_PC_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace brutils {
template<typename T>
class queue_pc {
 public:
  queue_pc() : _stopped(false) {}

  void push(const T &item) {
    std::unique_lock lock(_mutexQ);
    _queue.push(item);
    _cond.notify_one();
  }

  bool waitPop() {
    std::unique_lock lock(_mutexQ);
    _cond.wait(lock, [&] {
      return !_queue.empty() || _stopped;
    });
    if (_stopped)
      return false;
    return true;
  }

  T pop() {
    T item = _queue.front();
    _queue.pop();
    return item;
  }

  void stop() {
    _stopped = true;
    _cond.notify_one();
  }

 private:
  std::atomic_bool _stopped;
  std::queue<T> _queue;
  std::mutex _mutexQ;
  std::condition_variable _cond;
};
}

#endif //BRUTILS_QUEUE_PC_H
