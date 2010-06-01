/*
 * test_requests.h
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
 * test_requests.h
 *
 *  Created on: May 28, 2010
 *      Author: losvald
 */

#ifndef TEST_REQUESTS_H_
#define TEST_REQUESTS_H_

#include "entity_io_utils.h"
#include "seller.h"

namespace test {

void testRequests(Market& market, const string& filename) {

	vector<BuyerRequest> reqs;
	if(!EntityIOUtils::loadRequests(reqs, filename)) {
		cerr << "Error loading requests" << endl;
	}

	vector<Seller*> sellers = market.sellers();

	cout << "Requests: \n" << StringUtils::to_string(reqs.begin(), reqs.end(), "\n");
	FORC(it, reqs) {
		cout << "q(" << *it << ") = " << it->quantity() << endl;
	}

	FORC(s, sellers) {
		vector<const PurchasableOffer*> offers = (*s)->allOffers();
		FORC(o, offers) {
			FORC(r, reqs) {
				cout << "Offer " << (*o)->name()
								<< " matches " << r->name()
								<< ": " << (*o)->meetsConstraint(r->constraint())
								<< endl;
			}
		}
	}

}

}

#endif /* TEST_REQUESTS_H_ */
