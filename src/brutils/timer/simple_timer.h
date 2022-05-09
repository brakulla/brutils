//
// Created by brakulla on 3/10/19.
//

#ifndef BRUTILS_TIMER_H
#define BRUTILS_TIMER_H

#include <chrono>
#include <functional>
#include <memory>

namespace brutils
{
  /**
   * @brief simple_timer class is a timer class that is implementing a simple timer operation.
   *
   * It creates a new thread and waits until either a command is received (stop/restart) or the time elapses, whichever
   * is sooner.
   *
   * It internally uses steady_clock, therefore it is not affected by time zone changes.
   *
   * The timer can be set to be periodic.
   *
   * The timer can be set to call a callback function when the timer is elapsed, or it can be queried with `has_elapsed`
   * function call.
   *
   * The timer objects can be moved but cannot be copied.
   */
  class simple_timer
  {
    class impl;
    std::unique_ptr<impl> _impl;

   public:
    /**
     * @brief Constructor
     */
    simple_timer();
    /**
     * @brief Destructor
     */
    ~simple_timer();

    /**
     * @brief Copy constructor and assignment are deleted
     */
    simple_timer(simple_timer const&) = delete;
    simple_timer& operator=(simple_timer const&) = delete;

    /**
     * @brief Move constructor and assignment are defaulted
     */
    simple_timer(simple_timer&&) = default;
    simple_timer& operator=(simple_timer&&) = default;

    /**
     * @brief Set the timer periodic
     * @param is_periodic
     */
    void set_periodic(bool is_periodic);
    /**
     * @brief Set the callback function to be called when the timer elapses. Called everytime the timer elapses if
     * periodic is set to true. The callback function will be called from the new thread. Do not block the execution
     * on this callback as it will block the execution of the timer.
     * @param cb
     */
    void set_callback(std::function<void()> cb);
    /**
     * @brief Set the duration of the timer. If set to zero `0`, then it will elapse as soon as the new thread is
     * created successfully. The callback function, if set, will be called from the new thread.
     * @param duration
     */
    void set_duration(std::chrono::milliseconds duration);
    /**
     * @brief Start the timer
     */
    void start();
    /**
     * @brief Start the timer with the given duration.
     *
     * Shortcut to:
     * @code{.cpp}
     * set_duration(std::chrono::milliseconds{10});
     * start();
     * @endcode
     * @param duration
     */
    void start(std::chrono::milliseconds duration);
    /**
     * @brief Restart the timer without calling the callback, if set. It will simply start the timer again.
     */
    void restart();
    /**
     * @brief Restart the timer without calling the callback, if set. It will simply start the timer again, with
     * the given duration this time.
     * @param duration
     */
    void restart(std::chrono::milliseconds duration);
    /**
     * @brief Stops the timer without calling the callback.
     */
    void stop();

    /**
     * @brief Is running check. Returns true if the timer is running, false otherwise.
     * @return
     */
    [[nodiscard]] bool is_running() const;
    /**
     * @brief Has elapsed check. Returns true if the timer has elapsed and not running anymore, false otherwise.
     * @return
     */
    [[nodiscard]] bool has_elapsed() const;
  };
}

#endif //BRUTILS_TIMER_H
