/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Extends the std::chrono time api by some useful features and shortcuts.

#pragma once

#include <chrono>
#include <ratio>

#if __cplusplus >= 201402L
using namespace std::literals::chrono_literals;
#endif //c++14

namespace nytl
{

class TimePoint;

typedef std::chrono::duration<double,std::ratio<1,1000000000>> nanoseconds;
typedef std::chrono::duration<double,std::ratio<1,1000000>> microseconds;
typedef std::chrono::duration<double,std::ratio<1,1000>> milliseconds;
typedef std::chrono::duration<double,std::ratio<1,1>> seconds;
typedef std::chrono::duration<double,std::ratio<60,1>> minutes;
typedef std::chrono::duration<double,std::ratio<3600,1>> hours;

///\ingroup utility
///Dervied from the std duration class but offers more operations and built-in conversions.
class TimeDuration : public std::chrono::high_resolution_clock::duration
{
protected:
    typedef std::chrono::high_resolution_clock::duration stdDuration;

public:
    TimeDuration() = default;
    TimeDuration(const stdDuration& other) : stdDuration(other) {}
    template<class T, class R> TimeDuration(const std::chrono::duration<T, R>& other) 
		: stdDuration(std::chrono::duration_cast<stdDuration>(other)) {}

    TimeDuration& operator=(const stdDuration& other)
		{ stdDuration::operator=(other); return *this; }
    template<class T, class R> TimeDuration& operator=(const std::chrono::duration<T, R>& other)
		{ stdDuration::operator=(other); return *this; }

    double nanoseconds() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<1,1000000000>>>(*this).count(); }
    double microseconds() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<1,1000000>>>(*this).count(); }
    double milliseconds() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<1,1000>>>(*this).count(); }
    double seconds() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<1,1>>>(*this).count(); }
    double minutes() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<60,1>>>(*this).count(); }
    double hours() const 
	{ return std::chrono::duration_cast<duration<double,std::ratio<3600,1>>>(*this).count(); }

    TimePoint then() const;
};

///\ingroup utility
///Dervied from the std time_point class but offers more operations and built-in conversions.
class TimePoint : public std::chrono::high_resolution_clock::time_point
{
protected:
    typedef std::chrono::high_resolution_clock::time_point stdPoint;
public:
    TimePoint() : stdPoint(std::chrono::high_resolution_clock::now()) {};
    TimePoint(const TimeDuration& d) : stdPoint(std::chrono::high_resolution_clock::now() + d) {};
    TimePoint(const stdPoint& other) : stdPoint(other) {}
    template<class T, class R> TimePoint(const std::chrono::time_point<T, R>& other) 
		: stdPoint(std::chrono::time_point_cast<stdPoint>(other)) {}

    TimePoint& operator=(const stdPoint& other){ stdPoint::operator=(other); return *this; }
    template<class T, class R> TimePoint& operator=(const std::chrono::time_point<T, R>& other)
		{ stdPoint::operator=(other); return *this; }

    TimeDuration fromNow() const { return *this - TimePoint(); }
    bool inFuture() const { return TimeDuration(*this - TimePoint()).nanoseconds() > 0; }

    void fromNow(const TimeDuration& d)
		{ stdPoint::operator=(std::chrono::high_resolution_clock::now() + d); }
};

inline TimePoint now(){ return TimePoint(); }
inline TimePoint TimeDuration::then() const { return TimePoint(*this); }

///\ingroup utility
///Offers functionality to track the elapsed time since a set time point.
class Timer
{
public:
    TimePoint point;

public:
    void reset(){ point = now(); };
    TimeDuration elapsedTime() const { return now() - point; };
};

}
