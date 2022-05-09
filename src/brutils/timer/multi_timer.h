/**
 * Multi timer implementation
 */

#ifndef BRUTILS_SRC_BRUTILS_TIMER_MULTI_TIMER_H_
#define BRUTILS_SRC_BRUTILS_TIMER_MULTI_TIMER_H_

#include <chrono>
#include <functional>
#include <memory>

namespace brutils
{
  /**
   * @brief multi_timer class is a class that implements multiple timers in single thread instead of a thread per timer
   * approach. Use this multi_timer instead of simple_timer if more than one timer is to be created in a single place.
   *
   * It creates a new thread on start and waits until one of the possible triggers:
   *    - a new timer is added when there is no other timer
   *    - a new timer is added when there is at least one timer running, and it has closer end time than existing ones
   *    - the closest timer has expired
   *    - the closest timer is cancelled
   *
   * It internally uses system_clock, therefore susceptible to vulnerabilities of system_clock.
   *
   * It can trigger a callback per timer, or a default callback if former is not set.
   *
   * It provides detailed information about each timer in the system.
   */
  class multi_timer
  {
    class impl;
    std::unique_ptr<impl> _impl;

   public:
    /**
     * @brief A time record data object
     */
    struct record
    {
      long id = -1;
      bool periodic;
      std::chrono::milliseconds duration;
      std::chrono::system_clock::time_point start;
      std::chrono::system_clock::time_point end;
    };

    /**
     * @brief Constructor
     */
    multi_timer();
    /**
     * @brief Destructor
     */
    ~multi_timer();

    /**
     * @brief Starts the timer operation. This creates a new thread.
     */
    void start();
    /**
     * @brief Stops the timer operation. This waits until the underlying thread is finished.
     */
    void stop();

    /**
     * @brief Sets the default callback.
     * @param callback
     */
    void set_default_callback(std::function<void(long)> callback);
    /**
     * @brief Adds a non-periodic timer with given duration. Uses default callback.
     * @param duration
     * @return
     */
    record add_timer(std::chrono::milliseconds duration);
    /**
     * @brief Adds a timer with given periodic property and duration. Uses default callback.
     * @param duration
     * @param periodic
     * @return
     */
    record add_timer(std::chrono::milliseconds duration, bool periodic);
    /**
     * @brief Adds a timer with given duration and callback. Uses the given callback instead of the default one.
     * @param duration
     * @param callback
     * @return
     */
    record add_timer(std::chrono::milliseconds duration, std::function<void()> callback);
    /**
     * @brief Adds a timer with given duration, periodic property and callback. Uses the given callback instead of
     * the default one.
     * @param duration
     * @param periodic
     * @param callback
     * @return
     */
    record add_timer(std::chrono::milliseconds duration, bool periodic, std::function<void()> callback);
    /**
     * @brief Adds a non-periodic timer that will be expired on the given time point. Uses default callback.
     * @param end_time
     * @return
     */
    record add_timer(std::chrono::system_clock::time_point end_time);
    /**
     * @brief Adds a non-periodic timer that will be expired on the given time point. Uses the given callback instead
     * of the default one.
     * @param end_time
     * @param callback
     * @return
     */
    record add_timer(std::chrono::system_clock::time_point end_time, std::function<void()> callback);
    /**
     * @brief Cancels the timer with given id if it exists.
     * @param id
     */
    void cancel_timer(long id);
  };
} // brutils

#endif //BRUTILS_SRC_BRUTILS_TIMER_MULTI_TIMER_H_
