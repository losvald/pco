/*
 * test_constraints.h
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
 * test_constraints.h
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#ifndef TEST_CONSTRAINTS_H_
#define TEST_CONSTRAINTS_H_

#include "utils.h"
#include "test_entity_io.h"
#include "constraint.h"
#include "purchasable.h"
#include "purchasable_data.h"
#include "string_utils.h"

#include <vector>
#include <string>
using namespace std;

namespace test {

	void filterName() {

		struct ConsGE : public Constraint<Purchasable> {
			int len_;
			void set_len(int len) { len_ = len; }
			bool isMet(const Purchasable& purchasable) const {
				return (int)purchasable.name().size() >= len_;
			}
		} nameLenGE;
		struct ConsLE : public Constraint<Purchasable> {
			int len_;
			void set_len(int len) { len_ = len; }
			bool isMet(const Purchasable& purchasable) const {
				return (int)purchasable.name().size() <= len_;
			}
		} nameLenLE;

		ComplexConstraint<Purchasable> nameLenBetween(nameLenGE, nameLenLE, AND);
		ComplexConstraint<Purchasable> nameLenLess(nameLenGE, NOT);

		while(true) {
			int ge, le;
			printf("Input name len >= : ");
			scanf("%d", &ge);
			printf("Input name len <= : ");
			scanf("%d", &le);

			nameLenGE.set_len(ge);
			nameLenLE.set_len(le);

			vector<const Purchasable*> v = PurchasableData
					::instance().getPurchasables();

			PurchasableData ge_data, le_data, less_data, between_data;
			FORC(it, v) {
				const Purchasable& p = **it;
				if(nameLenGE.isMet(p))
					ge_data.addPurchasable(p);
				if(nameLenLE.isMet(p))
					le_data.addPurchasable(p);
				if(nameLenBetween.isMet(p))
					between_data.addPurchasable(p);
				if(nameLenLess.isMet(p))
					less_data.addPurchasable(p);
			}

			printf("Filtered purchasables with name length >= %d: cnt %d\n",
					ge, ge_data.size());
			EntityIOUtils::printPurchasables(ge_data);
			printf("\n");
			printf("Filtered purchasables with name length <= %d: cnt %d\n",
					le, le_data.size());
			EntityIOUtils::printPurchasables(le_data);

			printf("Filtered purchasables with name length < %d: cnt %d\n",
								ge, less_data.size());
						EntityIOUtils::printPurchasables(less_data);

			printf("\n");
			printf("Filtered purchasables with name length [%d, %d]: cnt %d\n",
								ge, le, between_data.size());
			EntityIOUtils::printPurchasables(between_data);
		}

	}

	vector<int> filterInts(const vector<int>& v, const Constraint<int>& cons) {
		vector<int> ret;
		FORC(it, v)
			if(cons.isMet(*it))
				ret.push_back(*it);
		return ret;
	}

	void testIntConstraints() {
		vector<int> v;
		for(int i = 0; i < 30; ++i)
			v.push_back(i);
		struct PrimeCons : public Constraint<int> {
			bool isMet(const int& x) const {
				for(int i = 2; i*i <= x; ++i)
					if(x%i == 0)
						return false;
				return true;
			}
		} primec;
		struct LessThanCons : public Constraint<int> {
			bool isMet(const int& x) const {
				return x < 7;
			}
		} ltc;
		vector<int> lt7 = filterInts(v, ltc);
		cout << "< 7: " << StringUtils::to_string(lt7.begin(), lt7.end()) << endl;

		vector<int> ge7 = filterInts(v, ComplexConstraint<int>(ltc, NOT));
		cout << ">= 7: " << StringUtils::to_string(ge7.begin(), ge7.end()) << endl;

		vector<int> pge7 = filterInts(v, ComplexConstraint<int>(
				ComplexConstraint<int>(ltc, NOT), primec, AND));
		cout << "prime && >= 7: " << StringUtils::to_string(pge7.begin(), pge7.end())
		<< endl;

		vector<int> p_or_lt7 = filterInts(v, ComplexConstraint<int>(
						ltc, primec, OR));
		cout << "prime || < 7: " << StringUtils::to_string(
				p_or_lt7.begin(), p_or_lt7.end()) << endl;

		vector<int> eq3 = filterInts(v, EqualsConstraint<int>(3));
		cout << "== 3: " << StringUtils::to_string(eq3.begin(), eq3.end()) << endl;

		vector<int> between2_7 = filterInts(v, BoundedRangeConstraint<int>(2, 7));
		cout << "in [2, 7): " << StringUtils::to_string(between2_7.begin(),
				between2_7.end()) << endl;
	}
}

#endif /* TEST_CONSTRAINTS_H_ */
