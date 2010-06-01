/*
 * time_counter.h
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
 * time_counter.h
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#ifndef TIME_COUNTER_H_
#define TIME_COUNTER_H_

#include <ctime>
#include <unistd.h>
#include <sys/time.h>


class TimeCounter {
private:
	timeval start_time_;
	timeval end_time_;
	bool time_running_;
public:
	TimeCounter();
	bool startTimer();
	bool stopTimer();
	bool isTimeRunning() const;
	double getElapsedTime() const;
protected:
	double getDifference(const timeval& from, const timeval& to) const;
};

#endif /* TIME_COUNTER_H_ */
