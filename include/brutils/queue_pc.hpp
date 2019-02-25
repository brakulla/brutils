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

namespace brutils {
template<typename T>
class queue_pc {
 public:
  void push(const T &item) {
    std::unique_lock lock(_mutexQ);
    _queue.push(item);
  }

  T pop() {
    std::unique_lock lock(_mutexQ);
    _cond.wait(_mutexQ, [&] {
      return !_queue.empty();
    });
    T item = _queue.front();
    _queue.pop();
    return item;
  }

 private:
  std::queue<T> _queue;
  std::mutex _mutexQ;
  std::condition_variable _cond;
};
}

#endif //BRUTILS_QUEUE_PC_H
