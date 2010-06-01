/*
 * offer_factory.h
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
 * offer_factory.h
 *
 *  Created on: Apr 17, 2010
 *      Author: losvald
 */

#ifndef OFFER_FACTORY_H_
#define OFFER_FACTORY_H_

#include "base_types.h"
#include "random_utils.h"
#include "entity_factory.h"
#include "price_range_data.h"
#include "purchasable_offer.h"
#include "seller.h"

#include <iostream>
#include <vector>
#include <map>
#include "string_utils.h"

class OfferFactory : public EntityFactory<PurchasableOffer> {
private:
	std::vector<const Purchasable*> purchasables_;
public:
	struct ContentsSizeVar : public RandomUtils::DiscreteRangeRandomVariable<int> {
		virtual double getProbability(int quantity) const = 0;
	};

	struct ContentsQuantityVar : public RandomUtils::DiscreteRangeRandomVariable<int> {
		virtual double getProbability(int size) const = 0;
	};

	class WholesalePricesVar : public RandomUtils::SeedClass {
	private:
		double discount_cap_;
	public:
		WholesalePricesVar(double discount_cap) : discount_cap_(discount_cap) { };
		virtual vector< pair< QuantityRange, Price> >
		getWholesalePrices(Price price) const = 0;
	};

	class DefaultWholesalePricesVar : public WholesalePricesVar {
	private:
		QuantityRange first_wholesale_range_;
		QuantityRange last_wholesale_range_;
		QuantityRange range_increment_;
		unsigned seed_val_;
		mutable unsigned* seed_;
	public:
		DefaultWholesalePricesVar(QuantityRange first_wholesale_range,
				QuantityRange last_wholesale_range,
				QuantityRange range_increment,
				unsigned seed_val = RandomUtils::new_seed_val())
		: WholesalePricesVar(0.8),
		  first_wholesale_range_(first_wholesale_range),
		  last_wholesale_range_(last_wholesale_range),
		  range_increment_(range_increment),
		  seed_val_(seed_val),
		  seed_(&seed_val_) /*!!!seed_val_*/ {
			if(range_increment.second <= 0)
				throw InvalidQuantityRangeException();
		}

//		int getRangeIndex(Quantity quantity) const {
//			int ret = lower_bound(thresholds_.begin(), thresholds_.end(), quantity)
//									- thresholds_.end();
//			return ret == thresholds_.size() ? -1 : ret;
//		}

		virtual Price calcWholesalePrice(Price base_price, QuantityRange range,
				int range_index) const {
			static const double discounts[] = {0.95, 0.90, 0.87, 0.85, 0.83, 0.8};
			int n = sizeof(discounts)/sizeof(double);
			if(range_index < n) return (Price) (discounts[range_index]*base_price+1e-9);
			return (Price) (0.75*base_price+1e-9);
		}

		vector< pair< QuantityRange, Price> > getWholesalePrices(Price price) const {

			int from = RandomUtils::random(
					RandomUtils::fromInclusiveRange(first_wholesale_range_), seed_);
			int to = RandomUtils::random(
					RandomUtils::fromInclusiveRange(last_wholesale_range_), seed_);

			vector< pair< QuantityRange, Price> > ret;

			if(to < from)
				return ret;

			int x = from, delta = 1;
			vector<Quantity> thresholds_;
			while(x < to) {
				thresholds_.push_back(x);
				x += delta += RandomUtils::random(
						RandomUtils::fromInclusiveRange(range_increment_), seed_);
			}
			thresholds_.push_back(to);
//			std::cout << "Wholesale thresholds: " << StringUtils::to_string(thresholds_.begin(),
//					thresholds_.end()) << endl;

			for(int i = 0; i < (int)thresholds_.size()-1; ++i) {
				QuantityRange qr = QuantityRange(thresholds_[i], thresholds_[i+1]-1);
				ret.push_back(make_pair(qr, calcWholesalePrice(price, qr, i)));
			}
			return ret;
		}

	};

	protected:
	static Id curr_id_;
	private:
	const PurchasableData& purchasable_data_;
	const PriceRangeData& purchasable_price_ranges_;
	Price min_offer_price_;
	Price max_offer_price_;
	const ContentsSizeVar& contents_size_var_;
	const ContentsQuantityVar& contents_quantity_var_;
	const WholesalePricesVar& wholesale_prices_var_;
	const std::map<Id, QuantityRange>& offer_limit_ranges_;

	public:

	OfferFactory(const PurchasableData& data,
			const PriceRangeData& purchasable_price_ranges,
			Price min_offer_price, Price max_offer_price,
			const ContentsSizeVar& contents_size_var,
			const ContentsQuantityVar& contents_quantity_var,
			const WholesalePricesVar& wholesale_prices_var,
			const std::map<Id, QuantityRange>& offer_limit_ranges,
			unsigned seed);

	PurchasableOffer createEntity();
	static std::map<Id, QuantityRange> equal_limit_ranges(
			const PurchasableData& purchasable_data,
			const QuantityRange& range) {
		std::vector<const Purchasable*> purchasables_ = purchasable_data
				.getPurchasables();
		std::map<Id, QuantityRange> ret;
		FORC(p, purchasables_) {
			ret[(*p)->id()] = range;
			std::cout << "OfferLimitRange(" << (*p)->id() << "): "
								<< ret[(*p)->id()] << std::endl;
		}
		return ret;
	}
	//	std::vector<const Purchasable*> purchasable_pool() const;
	private:
	void init();
	Quantity getPossibleQuantity(Id purchasableId);
	void debug_limits();
};

#endif /* OFFER_FACTORY_H_ */
