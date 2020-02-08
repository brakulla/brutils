/**
 * @file combined_timer.h
 *
 * Description
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_TIMERS_COMBINED_TIMER_H_
#define BRUTILS_INCLUDE_BRUTILS_TIMERS_COMBINED_TIMER_H_

#include "brutils/br_object.h"

#include <map>
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace brutils {

class combined_timer : br_object
{
 public:
  explicit combined_timer(br_object *parent = nullptr);
  ~combined_timer() override;

  int16_t addTimer(uint64_t duration_ms, bool periodic = false);
  int16_t addTimer(std::chrono::milliseconds duration, bool periodic = false);
  bool stopTimer(int16_t timerId);

 public: // signals
  signal<int16_t> timeout; // id

 private:
  void start();
  void stop();
  void run();
  int16_t getClosestTimerId();

 private:
  struct TimerData_s {
    bool running = false;
    bool periodic = false;
    int16_t id = -1;
    uint64_t duration = 0;
    std::chrono::steady_clock::time_point expirationDate;
  };

 private:
  int16_t _lastTimerId;
  std::map<int16_t, TimerData_s> _timeKeeperMap;

  std::atomic_bool _stopped;
  std::mutex _mutex;
  std::condition_variable _condVariable;
  std::thread _timerThread;

};

} // namespace brutils

#endif //BRUTILS_INCLUDE_BRUTILS_TIMERS_COMBINED_TIMER_H_
