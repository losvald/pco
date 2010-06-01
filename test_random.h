/*
 * test_random.h
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
 * test_random.h
 *
 *  Created on: Apr 22, 2010
 *      Author: losvald
 */

#ifndef TEST_RANDOM_H_
#define TEST_RANDOM_H_

#include "random_utils.h"

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

namespace test {

	template<typename T>
		void testRandom() {
			unsigned seed;
			printf("Seed: ");
			scanf("%u", &seed);
			T a, b;
	//		printf("Input a and b:\n");
			cin >> a >> b;
			while(true) {
				scanf("%*c");
				T r = RandomUtils::RandomUtils::random(make_pair<T, T>(a, b), &seed);
				cout << "> " << r << " = " << StringUtils::to_bitstring(r)
				<< endl;
			}
		}

	void testDiscreteRangeRandomVar() {
		int a, b;
		cout << "Input range: ";
		cin >> a >> b;

		struct Var : virtual public RandomUtils::DiscreteRangeRandomVariable<int> {
			Var(int from, int to) : RandomUtils::DiscreteRangeRandomVariable<int>(from, to) {
				setRange(from, to);
			}

			double getProbability(int v) const {
				if(v == from())
					return 0.4;
				if(v == to()-1)
					return 0.4;
				return (1-2*0.4) / (to()-from()-2);
			}
		} var(a, b);

		var.clear();
		var.setRange(a+1, b-1);

		vector< pair<int, double> > dist = var.getDistribution();
			FORC(it, dist) {
				cout << it->first << "\t" << it->second << endl;
			}

		while(true) {
			scanf("%*c");
			cout << "> " << var.get() << endl;
		}
	}



	void testDiscreteVariable() {
		RandomUtils::DiscreteRandomVariable<int> var(-1);
		double prob[] = {0.5, 0.25, 0.25};
		int n = sizeof(prob)/sizeof(double);
		for(int i = 0; i < n; ++i)
			var.add(i, prob[i]);
		vector< pair<int, double> > d = var.getDistribution();
		for(int i = 0; i < n; ++i)
			printf("%9d ", d[i].first);
		printf("\n");
		for(int i = 0; i < n; ++i)
			printf("%9.6lf ", d[i].second);
		printf("\n");
		for(int i = 0; i < n; ++i)
			printf("%9.6lf ", var.getProbability(i));
		printf("\n");
		while(true) {
			scanf("%*c");
			printf(">%d\n", var.get());
		}
	}

}

#endif /* TEST_RANDOM_H_ */
