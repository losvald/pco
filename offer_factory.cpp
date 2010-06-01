/*
 * offer_factory.cpp
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
 * offer_factory.cpp
 *
 *  Created on: Apr 17, 2010
 *      Author: losvald
 */

#include "offer_factory.h"
#include "random_utils.h"
#include "currency.h"
#include <cmath>
#include <iostream>
#include "string_utils.h"
#include <stdexcept>

Id OfferFactory::curr_id_;


void OfferFactory::debug_limits() {
	std::vector<const Purchasable*> purchasables_ = PurchasableData::instance().getPurchasables();
	std::cout << "Offer limit ranges: " << offer_limit_ranges_.size() << " n purch: "
			<< purchasables_.size() << std::endl;
	FORC(p, purchasables_) {
		std::cout << "OfferLimitRange(" << (*p)->id() << "): "
				<< offer_limit_ranges_.at((*p)->id())
				<< std::endl;
	}
}

OfferFactory::OfferFactory(const PurchasableData& purchasable_data,
		const PriceRangeData& purchasable_price_ranges,
		Price min_offer_price, Price max_offer_price,
		const ContentsSizeVar& contents_size_var,
		const ContentsQuantityVar& contents_quantity_var,
		const WholesalePricesVar& wholesale_prices_var,
		const std::map<Id, QuantityRange>& offer_limit_ranges,
		unsigned seed)
: EntityFactory<PurchasableOffer>(seed),
  purchasable_data_(purchasable_data),
  purchasable_price_ranges_(purchasable_price_ranges),
  min_offer_price_(min_offer_price), max_offer_price_(max_offer_price),
  contents_size_var_(contents_size_var),
  contents_quantity_var_(contents_quantity_var),
  wholesale_prices_var_(wholesale_prices_var),
  offer_limit_ranges_(offer_limit_ranges) {
	init();
	//debug_limits();

}

PurchasableOffer OfferFactory::createEntity() {
	++curr_id_;
	printf("Generating offer #%d...\n", curr_id_);
	char buff[24]; sprintf(buff, "%d", curr_id_);
	PurchasableOffer offer(curr_id_);
	offer.set_name(string("offer")+buff);
	//take random k purchasables
	int n = purchasables_.size();
	int k = std::min(contents_size_var_.get(), n);
//	printf("Csv seed: %u\n", contents_size_var_.seed_value());
//	printf("N = %d; K = %d; seed = %u\n", n, k, *seed());
	vector<int> v = RandomUtils::random_vector(k, make_pair(0, n-1), seed());
//	cout << "Random k purch: " << StringUtils::to_string(v.begin(), v.end()) << endl;
	Price offer_price = 0;
	FORC(r, v) {
		const Purchasable* p = purchasables_.at(*r);
		if(p == NULL) {
			cout << "Warning: " << "purchasable #" << *r << " not found" << endl;
			continue;
		}
		Quantity q = getPossibleQuantity(p->id());
		offer.addPurchasable(*p, q);
		PriceRange range;
		if(purchasable_price_ranges_.count(p->id())) {
			range = purchasable_price_ranges_.at(p->id());
		} else {
			range = make_pair(10, 1000);
			fprintf(stderr, "Price range not found for purch. #%d: %s %lld - %lld\n",
					p->id(), "setting range:", range.first, range.second);
		}
		++range.second;
		Price price = RandomUtils::random(range, seed());
		cout << "+" << price << "\t" << q << "x " << *p << endl;
		offer_price += q * price;
	}
	//printf("Setting price to %lld\n", offer_price);
	offer.set_currency(*Currency::base_currency());
	offer.set_price(offer_price);
	vector< pair< QuantityRange, Price> > wholesale_prices = wholesale_prices_var_
			.getWholesalePrices(offer_price);
	FORC(it, wholesale_prices) {
		offer.set_wholesale_price(it->first, it->second);
	}
//	cout << StringUtils::to_string(wholesale_prices.begin(), wholesale_prices.end());

	cout << offer;

	return offer;
}

void OfferFactory::init() {
	purchasables_ = purchasable_data_.getPurchasables();
}

Quantity OfferFactory::getPossibleQuantity(Id purchasableId) {
//	std::cout << "PID = " << purchasableId;
	QuantityRange exclRange;
//	cerr << "[" << offer_limit_ranges_.size()
//				<< ": " <<
//				offer_limit_ranges_
//				.at(purchasableId) << endl;
//		cerr << "Press key..";
	exclRange = RandomUtils::fromInclusiveRange(
			offer_limit_ranges_
			.at(purchasableId));
	Quantity limit = RandomUtils::random(
			exclRange, seed());
	Quantity q = contents_quantity_var_.get();
	if(q > limit)
		q = limit;
	return q;
}

