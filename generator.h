/*
 * generator.h
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
 * generator.h
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

//gen includes
#include "base_types.h"
#include "random_utils.h"
#include "gen_utils.h"
#include "gen_io_utils.h"
#include "entity_factory.h"
#include "offer_factory.h"
#include "seller_factory.h"
#include "price_range_data.h"
#include "manual_selector.h"

#include "market.h"
#include "entity_io_utils.h"

#include "test_entity_io.h"

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

bool generate(Market& market,
		string currencies_file,
		string purchasables_file,
		string price_range_data_file,
		string seller_file,
		string offers_file,
		string supplies_file,
		int min_supplies,
		int max_supplies,
		int num_offers,
		unsigned seed_val) {
	printf("Entity generation started.\n");
	PriceRangeData price_range_data;

	if(!EntityIOUtils::loadExchangeRates(currencies_file)) {
		cerr << "Error loading currencies" << endl;
		return false;
	}
	if(!EntityIOUtils::loadPurchasables(PurchasableData::instance(),
			purchasables_file)) {
		cerr << "Error loading purchasables" << endl;
		return false;
	}

	bool b = GenIOUtils::loadPriceRangeData(price_range_data, price_range_data_file);
	if(!b) {
		fprintf(stderr, "Invalid price list file\n");
		return false;
	}

	printf("ALL OK\n");

	struct SizeVar : public OfferFactory::ContentsSizeVar {
		SizeVar() {
			setRange(1, 10);
		}
		double getProbability(int q) const {
			if(q == 1) return 0.5;
			else if(q > 1 && q <= 3) return 0.1;
			return 0.1*exp(-(int)q*0.5);
		}
	} size_var;
	size_var.set_seed(seed_val);

	struct QuantityVar : public OfferFactory::ContentsQuantityVar {
		QuantityVar() {
			setRange(1, 100);
		}
		double getProbability(int quantity) const {
			return exp(-(int)quantity*0.1);
		}
	} quantity_var;
	quantity_var.set_seed(seed_val);

	struct WholesaleVar : public OfferFactory::WholesalePricesVar {
		WholesaleVar() : WholesalePricesVar(0.8) { }
		vector< pair< QuantityRange, Price> > getWholesalePrices(Price price) const {
			vector< pair< QuantityRange, Price> > ret;
			ret.push_back(make_pair(QuantityRange(3, 5), 0.95));
			ret.push_back(make_pair(QuantityRange(6, 9), 0.90));
			ret.push_back(make_pair(QuantityRange(10, 24), 0.87));
			ret.push_back(make_pair(QuantityRange(25, maxQuantity), 0.80));
			return ret;
		}
	} wholesale_var1;
	wholesale_var1.set_seed_val(seed_val);

	OfferFactory::DefaultWholesalePricesVar wholesale_var_def(QuantityRange(3, 5),
			QuantityRange(300, 400),
			QuantityRange(0, 30), seed_val);

	cout << "Offer size distribution: " << endl;
	vector< pair<int, double> > dist = size_var.getDistribution();
	FORC(it, dist) {
		cout << it->first << "\t" << it->second << endl;
	}

	cout << "Quantity distributions: " << endl;
	vector< pair<int, double> > dist2 = quantity_var.getDistribution();
	FORC(it, dist2) {
		cout << it->first << "\t" << it->second << endl;
	}

	Price min_offer_price = 100;
	Price max_offer_price = 1000000;

	map<Id, QuantityRange> offer_limit_ranges = OfferFactory::equal_limit_ranges(PurchasableData::instance(),
			QuantityRange(3, 7));


	QuantityRange supply_range(min_supplies, max_supplies);
	SellerFactory sf(num_offers,
			market,
			*Currency::base_currency(),
			PurchasableData::instance(), price_range_data,
			min_offer_price, max_offer_price,
			size_var,
			quantity_var,
			wholesale_var_def,
			supply_range,
			seed_val
	);

	for(int i = 0; i < 1; ++i) {
		Seller s = sf.createEntity();
		cout << s << endl;
	}

	cout << endl << "Seller generation done. " << endl;
//	scanf("%*c");
	vector<Seller*> v = market.sellers();
	FORC(it, v) {
		cout << "OUTPUTING SELLER: " << endl;
		cout << **it << endl;
	}

	EntityIOUtils::printMarket(market);

	if(!EntityIOUtils::saveSellers(seller_file, market)) {
		cerr << "[GEN] Error saving sellers" << endl;
		return false;
	}
	if(!EntityIOUtils::saveOffers(offers_file, market)) {
		cerr << "[GEN] Error saving offers" << endl;
		return false;
	}
	if(!EntityIOUtils::saveSupplies(supplies_file, market)) {
		cerr << "[GEN] Error saving supplies" << endl;
		return false;
	}

	return true;

//	OfferFactory of(PurchasableData::instance(), price_range_data,
//			min_offer_price, max_offer_price, size_var, quantity_var,
//			wholesale_var_def,
//			offer_limit_ranges,
//			seed_val);
//
//	while(true) {
//		scanf("%*c");
//		cout << of.createEntity() << endl;
//	}
//	return true;
}


#endif /* GENERATOR_H_ */
