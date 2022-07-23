#pragma once
#include <array>
#include <compare>
#include <cstdint>
#include <sys/time.h>
#include <string>

namespace kl {

struct TimeOfDay {
  uint32_t hour;
  uint32_t min;
  uint32_t sec;
  uint32_t nanos;
};

struct Date {
  uint32_t year;
  uint32_t month;
  uint32_t day;
};

struct TimeLimits {
  static constexpr int64_t MIN_TICKS = 0LL;                   // 0001-01-01 00:00:00 UTC (if that makes any sense)
  static constexpr int64_t MAX_TICKS = 3155378975999999999LL; // 9999-12-31 23:59:59 UTC
  static constexpr int64_t TICKS_PER_SECOND = 10'000'000LL;
  static constexpr int64_t TICKS_PER_MILLISECOND = 10'000LL;
  static constexpr int64_t TICKS_PER_MICROSECOND = 10LL;
  static constexpr int64_t TICKS_PER_DAY = TICKS_PER_SECOND * 24 * 3600;
  static constexpr uint32_t DAYS_IN_400_YEARS = 365 * 400 + 97;
};

// Interface inspired after the C# Timespan
struct TimeSpan {
  int64_t ticks;

  static constexpr TimeSpan FromHours(int64_t h) { return {.ticks = h * 3600 * TimeLimits::TICKS_PER_SECOND}; }
  static constexpr TimeSpan FromMinutes(int64_t m) { return {.ticks = m * 60 * TimeLimits::TICKS_PER_SECOND}; }
  static constexpr TimeSpan FromSeconds(int64_t s) { return {.ticks = s * TimeLimits::TICKS_PER_SECOND}; }
  static constexpr TimeSpan FromDays(int64_t d) { return {.ticks = d * TimeLimits::TICKS_PER_DAY}; }
  static constexpr TimeSpan FromNanos(int64_t d) { return {.ticks = d / 100}; }
  static constexpr TimeSpan FromTimeval(struct timeval tv) {
    return {.ticks = (int64_t)tv.tv_sec * TimeLimits::TICKS_PER_SECOND +
                     (int64_t)tv.tv_usec * TimeLimits::TICKS_PER_MICROSECOND};
  }

  constexpr int64_t TotalHours() const { return ticks / (TimeLimits::TICKS_PER_SECOND * 3600); }
  constexpr int64_t TotalMinutes() const { return ticks / (TimeLimits::TICKS_PER_SECOND * 60); }
  constexpr int64_t TotalSeconds() const { return ticks / TimeLimits::TICKS_PER_SECOND; }
  constexpr int64_t TotalMilliseconds() const { return ticks / (TimeLimits::TICKS_PER_SECOND / 1000); }
  constexpr int64_t Hours() const { return (ticks / (TimeLimits::TICKS_PER_SECOND * 3600)) % 24; }
  constexpr int64_t Minutes() const { return (ticks / (TimeLimits::TICKS_PER_SECOND * 60)) % 60; }
  constexpr int64_t Seconds() const { return (ticks / TimeLimits::TICKS_PER_SECOND) % 60; }
  constexpr int64_t Days() const { return ticks / TimeLimits::TICKS_PER_DAY; }
  constexpr int64_t Milliseconds() const { return (ticks / (TimeLimits::TICKS_PER_MILLISECOND)) % 1000; }
  constexpr struct timeval Timeval() const {
    return {.tv_sec = TotalSeconds(),
            .tv_usec = (ticks % TimeLimits::TICKS_PER_SECOND) / TimeLimits::TICKS_PER_MICROSECOND};
  }

  auto operator<=>(const TimeSpan&) const = default;
  constexpr TimeSpan operator-(const TimeSpan& ts) const { return {.ticks = ticks - ts.ticks}; }
  constexpr TimeSpan operator+(const TimeSpan& ts) const { return {.ticks = ticks + ts.ticks}; }
};

// Interface inspired after the C# Datetime
class DateTime {
  // One Tick represents 100 nanoseconds.
  int64_t _ticks = TimeLimits::MIN_TICKS;

public:
  DateTime() = default;
  DateTime(time_t seconds, int32_t nsec = 0);
  DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour = 0, uint32_t minute = 0, uint32_t sec = 0,
           uint32_t nsec = 0);
  DateTime(const DateTime&) = default;

public:
  int64_t Ticks() const;
  int32_t Days() const;
  TimeOfDay GetTimeOfDay() const;
  Date GetDate() const;
  static DateTime FromTicks(int64_t ticks);
  static DateTime Parse(const std::string& src);

public:
  DateTime& operator=(const DateTime& d) = default;
  TimeSpan operator-(DateTime d);
  DateTime operator-(TimeSpan ts);
  DateTime operator+(TimeSpan ts);
  std::strong_ordering operator<=>(const DateTime& target) const = default;

  static const DateTime UnixEpoch;
  static const DateTime MAX;
  static const DateTime MIN;
  static DateTime now();
};

} // namespace kl
