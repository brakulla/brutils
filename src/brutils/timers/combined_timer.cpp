//
// Created by congay on 15/10/2019.
//

#include "brutils/timers/combined_timer.h"
#include <spdlog/spdlog.h>

brutils::combined_timer::combined_timer(brutils::br_object *parent) :
    br_object(parent),
    timeout(parent),
    _stopped(true),
    _lastTimerId(0)
{
  spdlog::info("Combined timer constructor");
}
brutils::combined_timer::~combined_timer()
{
  stop();
}
int16_t brutils::combined_timer::addTimer(uint64_t duration_ms, bool periodic)
{
  spdlog::trace("combined_timer::addTimer - ");
  stop();
  std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
  TimerData_s timerStruct{false,
                          periodic,
                          _lastTimerId,
                          duration_ms,
                          (now + std::chrono::milliseconds(duration_ms))};
  ++_lastTimerId;

  _timeKeeperMap[timerStruct.id] = timerStruct;
  spdlog::trace("combined_timer::addTimer - With timer id {}", timerStruct.id);

  start();
  return timerStruct.id;
}
int16_t brutils::combined_timer::addTimer(std::chrono::milliseconds duration, bool periodic)
{
  return addTimer(duration.count(), periodic);
}
bool brutils::combined_timer::stopTimer(int16_t timerId)
{
  spdlog::trace("combined_timer::stopTimer - ");
  std::scoped_lock lock(_dataMutex);
  spdlog::trace("combined_timer::stopTimer -- ");
  if (_timeKeeperMap.end() == _timeKeeperMap.find(timerId)) {
    spdlog::trace("combined_timer::stopTimer - Could not find the given timer id {}", timerId);
    return false;
  }
  _dataMutex.unlock();
  stop();
  _dataMutex.lock();
  spdlog::trace("combined_timer::stopTimer - Removing timer id {}", timerId);
  _timeKeeperMap.erase(timerId);
  start();
  return true;
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
  while (!_stopped) {
    int16_t closestTimerId = getClosestTimerId();
    if (-1 == closestTimerId) {
      spdlog::trace("combined_timer::run - No timer, stopping thread");
      _stopped = true;
      return;
    }
    TimerData_s closestTimer = _timeKeeperMap.at(closestTimerId);

    std::unique_lock lock(_mutex);
    _condVariable.wait_until(lock, closestTimer.expirationDate, [&] {
      return (closestTimer.expirationDate <  std::chrono::steady_clock::now() || _stopped);
    });

    if (!_stopped) {
      std::scoped_lock lock(_dataMutex);
      spdlog::trace("combined_timer::run - Timeout for timer id {}", closestTimer.id);
      timeout.emit(closestTimer.id);
      if (!closestTimer.periodic) {
        _timeKeeperMap.erase(closestTimer.id);
      } else {
        _timeKeeperMap[closestTimerId].expirationDate =
            std::chrono::steady_clock::now() + std::chrono::milliseconds(closestTimer.duration);
      }
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
      closestExpDate = it->second.expirationDate;
    }
  }
  return closestId;
}
