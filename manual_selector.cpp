/*
 * manual_selector.cpp
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
 * manual_selector.cpp
 *
 *  Created on: May 29, 2010
 *      Author: losvald
 */

#include "manual_selector.h"

using namespace std;

namespace manual_selector {
static const char* MSG_TAG = "[BF-SEL] ";
}

ManualSelector::ManualSelector(Market& market, PurchasableData& purchasable_data)
: FlowBasedSelector(market, purchasable_data) {
	// TODO Auto-generated constructor stub

}

ManualSelector::~ManualSelector() {
	// TODO Auto-generated destructor stub
}

bool ManualSelector::start(
		const std::vector<BuyerRequest>& purchasable_requests,
		const Constraint<Seller>& seller_constraint,
		const Constraint<Selection>& selection_constraint) {

	initStart(purchasable_requests, seller_constraint, selection_constraint);

	EntityIOUtils::printMarket(market_);
	cout << selection_ << endl;
	printGraph();

	vector<const PurchasableOffer*> v = market_.getAllOffers();
	map<Id, const PurchasableOffer*> offer;
	FORC(it, v) {
		offer[(*it)->id()] = *it;
		cout << "Offer for man. sel: " << endl;
		cout << **it;
	}

	for(int x, q; ;) {
		printf("(offer id) (quantity to buy/return): \n");
		scanf("%d%d", &x, &q);
		if(q == 0) {
			cout << "Clearing selection...";
			clearSelection();
			cout << "DONE clearing selection!" << endl;
		}
		else {
			//Id offer_id = offer[x]->id();
			//Seller* seller = offer[x]->seller();
			cout << "Manual selecting: " << endl;
			cout << *offer[x];
			if(q > 0) {
				//buy
				Quantity times = selectOffer(*offer[x], q);
				printf("%sBought %d times.\n", manual_selector::MSG_TAG, times);
			}
			else {
				//return
				Quantity times = deselectOffer(*offer[x], -q);
				printf("%sReturned %d times.\n", manual_selector::MSG_TAG, times);
			}
		}
		cout << selection_ << endl;
		printGraph();
		cout << "Total disj. met: " << calcMaxFlow()
				<< "/" << total_disj_req_quantity_ << endl;
		cout << "All disj. met: " << disjunctiveRequestsMet() << endl;
		cout << "All non-disj. met: " << nonDisjunctiveRequestsMet() << endl;
		cout << "Feasible? " << isCurrentSelectionFeasible() << endl;
	}

	return true;
}

bool ManualSelector::stop() {
	return false;
}


#include "entity_io_utils.h"
#include "buyer_request.h"
#include "string_utils.h"
#include "purchasable.h"

#include <vector>
#include <iostream>

namespace test {

void testManualSelector(Market& market, const string& filename) {

	NoConstraint<Seller> seller_constr;
	NoConstraint<Selection> selection_constr;
	vector<BuyerRequest> reqs;

	if(!EntityIOUtils::loadRequests(reqs, filename)) {
		cerr << "Error loading requests" << endl;
	}

	cout << StringUtils::to_string(reqs.begin(), reqs.end(), ",") << endl;

	ManualSelector ms(market, PurchasableData::instance());
	if(!ms.start(reqs, seller_constr, selection_constr)) {
		cerr << "Failed to find solution with manual selector!" << endl;
	}

}

}
