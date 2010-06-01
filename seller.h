/*
 * seller.h
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
 * PurchasableProvider.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef PURCHASABLE_PROVIDER_H_
#define PURCHASABLE_PROVIDER_H_

#include "properties_entity.h"
#include "purchasable.h"
#include "purchasable_offer.h"
#include "currency.h"
#include "supply.h"
#include "constraint.h"

#include <set>
#include <vector>
#include <algorithm>

using namespace std;

class PurchasableOffer;

class Seller : public PropertiesEntity {
private:
	Price shipping_cost_;
	Price subscription_fee_;
	const Currency* default_currency_;
	map<Id, PurchasableOffer> all_offers_;
	map<Id, set<Id> > offers_;
	Supply supplies_;
public:
	Seller();
	Seller(const Currency* defaultCurrency);
	const PurchasableOffer& offer(Id offer_id) const;
	const Currency* default_currency() const;
	void set_default_currency(const Currency* currency);
	Price shipping_cost() const;
	void set_shipping_cost(Price price);
	Price subscription_fee() const;
	void set_subscription_fee(Price price);
	bool addOffer(const PurchasableOffer& offer);
	bool removeOffer(Id offer_id);
	void clearOffers();
	const Supply& supplies() const;
	void addSupply(Id purchasableId, Quantity quantity);
	void clearAllSupplies();
	void clearSupply(Id purchasable_id);
	Quantity getQuantity(Id purchasable_id) const;
	vector<const PurchasableOffer*> allOffers() const;
	bool offers(Id purchasableId) const;
	virtual set<const PurchasableOffer*> getOffers(Id purchasable_id) const;
	virtual set<const PurchasableOffer*> getOffers(
			const Constraint<Purchasable>& constraint) const;

	Quantity buyOffer(Id offer_id, Quantity times);
	Quantity returnOffer(Id offer_id, Quantity times);
	Quantity maxQuantity(Id offer_id) const;
private:
	bool hasOffer(Id offer_id);
	void init();

	friend bool operator<(const Seller& a, const Seller& b);
	friend bool operator==(const Seller& a, const Seller& b);

	friend ostream& operator<<(ostream& os, const Seller& s);
};

#endif /* PURCHASABLE_PROVIDER_H_ */
