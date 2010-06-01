/*
 * test_selection.h
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
 * test_selection.h
 *
 *  Created on: May 24, 2010
 *      Author: losvald
 */

#ifndef TEST_SELECTION_H_
#define TEST_SELECTION_H_

#include "utils.h"
#include "base_types.h"
#include "selector.h"
#include "selection.h"
#include "market.h"
#include "entity_io_utils.h"

#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>

using namespace std;

namespace test {

namespace test_selection {
	static const char* MSG_TAG = "[TEST-ADD-RETURN] ";
}

typedef vector< pair<const PurchasableOffer*, Quantity> >_add_and_ret_vector;

void addAndReturnOffers(Market& market, Selection& selection,
		_add_and_ret_vector& v) {

	cout << MSG_TAG << "MARKET BEFORE: " << endl;
	EntityIOUtils::printMarket(market);

	FORC(it, v) {
		printf("Press any key to add and return: %dx %d\n",
				it->second, it->first->id());
		scanf("%*c");
		//buy
		Quantity q = it->first->seller()->buyOffer(it->first->id(), it->second);
		printf("%sBought %d times.\n", test_selection::MSG_TAG, q);
		cout << selection << endl;
		EntityIOUtils::printMarket(market);
		//return
		q = it->first->seller()->returnOffer(it->first->id(), it->second);
		printf("%sReturned %d times.\n", test_selection::MSG_TAG, q);
		cout << selection << endl;
		EntityIOUtils::printMarket(market);
	}

	cout << MSG_TAG << "MARKET AFTER: " << endl;
	EntityIOUtils::printMarket(market);
}

void manualSelectionTest(Market& market) {
	cout << MSG_TAG << "MARKET BEFORE: " << endl;
	EntityIOUtils::printMarket(market);

	Selection selection;


	vector<const PurchasableOffer*> v = market.getAllOffers();
	map<Id, const PurchasableOffer*> offer;
	FORC(it, v) {
		offer[(*it)->id()] = *it;
	}

	for(int x, q; ;) {
		printf("(offer id) (quantity to buy/return): \n");
		scanf("%d%d", &x, &q);
		Id offer_id = offer[x]->id();
		Seller* seller = offer[x]->seller();
		if(q > 0) {
			//buy
			Quantity times = seller->buyOffer(offer_id, q);
			selection.addOffer(*offer[x], times);
			printf("%sBought %d times.\n", test_selection::MSG_TAG, times);
		}
		else {
			//return
			Quantity times = seller->returnOffer(offer_id, -q);
			selection.removeOffer(*offer[x], times);
			printf("%sReturned %d times.\n", test_selection::MSG_TAG, times);
		}
		EntityIOUtils::printMarket(market);
		cout << selection << endl;
	}

	cout << MSG_TAG << "MARKET AFTER: " << endl;
	EntityIOUtils::printMarket(market);
}

void defaultSelectionTest(Market& market) {
	Selection sel;
	vector<const PurchasableOffer*> offers = market.getAllOffers();
	_add_and_ret_vector v;
	int cnt = 0;
	FORC(it, offers) {
		v.push_back(make_pair(*it, 1+cnt));
		++cnt;
	}
	addAndReturnOffers(market, sel, v);
}


}


#endif /* TEST_SELECTION_H_ */
