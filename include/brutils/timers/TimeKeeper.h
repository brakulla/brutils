/**
* Riverfall 2020. All rights reserved.
* @author Burak Ongay
**/

#ifndef EOG_SERVER_EOG_GAMEENGINE_IMPL_INCLUDE_TIMEKEEPER_TIMEKEEPER_H_
#define EOG_SERVER_EOG_GAMEENGINE_IMPL_INCLUDE_TIMEKEEPER_TIMEKEEPER_H_

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

#include <brutils/br_object.h>

/**
 * @brief
 *    Struct to hold interval information, start and end information of a time interval.
 */
struct Interval
{
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point end;
};

/*!
 * @brief
 *    A record of time keeping. Time keeper keeps track of timers as records and passes this object also to the timer
 *    user software for passing record of time.
 */
struct TimeRecord
{
  std::uint16_t id = -1;
  std::chrono::milliseconds duration;
  Interval interval;
};

/*!
 * @brief
 *    TimeKeeper class provides single threaded timer operations in a self contained thread. Add record to be notified
 *    when the given duration has passed. It creates a new thread and manages it internally. Uses conditional variables
 *    internally to prevent unnecessary cpu usage.
 */
class TimeKeeper : public brutils::br_object
{
 public:
  /*!
   * @brief
   *    Constructor. Creates an internal thread and starts running in it.
   */
  explicit TimeKeeper();
  /*!
   * @brief
   *    Virtual destructor for inheritance. Stops the internally created thread before returning.
   */
  ~TimeKeeper() override;

  brutils::signal<TimeRecord> timeout;

  /*!
   * @brief
   *    Creates a new record with unique id, end time_point calculated with system_clock::now() + duration. Adds it to
   *    record book, an internal unordered_map. Returns the created record for later reference. A thread-safe method.
   * @param[in] duration - The duration of the timer event, timeout signal will be emitted when this duration is passed
   * @throws std::invalid_argument - When the duration is 0 (zero)
   * @throws std::runtime_error - When the size is full
   * @return TimeRecord
   *    New time record object
   */
  TimeRecord AddRecord(std::chrono::milliseconds duration);
  /*!
   * @brief
   *    Cancels a record in the system. It throws if record not found with the same id. A thread-safe method.
   * @param[in] record - TimeRecord object to check the id against the ones in the system
   * @throws std::invalid_argument - When the record is not found
   */
  void CancelRecord(const TimeRecord& record);
  /*!
   * @brief
   *    Cancels a record in the system. It throws if record not found with the id. A thread-safe method.
   * @param[in] id - Id of the record to be cancelled
   * @throws std::invalid_argument - When the record is not found
   */
  void CancelRecord(std::uint16_t id);

  /*!
   * @brief
   *    Returns the size of the time keeper record book. A thread-safe method.
   * @return std::size_t
   *    The size of the time keeper record book at the time of function call
   */
  std::size_t Size();
  /*!
   * @brief
   *    Check the emptiness of the time keeper record book. A thread-safe method.
   * @return bool
   *    True if empty, false otherwise
   */
  bool Empty();

 protected:
  std::uint16_t _lastRecordId;

  std::thread _thread;
  std::atomic_bool _running;
  std::atomic_bool _notification;
  std::mutex _mutex;
  std::condition_variable _conditionVariable;

  std::unordered_map<std::uint16_t, TimeRecord> _recordBook;

  void run();
  void stop();
  void wait();
  TimeRecord& getClosestRecord();
};

#endif //EOG_SERVER_EOG_GAMEENGINE_IMPL_INCLUDE_TIMEKEEPER_TIMEKEEPER_H_
