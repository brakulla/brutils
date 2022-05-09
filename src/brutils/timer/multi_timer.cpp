//
// Created by burak on 5/9/2022.
//

#include "multi_timer.h"

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <set>
#include <thread>

namespace brutils
{
  constexpr bool operator<(multi_timer::record const& lhs, long const rhs_id)
  {
    return lhs.id < rhs_id;
  }
  constexpr bool operator<(long const lhs_id, multi_timer::record const& rhs)
  {
    return lhs_id < rhs.id;
  }
  constexpr bool operator<(multi_timer::record const& lhs, multi_timer::record const& rhs)
  {
    return lhs.end < rhs.end;
  }

  class multi_timer::impl
  {
    std::condition_variable _cond_var;
    std::mutex _mutex;
    std::unique_ptr<std::thread> _thread;
    void run();

    std::atomic_bool _flagged{false};
    std::atomic_bool _running{false};

    std::function<void(long)> _default_cb;
    long _last_id{0};
    std::set<multi_timer::record, std::less<>> _time_records;
    std::map<long, std::function<void()>> _callback_map;

    long _first_el_id{-1};
    void add_new_record(multi_timer::record const& record);
    void flag_change_if_necessary();
    void timer_elapsed(multi_timer::record const& elapsed_record);

   public:
    void start();
    void stop();
    void set_default_callback(std::function<void(long)> callback);
    record add_timer(std::chrono::milliseconds duration);
    record add_timer(std::chrono::milliseconds duration, bool periodic);
    record add_timer(std::chrono::milliseconds duration, std::function<void()> callback);
    record add_timer(std::chrono::milliseconds duration, bool periodic, std::function<void()> callback);
    record add_timer(std::chrono::system_clock::time_point end_time);
    record add_timer(std::chrono::system_clock::time_point end_time, std::function<void()> callback);
    void cancel_timer(long id);
  };

  void multi_timer::impl::start()
  {
    _thread = std::make_unique<std::thread>(&multi_timer::impl::run, this);
  }

  void multi_timer::impl::stop()
  {
    _running = false;
    _flagged = true;
    std::unique_lock lock(_mutex);
    _cond_var.notify_one();

    _thread->join();
    _thread.reset();
  }

  void multi_timer::impl::set_default_callback(std::function<void(long)> callback)
  {
    _default_cb = std::move(callback);
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::milliseconds duration)
  {
    // no need to block run from running since it's a single producer single consumer problem
    auto now = std::chrono::system_clock::now();
    multi_timer::record
      new_record{.id = _last_id++, .periodic = false, .duration = duration, .start = now, .end = now + duration};
    add_new_record(new_record);
    return new_record;
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::milliseconds duration, bool periodic)
  {
    auto now = std::chrono::system_clock::now();
    multi_timer::record
      new_record{.id = _last_id++, .periodic = periodic, .duration = duration, .start = now, .end = now + duration};
    add_new_record(new_record);
    return new_record;
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::milliseconds duration, std::function<void()> callback)
  {
    auto now = std::chrono::system_clock::now();
    multi_timer::record
      new_record{.id = _last_id++, .periodic = false, .duration = duration, .start = now, .end = now + duration};
    _callback_map[new_record.id] = std::move(callback);
    add_new_record(new_record);
    return new_record;
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::milliseconds duration,
                                                   bool periodic,
                                                   std::function<void()> callback)
  {
    auto now = std::chrono::system_clock::now();
    multi_timer::record
      new_record{.id = _last_id++, .periodic = periodic, .duration = duration, .start = now, .end = now + duration};
    _callback_map[new_record.id] = std::move(callback);
    add_new_record(new_record);
    return new_record;
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::system_clock::time_point end_time)
  {
    multi_timer::record new_record
      {.id = _last_id++, .periodic = false, .duration = {}, .start=std::chrono::system_clock::now(), .end = end_time};
    add_new_record(new_record);
    return new_record;
  }

