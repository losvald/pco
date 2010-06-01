/*
 * greedy_solver.cpp
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
 * greedy_solver.cpp
 *
 *  Created on: May 31, 2010
 *      Author: losvald
 */

#include "greedy_solver.h"

#include "greedy_selector.h"
#include "seller.h"
#include "constraint.h"
#include "buyer_request.h"
#include "entity_io_utils.h"
#include "string_utils.h"
#include "flow_based_greedy_selector.h"

#include <iostream>
#include <vector>

using namespace std;

namespace greedy {

void solve2(Market& market, const std::string& filename) {
	NoConstraint<Seller> seller_constr;
	NoConstraint<Selection> selection_constr;
	vector<BuyerRequest> reqs;

	if(!EntityIOUtils::loadRequests(reqs, filename)) {
		cerr << "Error loading requests" << endl;
	}

	cout << StringUtils::to_string(reqs.begin(), reqs.end(), ",") << endl;

	FlowBasedGreedySelector fbgs(market, PurchasableData::instance());
	if(!fbgs.start(reqs, seller_constr, selection_constr)) {
		cerr << "Failed to find solution with flow-based greedy" << endl;
	}
//	cout << gs.selection() << endl;
}

void solve(Market& market, const std::string& filename) {
	NoConstraint<Seller> seller_constr;
	NoConstraint<Selection> selection_constr;
	vector<BuyerRequest> reqs;

	if(!EntityIOUtils::loadRequests(reqs, filename)) {
		cerr << "Error loading requests" << endl;
	}

	cout << StringUtils::to_string(reqs.begin(), reqs.end(), ",") << endl;

	GreedySelector gs(market, PurchasableData::instance());
	if(!gs.start(reqs, seller_constr, selection_constr)) {
		cerr << "Failed to find solution with greedy" << endl;
	}
//	cout << gs.selection() << endl;
}

}
