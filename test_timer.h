/*
 * test_timer.h
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
 * test_timer.h
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#ifndef TEST_TIMER_H_
#define TEST_TIMER_H_

#include "time_counter.h"

#include <cstdio>
#include <iostream>

namespace test {

	void testStopWatch() {
		TimeCounter tc;
		while(true) {
			int x;
			scanf("%d", &x);
			if(x < 0) tc.stopTimer();
			else if(x > 0) tc.startTimer();
			printf("Elapsed time: %lf\tRunning:%d\n", tc.getElapsedTime(),
					tc.isTimeRunning());
		}
	}

}

#endif /* TEST_TIMER_H_ */
