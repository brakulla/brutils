//
// Created by brakulla on 3/10/19.
//

#ifndef BRUTILS_TIMER_H
#define BRUTILS_TIMER_H

#include <thread>
#include <condition_variable>
#include <atomic>
#include <functional>

namespace brutils {
class timer {
 public:
  void start(std::chrono::milliseconds millisec, bool periodic = false) {
    _periodic = periodic;
    _deadline = Clock::now() + millisec;
    _thread = std::thread(&timer::run, this);
  }
  void start(int millisec, bool periodic = false) {
    this->start(std::chrono::milliseconds(millisec), periodic);
  }
  void restart(int millisec) {
    this->stop();
    this->start(millisec);
  }
  void stop() {
    _running = false;
    _cond.notify_one();
    if (_thread.joinable()) {
      _thread.join();
    }
  }
  void notify(std::function<void()> const &slot) {
    _func = slot;
  }
 private:
  std::atomic_bool _running = false;
  std::thread _thread;
  std::mutex _mutex;
  std::condition_variable _cond;
  std::function<void()> _func;
  std::chrono::time_point<std::chrono::system_clock> _deadline;
  bool _periodic;
  using Clock = std::chrono::system_clock;
 private:
  void run() {
    do {
      std::unique_lock lock(_mutex);
      _cond.wait_until(lock, _deadline, [&] {
        return (_deadline < Clock::now() || !_running);
      });
      if (_running) {
        if (!_periodic) {
          _running = false;
        }
        _func();
      }
    } while(_periodic);
  }
};
}

#endif //BRUTILS_TIMER_H
