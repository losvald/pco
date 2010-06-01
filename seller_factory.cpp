/*
 * seller_factory.cpp
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
 * seller_factory.cpp
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#include "seller_factory.h"

Id SellerFactory::curr_id_;

SellerFactory::SellerFactory(int offer_count,
		Market& market,
		const Currency& currency,
		const PurchasableData& purchasable_data,
		const PriceRangeData& purchasable_price_ranges,
		Price min_offer_price, Price max_offer_price,
		const OfferFactory::ContentsSizeVar& contents_size_var,
		const OfferFactory::ContentsQuantityVar& contents_quantity_var,
		const OfferFactory::WholesalePricesVar& wholesale_prices_var,
		const QuantityRange& supply_range,
		unsigned seed)
: EntityFactory<Seller>(seed),
  offer_count_(offer_count),
  market_(market),
  currency_(currency),
  purchasable_data_(purchasable_data),
  purchasable_price_ranges_(purchasable_price_ranges),
  min_offer_price_(min_offer_price), max_offer_price_(max_offer_price),
  contents_size_var_(contents_size_var),
  contents_quantity_var_(contents_quantity_var),
  wholesale_prices_var_(wholesale_prices_var),
  supply_range_(supply_range) {

}


Seller SellerFactory::createEntity() {
	printf("Generating seller #%d...\n", curr_id_);
	Seller seller;
	char buff[24]; sprintf(buff, "%d", curr_id_);
	seller.set_name(string("seller")+buff);
	seller.set_id(curr_id_++);
	seller.set_default_currency(Currency::base_currency());
	market_.registerSeller(seller);
	Seller* s = market_.seller(seller.id());

	std::cout << "Supplies for seller #" << seller.id() << ":\n";
	std::vector<const Purchasable*> purchasables_ = purchasable_data_
				.getPurchasables();
	FORC(p, purchasables_) {
		s->addSupply((*p)->id(), RandomUtils::random(supply_range_, seed()));
//		std::cout << "Supplies(" << (*p)->id() << "): "
//		<< seller.getQuantity((*p)->id());
	}

	map<Id, QuantityRange> offer_limit_ranges = OfferFactory::equal_limit_ranges(purchasable_data_,
			supply_range_);
	OfferFactory offer_factory(purchasable_data_,
			purchasable_price_ranges_,
			min_offer_price_, max_offer_price_,
			contents_size_var_,
			contents_quantity_var_,
			wholesale_prices_var_,
			offer_limit_ranges,
			seed_value());

	for(int i = 0; i < offer_count_; ++i) {
		PurchasableOffer offer = offer_factory.createEntity();
		offer.set_seller(s);
		s->addOffer(offer);
	}
	return *s;
}
