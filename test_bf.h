/*
 * test_bf.h
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
 * test_bf.h
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#ifndef TEST_BF_H_
#define TEST_BF_H_

#include "brute_force_selector.h"
#include "entity_io_utils.h"
#include "buyer_request.h"
#include "string_utils.h"
#include "purchasable.h"

#include <vector>
#include <iostream>

using namespace std;

namespace test {

void testBF(Market& market, const string& filename) {

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
	}
	else {
		cerr << "Failed to find solution with brute force!" << endl;
	}

}

}

#endif /* TEST_BF_H_ */
