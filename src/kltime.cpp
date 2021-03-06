#include "kltime.hpp"

#include <algorithm>
#include <chrono>
//#include <ctre.hpp>
#include <map>

namespace kl {

static constexpr bool _leap_year(uint32_t year) { // Leap year, for one-based 1-400 interval
  return ((year & 0x03) == 0) && year != 100 && year != 200 && year != 300;
}

static constexpr std::array<uint32_t, 12> MSIZES = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static constexpr std::array<uint32_t, 12> MSIZES_LEAP = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static constexpr const std::array<uint32_t, 12>& _month_sizes(int32_t year) { // year in the interval 1-400.
  return _leap_year(year) ? MSIZES_LEAP : MSIZES;
}

static constexpr std::array<int32_t, 400 * 12 + 1> _calculateDeltaMonths() {
  std::array<int32_t, 400 * 12 + 1> res;
  int32_t totalDays = 0;
  res[0] = 0;
  int32_t index = 1;
  for (int32_t y = 0; y < 400; y++) {
    auto& sizes = _month_sizes(y + 1);
    for (int32_t m = 0; m < 12; m++, index++) {
      totalDays += sizes[m];
      res[index] = totalDays;
    }
  }
  return res;
}

static const std::array<int32_t, 400 * 12 + 1> DeltaMonths = _calculateDeltaMonths();

const DateTime DateTime::UnixEpoch(1970, 1, 1);
const DateTime DateTime::MAX = DateTime::FromTicks(TimeLimits::MAX_TICKS);
const DateTime DateTime::MIN = DateTime::FromTicks(TimeLimits::MIN_TICKS);

Date DateTime::GetDate() const {
  auto d = _ticks / TimeLimits::TICKS_PER_DAY;
  auto fh = std::lldiv(d, TimeLimits::DAYS_IN_400_YEARS);
  auto it = std::lower_bound(DeltaMonths.begin(), DeltaMonths.end(), fh.rem + 1) - 1;
  auto mo = std::div((uint32_t)std::distance(DeltaMonths.begin(), it), 12);

  return Date{.year = (uint32_t)fh.quot * 400 + mo.quot + 1,
              .month = (uint32_t)mo.rem + 1,
              .day = 1 + (uint32_t)fh.rem - *it};
}

DateTime DateTime::now() {
  // auto n = std::chrono::utc_clock::now(); TODO FIX THIS WHEN THE COMPILER IMPLEMENTS utc_clock
  auto n = std::chrono::system_clock::now();
  int64_t nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(n.time_since_epoch()).count();
  return DateTime(nanos / 1'000'000'000, nanos % 1'000'000'000);
}

int64_t DateTime::Ticks() const { return _ticks; }
int32_t DateTime::Days() const { return _ticks / TimeLimits::TICKS_PER_DAY; }
TimeOfDay DateTime::GetTimeOfDay() const {
  int64_t dayTicks = _ticks % TimeLimits::TICKS_PER_DAY;
  auto sec = lldiv(dayTicks, TimeLimits::TICKS_PER_SECOND);
  auto mn = div(sec.quot, 60);
  auto hr = div(mn.quot, 60);
  return {.hour = (uint32_t)hr.quot,
          .min = (uint32_t)hr.rem,
          .sec = (uint32_t)mn.rem,
          .nanos = (uint32_t)sec.rem * 100};
}

DateTime DateTime::FromTicks(int64_t ticks) {
  DateTime dt;
  dt._ticks = ticks < TimeLimits::MIN_TICKS   ? TimeLimits::MIN_TICKS
              : ticks > TimeLimits::MAX_TICKS ? TimeLimits::MAX_TICKS
                                              : ticks;
  return dt;
}

DateTime::DateTime(time_t seconds, int32_t nsec) {
  _ticks = DateTime::UnixEpoch.Ticks() + seconds * TimeLimits::TICKS_PER_SECOND + nsec / 100;
}

DateTime::DateTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec,
                   uint32_t nsec) {
  if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1 || nsec > 999'999'999ULL || sec > 59 ||
      minute > 59 || hour > 23) [[unlikely]] {
    _ticks = 0;
    return;
  }
  year--;
  month--;
  day--;
  // This isn't constexpr in C++20 :( auto years = std::div(year - 1, 400);
  // ldiv_t years{.quot = year / 400, .rem = year % 400};
  auto years = std::div(year, 400);
  auto& monthsizes = _month_sizes(years.rem + 1);
  if (day >= monthsizes[month]) [[unlikely]] {
    _ticks = 0;
    return;
  }
  auto seconds = sec + minute * 60 + hour * 3600;
  auto days = DeltaMonths[years.rem * 12 + month] + day + years.quot * TimeLimits::DAYS_IN_400_YEARS;
  _ticks = nsec / 100;
  _ticks += seconds * TimeLimits::TICKS_PER_SECOND;
  _ticks += days * TimeLimits::TICKS_PER_DAY;
}

