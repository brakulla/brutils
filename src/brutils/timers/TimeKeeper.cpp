/**
* Riverfall 2020. All rights reserved.
* @author Burak Ongay
**/

#include "brutils/timers/TimeKeeper.h"

using namespace std::chrono;

TimeKeeper::TimeKeeper() :
    brutils::br_object(nullptr),
    timeout(this),
    _lastRecordId(0),
    _notification(false)
{
  _running = true;
  _thread = std::thread(&TimeKeeper::run, this);
}
TimeKeeper::~TimeKeeper()
{
  stop();
  wait();
}
TimeRecord TimeKeeper::AddRecord(milliseconds duration)
{
  std::unique_lock lock(_mutex);

  if (!duration.count())
    throw std::invalid_argument("Zero is not a valid argument!");
  if (std::numeric_limits<std::uint16_t>::max() == _recordBook.size())
    throw std::runtime_error("TimeKeeper is full!");

  return AddRecord(system_clock::now() + duration);
}
TimeRecord TimeKeeper::AddRecord(std::chrono::system_clock::time_point endTP)
{
  std::unique_lock lock(_mutex);

  auto now = system_clock::now();

  if (now > endTP)
    throw std::invalid_argument("Given time point is in the past!");
  if (std::numeric_limits<std::uint16_t>::max() == _recordBook.size())
    throw std::runtime_error("TimeKeeper is full!");

  TimeRecord record{
    .id = _lastRecordId++,
    .duration = std::chrono::duration_cast<milliseconds>(endTP - now),
    .interval = Interval{
      .start = now,
      .end = endTP
    }
  };

  // make sure no record exists in record book with the new id
  auto it = _recordBook.find(record.id);
  while (_recordBook.end() != it) {
    record.id += 1;
    it = _recordBook.find(record.id);
  }

  // no problem so far, store new record and notify the runner thread that there is a new record in the system
  _recordBook[record.id] = record;
  _notification = true;
  _conditionVariable.notify_one();

  return record;
}
void TimeKeeper::CancelRecord(const TimeRecord& record)
{
  CancelRecord(record.id);
}
void TimeKeeper::CancelRecord(std::uint16_t id)
{
  std::unique_lock lock(_mutex);
  auto it = _recordBook.find(id);
  if (_recordBook.end() == it)
    throw std::invalid_argument("Could not find id in record book!");

  // erase from record book and notify runner thread that there is a change in records
  _recordBook.erase(it);
  _notification = true;
  _conditionVariable.notify_one();
}
std::size_t TimeKeeper::Size()
{
  std::unique_lock lock(_mutex);
  return _recordBook.size();
}
bool TimeKeeper::Empty()
{
  std::unique_lock lock(_mutex);
  return _recordBook.empty();
}
void TimeKeeper::run()
{
  while (_running) {
    std::unique_lock lock(_mutex);

    // if empty, wait until being stopped or notified for new record
    if (this->_recordBook.empty()) {
      _conditionVariable.wait(
          lock, [this] {
            return !_running || _notification;
          }
      );
      // reset notification for further queries
      _notification = false;
      continue;
    }

    // then we have records to wait for
    auto record = getClosestRecord();
    _conditionVariable.wait_until(
        lock, record.interval.end, [this, &record] {
          return !_running || _notification || (system_clock::now() > record.interval.end);
        }
    );

    // if wake reason is notification, just clear it and do normal operations
    if (_notification) {
      _notification = false;
    }

    // if wake reason is timeout
    if (system_clock::now() > record.interval.end) {
      // check if record is still in record book
      auto it = _recordBook.find(record.id);
      if (_recordBook.end() == it)
        continue;

      timeout.emit(record);
      _recordBook.erase(it);
    }
    // if wake reason is notified or running is stopped, they will be checked in the next iteration, so no op here
  }
}
void TimeKeeper::stop()
{
  _running = false;
  _conditionVariable.notify_one();
}
TimeRecord& TimeKeeper::getClosestRecord()
{
  // record book should never be empty here
  if (_recordBook.empty())
    throw std::runtime_error("Record book is empty in getClosestRecord!");

  // get first record and compare its end time_point with others to find closest one
  auto& closestRecord = _recordBook.begin()->second;
  for (auto&[id, record]: _recordBook) {
    if (record.interval.end < closestRecord.interval.end)
      closestRecord = std::ref(record);
  }
  return closestRecord;
}
void TimeKeeper::wait()
{
  if (_thread.joinable())
    _thread.join();
}
