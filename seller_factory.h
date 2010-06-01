/*
 * seller_factory.h
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
 * seller_factory.h
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#ifndef SELLER_FACTORY_H_
#define SELLER_FACTORY_H_


#include "entity_factory.h"
#include "offer_factory.h"
#include "seller.h"
#include "market.h"
#include "currency.h"
#include "base_types.h"
#include "random_utils.h"
#include "price_range_data.h"
#include "purchasable_data.h"
#include "purchasable_offer.h"

#include <map>

class SellerFactory : public EntityFactory<Seller> {
public:

	SellerFactory(int offer_count,
			Market& market,
			const Currency& currency,
			const PurchasableData& purchasable_data,
			const PriceRangeData& purchasable_price_ranges,
			Price min_offer_price, Price max_offer_price,
			const OfferFactory::ContentsSizeVar& contents_size_var,
			const OfferFactory::ContentsQuantityVar& contents_quantity_var,
			const OfferFactory::WholesalePricesVar& wholesale_prices_var,
			const QuantityRange& supply_range,
			unsigned seed);

	Seller createEntity();

protected:
	static Id curr_id_;
	int offer_count_;
	Market& market_;
	const Currency& currency_;
	const PurchasableData& purchasable_data_;
	const PriceRangeData& purchasable_price_ranges_;
	Price min_offer_price_;
	Price max_offer_price_;
	const OfferFactory::ContentsSizeVar& contents_size_var_;
	const OfferFactory::ContentsQuantityVar& contents_quantity_var_;
	const OfferFactory::WholesalePricesVar& wholesale_prices_var_;
	const QuantityRange& supply_range_;
//	const std::map<Id, QuantityRange> supply_ranges_;
};

#endif /* SELLER_FACTORY_H_ */