TimeSpan DateTime::operator-(DateTime d) { return {.ticks = _ticks - d._ticks}; }
DateTime DateTime::operator-(TimeSpan ts) { return FromTicks(_ticks - ts.ticks); }
DateTime DateTime::operator+(TimeSpan ts) { return FromTicks(_ticks + ts.ticks); }

// auto date_format = ctre::match<
//     R"(([0-3][0-9])/(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)/20([1-9][0-9]):([0-2][0-9]):([0-5][0-9]):([0-5][0-9])
//     ([\+\-])([01][0-9])([0-5][0-9]))">;
// auto date_format = ctre::match<R"(([0-3][0-9]/[A-Z][a-z][a-z]/20[1-9][0-9]))">;
// auto time_format = ctre::match<R"(:([0-2][0-9]:[0-5][0-9]:[0-5][0-9] [\+\-][01][0-9][0-5][0-9]))">;

std::map<std::string_view, uint32_t> months = {{"Jan", 1}, {"Feb", 2},  {"Mar", 3},  {"Apr", 4},
                                               {"May", 5}, {"Jun", 6},  {"Jul", 7},  {"Aug", 8},
                                               {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12}};

// Supported time format: 06/Jul/2022:06:32:41 +0300
std::optional<DateTime> DateTime::Parse(std::string_view dt) {
  if (dt.size() != 26) {
    return std::nullopt;
  }

  if (dt[2] != '/' || dt[6] != '/' || dt[11] != ':' || dt[14] != ':' || dt[17] != ':' || dt[20] != ' ') {
    return std::nullopt;
  }
  if (dt[0] > '3' || dt[0] < '0' || dt[1] > '9' || dt[1] < '0') {
    return std::nullopt;
  }
  uint32_t day = (dt[0] - '0') * 10 + dt[1] - '0';
  auto it = months.find(dt.substr(3, 3));
  if (it == months.end()) {
    return std::nullopt;
  }
  uint32_t month = it->second;

  if (dt[9] > '9' || dt[9] < '0' || dt[10] > '9' || dt[10] < '0') {
    return std::nullopt;
  }
  uint32_t year = 2000 + (dt[9] - '0') * 10 + dt[10] - '0';

  if (dt[12] > '2' || dt[12] < '0' || dt[13] > '9' || dt[13] < '0') {
    return std::nullopt;
  }
  uint32_t hour = (dt[12] - '0') * 10 + dt[13] - '0';

  if (dt[15] > '5' || dt[15] < '0' || dt[16] > '9' || dt[16] < '0') {
    return std::nullopt;
  }
  uint32_t minute = (dt[15] - '0') * 10 + dt[16] - '0';

  if (dt[18] > '5' || dt[18] < '0' || dt[19] > '9' || dt[19] < '0') {
    return std::nullopt;
  }
  uint32_t second = (dt[18] - '0') * 10 + dt[19] - '0';

  DateTime res(year, month, day, hour, minute, second);
  if (res._ticks == 0) {
    return std::nullopt;
  }
  return res;
}

} // namespace kl
