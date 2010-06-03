/*
 * ga_solver.cpp
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
 * ga_solver.cpp
 *
 *  Created on: May 30, 2010
 *      Author: losvald
 */

#include "ga_solver.h"

#include <ecf/ECF.h>

#include "ga_selector.h"
#include "seller.h"
#include "constraint.h"
#include "buyer_request.h"
#include "entity_io_utils.h"
#include "string_utils.h"

#include <vector>
#include <iostream>

using namespace std;


namespace ga {

bool solve(Market& market, const string& requests_filename,
		const std::string& param_file, const std::string& argv0,
		Price price_limit) {

	NoConstraint<Seller> seller_constr;
	NoConstraint<Selection> selection_constr;
	vector<BuyerRequest> reqs;

	if(!EntityIOUtils::loadRequests(reqs, requests_filename)) {
		cerr << "Error loading requests" << endl;
		return false;
	}

	EntityIOUtils::printMarket(market);

	cout << StringUtils::to_string(reqs.begin(), reqs.end(), ",") << endl;

//	BruteForceSelector bfs(market, PurchasableData::instance());
//	if(bfs.start(reqs, seller_constr, selection_constr)) {
//		cout << "Found solution: \n" << endl;
//		cout << bfs.best_selection() << endl;
//	}
//	else {
//		cerr << "Failed to find solution with BF-GA!" << endl;
//	}
//
//	return ;

	GASelector gas(market, PurchasableData::instance(),
			param_file, argv0);
	gas.set_worst_acceptable_price(price_limit);
	if(gas.start(reqs, seller_constr, selection_constr)) {
		cout << "Found solution: \n" << endl;
		cout << gas.best_selection() << endl;
		return true;
	}
	else {
		cerr << "Failed to find solution with GA!" << endl;
	}
	return false;
}


}
