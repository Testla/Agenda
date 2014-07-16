#include "Date.h"

Date::Date() {
    year_ = 0;
    month_ = 0;
    day_ = 0;
    hour_ = 0;
    minute_ = 0;
}

Date::Date(int y, int m, int d, int h, int mi) : year_(y),
                                                 month_(m),
                                                 day_(d),
                                                 hour_(h),
                                                 minute_(mi) {}

int Date::getYear(void) const {
    return year_;
}     

void Date::setYear(int year) {
    year_ = year;
}

int Date::getMonth(void) const {
    return month_;
}

void Date::setMonth(int month) {
    month_ = month;
}

int Date::getDay(void) const {
    return day_;
}

void Date::setDay(int day) {
    day_ = day;
}

int Date::getHour(void) const {
    return hour_;
}

void Date::setHour(int hour) {
    hour_ = hour;
}

int Date::getMinute(void) const {
    return minute_;
}

void Date::setMinute(int minute) {
    minute_ = minute;
}

bool Date::isValid(Date date) {
    static const int leap_nday[12] = {31, 29, 31, 30, 31, 30, \
                                      31, 31, 30, 31, 30, 31};
    static const int nday[12] = {31, 28, 31, 30, 31, 30, \
                                 31, 31, 30, 31, 30, 31};
    if (date.year_ < 1000 || date.year_ > 9999
        || date.month_ <= 0 || date.month_ > 12
        || date.day_ <= 0
        || date.day_ > ((date.year_ % 400 == 0
                         || (date.year_ % 4 == 0 && date.year_ % 100 != 0))
                            ? leap_nday[date.month_ - 1]
                            : nday[date.month_ - 1])
        || date.hour_ < 0 || date.hour_ >= 24
        || date.minute_ < 0 || date.minute_ >= 60) {
        return false;
    }
    return true;
}

Date Date::stringToDate(std::string dateString) {
    Date result;
    result.setYear(1000 * (dateString[0] - '0')
                   + 100 * (dateString[1] - '0')
                   + 10 * (dateString[2] - '0')
                   + 1 * (dateString[3] - '0'));
    result.setMonth(10 * (dateString[5] - '0') + 1 * (dateString[6] - '0'));
    result.setDay(10 * (dateString[8] - '0') + 1 * (dateString[9] - '0'));
    result.setHour(10 * (dateString[11] - '0') + 1 * (dateString[12] - '0'));
    result.setMinute(10 * (dateString[14] - '0') + 1 * (dateString[15] - '0'));
    return result;
}

std::string Date::dateToString(Date date) {
    std::string result = "";
    result += date.year_ / 1000 + '0';
    result += date.year_ % 1000 / 100 + '0';
    result += date.year_ % 100 / 10 + '0';
    result += date.year_ % 10 + '0';
    result += '-';
    result += date.month_ / 10 + '0';
    result += date.month_ % 10 + '0';
    result += '-';
    result += date.day_ / 10 + '0';
    result += date.day_ % 10 + '0';
    result += '/';
    result += date.hour_ / 10 + '0';
    result += date.hour_ % 10 + '0';
    result += ':';
    result += date.minute_ / 10 + '0';
    result += date.minute_ % 10 + '0';
    return result;
}

Date& Date::operator=(const Date& date) {
    year_ = date.year_;
    month_ = date.month_;
    day_ = date.day_;
    hour_ = date.hour_;
    minute_ = date.minute_;
    return *this;
}

bool Date::operator==(const Date& date) const {
    return (year_ == date.year_
            && month_ == date.month_
            && day_ == date.day_
            && hour_ == date.hour_
            && minute_ == date.minute_);
}

bool Date::operator>(const Date& date) const {
    if (year_ > date.year_)
        return true;
    else if (year_ == date.year_) {
        if (month_ > date.month_)
            return true;
        else if (month_ == date.month_) {
            if (day_ > date.day_)
                return true;
            else if (day_ == date.day_) {
                if (hour_ > date.hour_)
                    return true;
                else if (hour_ == date.hour_) {
                    if (minute_ > date.minute_)
                        return true;
                }
            }
        }
    }
    return false;
}

bool Date::operator<(const Date& date) const {
    if (year_ < date.year_)
        return true;
    else if (year_ == date.year_) {
        if (month_ < date.month_)
            return true;
        else if (month_ == date.month_) {
            if (day_ < date.day_)
                return true;
            else if (day_ == date.day_) {
                if (hour_ < date.hour_)
                    return true;
                else if (hour_ == date.hour_) {
                    if (minute_ < date.minute_)
                        return true;
                }
            }
        }
    }
    return false;
}

bool Date::operator>=(const Date& date) const {
    return (*this > date) || (*this == date);
}

bool Date::operator<=(const Date& date) const {
    return (*this < date) || (*this == date);
}

