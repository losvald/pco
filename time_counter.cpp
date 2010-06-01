/*
 * time_counter.cpp
 *
 * Copyright (C) 2010 Leo Osvald <leo.osvald@gmail.com>
 *
 * This file is part of Purchasable Cost Optimizer.
 *
 * Purchasable Cost Optimizer is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Purchasable Cost Optimizer is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Purchasable Cost Optimizer. If not, see
 * <http://www.gnu.org/licenses/>.
 */
/*
 * time_counter.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#include "time_counter.h"

TimeCounter::TimeCounter() : time_running_(0) {
}

bool TimeCounter::stopTimer()
{
	if(!isTimeRunning())
		return false;
	gettimeofday(&end_time_, NULL);
	time_running_ = false;
	return true;
}



double TimeCounter::getElapsedTime() const
{
	if(isTimeRunning()) {
		timeval now;
		gettimeofday(&now, NULL);
		return getDifference(start_time_, now);
	}
	return getDifference(start_time_, end_time_);
}

bool TimeCounter::startTimer()
{
	if(isTimeRunning())
		return false;
	gettimeofday(&start_time_, NULL);
	time_running_ = true;
	return true;
}

bool TimeCounter::isTimeRunning() const
{
	return time_running_;
}

#include <iostream>
double TimeCounter::getDifference(const timeval& from, const timeval& to) const
{
	return (to.tv_sec-from.tv_sec)*1000.0
			- (to.tv_usec-from.tv_usec)/1000.0;
}