  multi_timer::record multi_timer::impl::add_timer(std::chrono::system_clock::time_point end_time,
                                                   std::function<void()> callback)
  {
    multi_timer::record new_record
      {.id = _last_id++, .periodic = false, .duration = {}, .start=std::chrono::system_clock::now(), .end = end_time};
    _callback_map[new_record.id] = std::move(callback);
    add_new_record(new_record);
    return new_record;
  }

  void multi_timer::impl::cancel_timer(long id)
  {
    // prevent run function from continuing its operation in case the cancelled one is the one in progress
    std::unique_lock lock(_mutex);
    auto it = _time_records.find(id);
    if (_time_records.end() != it) {
      _time_records.erase(it);
      flag_change_if_necessary();
    }
  }

  void multi_timer::impl::run()
  {
    _running = true;
    while (_running) {
      // calculate next deadline
      std::unique_lock lock(_mutex);
      auto closest_record = _time_records.begin(); // set is already ordered based on closest end time
      _cond_var.wait_until(lock, closest_record->end, [this, &closest_record] {
        return (closest_record->end < std::chrono::system_clock::now() || !_flagged);
      });
      if (!_flagged) {
        timer_elapsed(*closest_record);
      }
    }
  }

  void multi_timer::impl::add_new_record(multi_timer::record const& record)
  {
    _time_records.insert(record);
    flag_change_if_necessary();
  }

  void multi_timer::impl::flag_change_if_necessary()
  {
    if (_first_el_id == -1) {
      if (!_time_records.empty()) {
        _flagged = true;
        _first_el_id = _time_records.begin()->id;
      }
    } else {
      if (_time_records.empty()) {
        _flagged = true;
        _first_el_id = -1;
      } else if (_first_el_id != _time_records.begin()->id) {
        _flagged = true;
        _first_el_id = _time_records.begin()->id;
      }
    }

    if (_flagged) {
      _cond_var.notify_one();
    }
  }

  void multi_timer::impl::timer_elapsed(multi_timer::record const& elapsed_record)
  {
    // this function is to be called only in run function, therefore there is no need to flag the changes or lock
    // the lock should already be in lock state

    // erase from the records
    _time_records.erase(elapsed_record);

    // put it back with new end time if periodic
    if (elapsed_record.periodic) {
      auto record = elapsed_record;
      record.start = record.end;
      record.end = record.start + record.duration;
      _time_records.insert(record);
    }

    // trigger callback if set
    auto it = _callback_map.find(elapsed_record.id);
    if (_callback_map.end() != it) it->second();
    else if (_default_cb) _default_cb(elapsed_record.id);
  }

  // ---------------------------------------- //
  multi_timer::multi_timer() { _impl = std::make_unique<impl>(); }
  multi_timer::~multi_timer() = default;
  void multi_timer::start() { _impl->start(); }
  void multi_timer::stop() { _impl->stop(); }
  void multi_timer::set_default_callback(std::function<void(long)> callback)
  {
    _impl->set_default_callback(std::move(callback));
  }
  multi_timer::record multi_timer::add_timer(std::chrono::milliseconds duration)
  {
    return _impl->add_timer(duration);
  }
  multi_timer::record multi_timer::add_timer(std::chrono::milliseconds duration,
                                             std::function<void()> callback)
  {
    return _impl->add_timer(duration, std::move(callback));
  }
  multi_timer::record multi_timer::add_timer(std::chrono::milliseconds duration, bool periodic)
  {
    return _impl->add_timer(duration, periodic);
  }
  multi_timer::record multi_timer::add_timer(std::chrono::milliseconds duration,
                                             bool periodic,
                                             std::function<void()> callback)
  {
    return _impl->add_timer(duration, periodic, std::move(callback));
  }
  multi_timer::record multi_timer::add_timer(std::chrono::system_clock::time_point end_time)
  {
    return _impl->add_timer(end_time);
  }
  multi_timer::record multi_timer::add_timer(std::chrono::system_clock::time_point end_time,
                                             std::function<void()> callback)
  {
    return _impl->add_timer(end_time, std::move(callback));
  }
  void multi_timer::cancel_timer(long id) { _impl->cancel_timer(id); }

} // brutils