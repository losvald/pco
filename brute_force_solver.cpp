/*
 * brute_force_solver.cpp
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
 * brute_force_solver.cpp
 *
 *  Created on: May 27, 2010
 *      Author: losvald
 */

#include "brute_force_solver.h"

#include "brute_force_selector.h"
#include "seller.h"
#include "constraint.h"
#include "buyer_request.h"
#include "entity_io_utils.h"
#include "string_utils.h"
#include "purchasable.h"

#include <vector>
#include <iostream>

using namespace std;

namespace brute_force {

bool solve(Market& market, const std::string& filename) {

	NoConstraint<Seller> seller_constr;
	NoConstraint<Selection> selection_constr;
	vector<BuyerRequest> reqs;

	if(!EntityIOUtils::loadRequests(reqs, filename)) {
		cerr << "Error loading requests" << endl;
	}

	cout << StringUtils::to_string(reqs.begin(), reqs.end(), ",") << endl;

	BruteForceSelector bfs(market, PurchasableData::instance());
	if(bfs.start(reqs, seller_constr, selection_constr)) {
		cout << "Found solution: \n" << endl;
		cout << bfs.best_selection() << endl;
		return true;
	}
	else {
		cerr << "Failed to find solution with brute force!" << endl;
	}

	return false;
}

}
