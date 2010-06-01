/*
 * market.cpp
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
 * market.cpp
 *
 *  Created on: Apr 11, 2010
 *      Author: losvald
 */

#include "market.h"
#include "string_utils.h"
#include "purchasable_data.h"
#include "currency.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

Market Market::instance_;

void Market::registerSeller(const Seller& seller) {
	printf("Registering seller:\n");
	cout << seller;
	sellers_.insert(make_pair(seller.id(), seller));
}

std::vector<const Seller*> Market::getSellers() const {
	vector<const Seller*> ret;
	FORC(it, sellers_)
		ret.push_back(&it->second);
	return ret;
}

std::vector<Seller*> Market::sellers() {
	vector<Seller*> ret;
	FORC(it, sellers_) {
		ret.push_back(&it->second);
	}
	return ret;
}

Seller* Market::seller(Id id) {
	if(!sellers_.count(id))
		return NULL;
	return &sellers_.at(id);
}

std::vector<const PurchasableOffer*> Market::getAllOffers() const {
	vector<const PurchasableOffer*> ret;
	vector<const Seller*> _sellers = getSellers();
	FORC(s, _sellers) {
		vector<const PurchasableOffer*> offers = (*s)->allOffers();
		FORC(o, offers) {
			ret.push_back(*o);
		}
	}
	return ret;
}
