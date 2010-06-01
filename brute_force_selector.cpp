/*
 * brute_force_selector.cpp
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
 * brute_force_selector.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#include "brute_force_selector.h"

using namespace std;

namespace bf_selector {
static const char* MSG_TAG = "[BF-SEL] ";
}

BruteForceSelector::BruteForceSelector(Market& market,
		PurchasableData& purchasable_data)
: FlowBasedSelector(market, purchasable_data) {
}

bool BruteForceSelector::recurse(const vector<const PurchasableOffer*>& offers,
		int n) {
	bool found = false;

	if(isCurrentSelectionFeasible()) {
//		cout << bf_selector::MSG_TAG << " recurse: " << endl;
//		cout << selection_ << endl;
		//	cout << "Feasible? " << isCurrentSelectionFeasible() << endl;
//		scanf("%*c");
		found = true;
		updateBestSelection(selection_);
	}
	if(n >= (int)offers.size())
		return found;

	for(int q = 0; ; ++q) {
		int cnt = selectOffer(*offers[n], q);
		if(cnt < q) {
			deselectOffer(*offers[n], cnt);
			break;
		}
		found |= recurse(offers, n+1);
		deselectOffer(*offers[n], cnt);
	}

	return found;
}

bool BruteForceSelector::start(
		const std::vector<BuyerRequest>& purchasable_requests,
		const Constraint<Seller>& seller_constraint,
		const Constraint<Selection>& selection_constraint) {

	initStart(purchasable_requests, seller_constraint, selection_constraint);

	EntityIOUtils::printMarket(market_);
	cout << selection_ << endl;
	printGraph();

	vector<const PurchasableOffer*> v = market_.getAllOffers();

//	map<Id, const PurchasableOffer*> offer;
//	FORC(it, v) {
//		offer[(*it)->id()] = *it;
//	}

	return recurse(v, 0);
}

bool BruteForceSelector::stop() {
	return false;
}
