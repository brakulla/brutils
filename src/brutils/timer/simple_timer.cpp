/**
 * Simple timer implementation
 */

#include "simple_timer.h"

#include <condition_variable>
#include <thread>

namespace brutils
{
  enum class state
  {
    not_started,
    running,
    restarted,
    stopped,
    finished
  };

  class simple_timer::impl
  {
    std::unique_ptr<std::thread> _thread;
    std::mutex _mutex;
    std::condition_variable _cond;

    state _state{state::not_started};
    std::function<void()> _cb;
    std::chrono::time_point<std::chrono::steady_clock> _deadline;
    std::chrono::milliseconds _duration{0};
    bool _periodic{false};

    void change_state(state new_state);
    void run();

   public:
    impl() = default;
    ~impl() { stop(); }
    void set_periodic(bool is_periodic);
    void set_callback(std::function<void()> cb);
    void set_duration(std::chrono::milliseconds duration);
    void start();
    void start(std::chrono::milliseconds duration);
    void restart();
    void restart(std::chrono::milliseconds duration);
    void stop();
    [[nodiscard]] bool is_running() const;
    [[nodiscard]] bool has_elapsed() const;
  };

  void simple_timer::impl::set_periodic(bool is_periodic) { _periodic = is_periodic; }
  void simple_timer::impl::set_callback(std::function<void()> cb) { _cb = std::move(cb); }
  void simple_timer::impl::set_duration(std::chrono::milliseconds duration) { _duration = duration; }

  void simple_timer::impl::start()
  {
    if (is_running()) return;

    _deadline = std::chrono::steady_clock::now() + _duration;
    _thread = std::make_unique<std::thread>(&simple_timer::impl::run, this);
    _thread->detach();
  }

  void simple_timer::impl::start(std::chrono::milliseconds duration)
  {
    if (is_running()) return;

    _duration = duration;
    start();
  }

  void simple_timer::impl::restart()
  {
    if (!is_running()) return;

    _deadline = std::chrono::steady_clock::now() + _duration;
    change_state(state::restarted);
  }

  void simple_timer::impl::restart(std::chrono::milliseconds duration)
  {
    if (!is_running()) return;

    _duration = duration;
    _deadline = std::chrono::steady_clock::now() + _duration;
    change_state(state::restarted);
  }

  void simple_timer::impl::stop()
  {
    _periodic = false;
    change_state(state::stopped);
  }

  bool simple_timer::impl::is_running() const { return state::not_started != _state && state::finished != _state; }

  bool simple_timer::impl::has_elapsed() const { return state::finished == _state; }

  void simple_timer::impl::change_state(state new_state)
  {
    std::unique_lock lock(_mutex);
    _state = new_state;
    _cond.notify_one();
  }

  void simple_timer::impl::run()
  {
    _state = state::running;
    do {
      std::unique_lock lock(_mutex);
      _cond.wait_until(lock, _deadline, [this] {
        return (_deadline < std::chrono::steady_clock::now() || state::running != _state);
      });

      if (state::running == _state) {
        if (!_periodic)_state = state::finished;
        if (_cb) _cb();
      }
    } while (_periodic || state::restarted == _state);
  }

  simple_timer::simple_timer() { _impl = std::make_unique<impl>(); }
  simple_timer::~simple_timer() = default;

  void simple_timer::set_periodic(bool is_periodic) { _impl->set_periodic(is_periodic); }
  void simple_timer::set_callback(std::function<void()> cb) { _impl->set_callback(std::move(cb)); }
  void simple_timer::set_duration(std::chrono::milliseconds duration) { _impl->set_duration(duration); }
  void simple_timer::start() { _impl->start(); }
  void simple_timer::start(std::chrono::milliseconds duration) { _impl->start(duration); }
  void simple_timer::restart() { _impl->restart(); }
  void simple_timer::restart(std::chrono::milliseconds duration) { _impl->restart(duration); }
  void simple_timer::stop() { _impl->stop(); }

  bool simple_timer::is_running() const { return _impl->is_running(); }
  bool simple_timer::has_elapsed() const { return _impl->has_elapsed(); }
}
