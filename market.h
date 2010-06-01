/*
 * market.h
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
 * market.h
 *
 *  Created on: Apr 11, 2010
 *      Author: losvald
 */

#ifndef MARKET_H_
#define MARKET_H_

#include "seller.h"
#include "purchasable_offer.h"

#include <map>
#include <vector>
#include <string>

class Seller;
class PurchasableOffer;

class Market {
private:
	static Market instance_;
	std::map<Id, Seller> sellers_;
public:
	void registerSeller(const Seller& seller);
	std::vector<const Seller*> getSellers() const;
	std::vector<Seller*> sellers();
	Seller* seller(Id id);
	std::vector<const PurchasableOffer*> getAllOffers() const;
private:
//	Property toNameProperty(const PurchasableOffer& offer);
//	std::string fromNameProperty(const std::string& value);
//	Property toOfferContentsProperty(const PurchasableOffer& offer);
//	std::map<Id, Quantity> toOfferContentsProperty(const PurchasableOffer& offer);
//	std::string fromOfferContentsProperty(const std::string& value);
//	std::string toOfferRequirementsProperty(const std::string& value);
//	std::string fromOfferRequirementProperty(const std::string& value);
//	std::string toOfferPriceProperty(Price price);
//	Price fromOfferPriceProperty(const std::string& value);

	friend class EntityIOUtils;
};

#endif /* MARKET_H_ */
