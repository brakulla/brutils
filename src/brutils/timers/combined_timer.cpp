/**
 * @file combined_timer.cpp
 *
 * Description
 */

#include "brutils/timers/combined_timer.h"

brutils::combined_timer::combined_timer(brutils::br_object *parent) :
    br_object(parent),
    timeout(parent),
    _stopped(true),
    _lastTimerId(0)
{

}
brutils::combined_timer::~combined_timer()
{
  stop();
}
int16_t brutils::combined_timer::addTimer(uint64_t duration_ms, bool periodic)
{
  stop();
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  TimerData_s timerStruct{false,
                          periodic,
                          _lastTimerId,
                          duration_ms,
                          (now + std::chrono::milliseconds(duration_ms))};
  ++_lastTimerId;

  _timeKeeperMap[timerStruct.id] = timerStruct;

  start();
  return timerStruct.id;
}
int16_t brutils::combined_timer::addTimer(std::chrono::milliseconds duration, bool periodic)
{
  return addTimer(duration.count(), periodic);
}
bool brutils::combined_timer::stopTimer(int16_t timerId)
{
  if (_timeKeeperMap.end() == _timeKeeperMap.find(timerId)) {
    return false;
  }
  stop();
  _timeKeeperMap.erase(timerId);
  start();
  return false;
}
void brutils::combined_timer::start()
{
  _stopped = false;
  _timerThread = std::thread(&brutils::combined_timer::run, this);
}
void brutils::combined_timer::stop()
{
  _stopped = true;
  _condVariable.notify_one();
  if (_timerThread.joinable()) {
    _timerThread.join();
  }
}
void brutils::combined_timer::run()
{
  int16_t closestTimerId = getClosestTimerId();
  TimerData_s closestTimer = _timeKeeperMap.at(closestTimerId);
  while (!_stopped) {
    std::unique_lock lock(_mutex);
    _condVariable.wait_until(lock, closestTimer.expirationDate, [&] {
      return (closestTimer.expirationDate < std::chrono::steady_clock::now() || !_stopped);
    });
    if (!_stopped) {
      timeout.emit(closestTimer.id);
      _timeKeeperMap.erase(closestTimer.id);
    }
  }
}
int16_t brutils::combined_timer::getClosestTimerId()
{
  int16_t closestId = -1;
  if (_timeKeeperMap.empty()) {
    return closestId;
  }

  auto it = _timeKeeperMap.cbegin();
  closestId = it->second.id;
  auto closestExpDate = it->second.expirationDate;
  for (++it; it != _timeKeeperMap.cend(); ++it) {
    if (it->second.expirationDate < closestExpDate) {
      closestId = it->first;
    }
  }
  return closestId;
}
