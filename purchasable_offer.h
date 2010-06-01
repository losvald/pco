/*
 * purchasable_offer.h
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
 * PurchasableOffer.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef PURCHASABLE_OFFER_H_
#define PURCHASABLE_OFFER_H_

#include "purchasable.h"
#include "price_utils.h"
#include "seller.h"
#include "currency.h"
#include "constraint.h"

#include <set>
#include <map>

class Seller;
class Currency;

class PurchasableOffer : public PropertiesEntity {
private:
	Price price_;
	Price shipping_;
	Seller* seller_;
	const Currency* currency_;
	std::set<Id> purchasables_;
	std::map<Id, Quantity> counter_;
	std::map< std::pair<Quantity, Quantity>, Price > wholesale_prices_;
	Quantity contents_size_;
public:
	PurchasableOffer();
	PurchasableOffer(Id id);
	PurchasableOffer(Id id, Seller& seller);
	PurchasableOffer(Id id, Seller& seller, const Currency& currency);
	Price price() const;
	Price shipping() const;
	Price wholesalePrice(Quantity quantity) const;
	void set_price(Price price);
	void set_shipping(Price price);
	void set_currency(const Currency& currency);
	void set_wholesale_price(const std::pair<Quantity, Quantity>
		quantityRangeInclusive, Price price);
	const std::map< std::pair<Quantity, Quantity>, Price >&
	wholesale_prices() const;
	const Currency& currency() const;
	void addPurchasable(const Purchasable& purchasable, Quantity quantity);
	void clearPurchasables();
	const set<Id>& purchasables() const;
	bool contains(const Purchasable& p) const;
	bool meetsConstraint(const Constraint<Purchasable>& constraint) const;
	Quantity getQuantity(const Purchasable& p) const;
	Quantity getQuantity(Id purchasable_id) const;
	Seller* seller() const;
	void set_seller(Seller* seller);
	Quantity contents_size() const;
private:
	void init(Seller* seller);
	void init(Seller* seller, const Currency* currency);

	friend bool operator<(const PurchasableOffer& a, const PurchasableOffer& b);
	friend bool operator==(const PurchasableOffer& a, const PurchasableOffer& b);
	friend ostream& operator<<(ostream& os, const PurchasableOffer& offer);
};

#endif /* PURCHASABLE_OFFER_H_ */
