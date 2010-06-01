/*
 * selection.h
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
 * Selection.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef SELECTION_H_
#define SELECTION_H_

#include "purchasable.h"
#include "seller.h"
#include "hash_utils.h"

#include <set>
#include <vector>
#include <map>
#include <iostream>

class Selection {
public:
	typedef std::vector< std::pair<PurchasableOffer, Quantity> >
	OfferConfiguration;
private:
	Price total_price_;
	std::map<PurchasableOffer, Quantity> offers_;
	std::set<Seller> sellers_;
	std::map<Id, Quantity> purchasable_quanities_;
public:
	Selection();
	virtual ~Selection();
	Price getTotalPrice() const;
	void set_total_price(Price price);
	const std::map<PurchasableOffer, Quantity>& offers() const;
	void clear();
	bool addOffer(const PurchasableOffer& offer, Quantity times);
	bool removeOffer(const PurchasableOffer& offer, Quantity times);
	bool clearOffer(const PurchasableOffer& offer);
	bool hasPurchasable(Id purchasable_id, Quantity minQuantity) const;
	Quantity getPurchasableQuantity(Id purchasable_id) const;
	bool hasOffer(const PurchasableOffer& offer, Quantity minQuantity) const;
	bool hasOffer(const PurchasableOffer& offer) const;
	Quantity getOfferCount(const PurchasableOffer& offer) const;
	void loadFrom(const std::vector< std::pair<PurchasableOffer, Quantity> >&
			offer_quantities);
	std::vector< std::pair<PurchasableOffer, Quantity> > toVector() const;

private:
	bool increasePurchasableCount(Id purchasable_id, int inc);
	bool decreasePurchasableCount(Id purchasable_id, int dec);

public:
	friend std::ostream& operator<<(std::ostream& os, const Selection& s);
};

#endif /* SELECTION_H_ */
