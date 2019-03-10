//
// Created by brakulla on 3/10/19.
//

#ifndef BRUTILS_TIMER_H
#define BRUTILS_TIMER_H

#include <thread>
#include <condition_variable>
#include <atomic>

namespace brutils {
class timer {
 public:
  void start(int millisec) {
    auto now = std::chrono::system_clock::now();
    _deadline = now + std::chrono::milliseconds(millisec);
    _thread = std::thread(&timer::run, this);
  }
  void restart(int millisec) {
    _restarted = true;
    auto now = std::chrono::system_clock::now();
    _deadline = now + std::chrono::milliseconds(millisec);
    _cond.notify_one();
  }
  void stop() {
    _stopped = true;
    _cond.notify_one();
  }
  void notify(std::function<void()> const &slot) {
    _func = slot;
  }
 private:
  std::atomic_bool _restarted = false;
  std::atomic_bool _stopped = false;
  std::chrono::time_point<std::chrono::system_clock> _deadline;
  std::thread _thread;
  std::mutex _mutex;
  std::condition_variable _cond;
  std::function<void()> _func;
 private:
  void run() {
    while (_restarted) {
      _restarted = false;
      std::unique_lock lock(_mutex);
      _cond.wait_until(lock, _deadline, [&] { return _deadline < std::chrono::system_clock::now(); });
    }
    if (!_stopped) {
      _func();
      _thread.join();
    }
  }
};
}

#endif //BRUTILS_TIMER_H
