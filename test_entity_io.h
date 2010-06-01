/*
 * test_entity_io.h
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
 * test_entity_io.h
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#ifndef TEST_ENTITY_IO_H_
#define TEST_ENTITY_IO_H_

#include "utils.h"

#include <vector>
#include <iostream>

using namespace std;

namespace test {

static const char* MSG_TAG = "[TEST] ";
	static const string DATA_DIR = "data";

	void printCurrencies() {
		EntityIOUtils::printCurrencies();
	}

	static void printPurchasables() {
		EntityIOUtils::printPurchasables(PurchasableData::instance());
	}
	void savePurchasables() {
		printf("testing output..\n");
		EntityIOUtils::savePurchasables(DATA_DIR+"/foo_purchasables.out",
				PurchasableData::instance());
	}

	void testMarketOLD(Market& market) {
		cout << "Sellers on market: " << endl;
		const vector<const Seller*>& v = market.getSellers();
		FORC(s, v) {
			cout << **s;
		}
		printf("testing output..\n");
		EntityIOUtils::saveMarketOLD(DATA_DIR+"/foo_sellers.out", market);
	}

	void saveSellers(Market& market) {
		cout << MSG_TAG << "saving sellers..." << endl;
		if(!EntityIOUtils::saveSellers(DATA_DIR+"/foo_sellers.txt", market)) {
			cerr << MSG_TAG << "Error saving sellers!" << endl;
		}
	}

	void saveOffers(Market& market) {
		cout << MSG_TAG << "saving offers..." << endl;
		if(!EntityIOUtils::saveOffers(DATA_DIR+"/foo_offers.txt", market)) {
			cerr << MSG_TAG << "Error saving offers!" << endl;
		}
	}

	void saveSupplies(Market& market) {
		cout << MSG_TAG << "saving supplies..." << endl;
		if(!EntityIOUtils::saveSupplies(DATA_DIR+"/foo_supplies.txt", market)) {
			cerr << MSG_TAG << "Error saving supplies!" << endl;
		}
	}

	void testMarket(Market& market) {
		cout << "loading sellers: " << endl;
		EntityIOUtils::loadSellers(market, DATA_DIR+"/sellers.txt");
		if(!EntityIOUtils::loadOffers(market, DATA_DIR+"/offers.txt")) {
			fprintf(stderr, "TEST: loadOffers failed\n");
		}
		EntityIOUtils::printSellers(market);
	}

	void printPriceRangeData(const PriceRangeData& data) {
		cout << MSG_TAG << "price range data: " << endl;
		FORC(it, data) {
			cout << it->first << " " << it->second.first
					<< " - " << it->second.second << endl;
		}
	}

	bool testPriceRange() {
		PriceRangeData data;
		GenIOUtils::loadPriceRangeData(data, DATA_DIR+"/price-ranges.txt");
		printPriceRangeData(data);
		GenIOUtils::savePriceRangeData(DATA_DIR+"/foo_price-ranges.out", data);
		return true;
	}

}

#endif /* TEST_ENTITY_IO_H_ */
