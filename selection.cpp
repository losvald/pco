/*
 * selection.cpp
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
 * Selection.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "selection.h"
#include "utils.h"
#include "string_utils.h"

Selection::Selection()
: total_price_(0) {
	// TODO Auto-generated constructor stub

}

Selection::~Selection() {
	// TODO Auto-generated destructor stub
}

const map<PurchasableOffer, Quantity>& Selection::offers() const {
	return offers_;
}

void Selection::clear() {
	std::vector<PurchasableOffer> v;
	v.reserve(offers_.size());
	FORC(it, offers_) {
		v.push_back(it->first);
	}

	FORC(it, v) {
		clearOffer(*it);
	}
}

bool Selection::addOffer(const PurchasableOffer& offer, Quantity times) {
	if(times <= 0)
		return false;
	if(!offers_.count(offer)) {
		offers_.insert(make_pair(offer, times));
		total_price_ += offer.wholesalePrice(times);
		//update purchasable count
		FORC(p, offer.purchasables()) {
			increasePurchasableCount(*p, offer.getQuantity(*p) * times);
		}
		return true;
	}
	Quantity old_q = offers_[offer];
	total_price_ -= offer.wholesalePrice(old_q);
	Quantity new_q = old_q + times;
	offers_[offer] = new_q;
	total_price_ += offer.wholesalePrice(new_q);
	//update purchasable count
	FORC(p, offer.purchasables()) {
		increasePurchasableCount(*p, offer.getQuantity(*p) * times);
	}
	return false;
}

bool Selection::removeOffer(const PurchasableOffer& offer, Quantity times = 1) {
	if(times <= 0)
		return false;
	if(hasOffer(offer, times)) {
		Quantity old_q = offers_[offer];
		total_price_ -= offer.wholesalePrice(old_q);
		Quantity new_q = old_q - times;
		offers_[offer] = new_q;
		if(offers_[offer] <= 0) {
			offers_.erase(offer);
		}
		else {
			total_price_ += offer.wholesalePrice(new_q);
		}
		//update purchasable count
		FORC(p, offer.purchasables()) {
			decreasePurchasableCount(*p, offer.getQuantity(*p) * times);
		}
		return true;
	}
	return false;
}

bool Selection::clearOffer(const PurchasableOffer& offer) {
	if(hasOffer(offer, 1)) {
		return removeOffer(offer, getOfferCount(offer));
	}
	return false;
}

bool Selection::hasOffer(const PurchasableOffer& offer, Quantity times) const {
	return getOfferCount(offer) >= times;
}

Quantity Selection::getOfferCount(const PurchasableOffer& offer) const {
	if(offers_.find(offer) != offers_.end())
		return offers_.at(offer);
	return 0;
}

bool Selection::hasPurchasable(Id purchasable_id, Quantity minQuantity) const {
	return getPurchasableQuantity(purchasable_id) >= minQuantity;
}

Quantity Selection::getPurchasableQuantity(Id purchasable_id) const {
	if(purchasable_quanities_.count(purchasable_id))
		return (Quantity)purchasable_quanities_.at(purchasable_id);
	return 0;
}

void Selection::loadFrom(const std::vector< std::pair<PurchasableOffer, Quantity> >&
			offer_quantities) {
	clear();
}
std::vector< std::pair<PurchasableOffer, Quantity> > Selection::toVector() const {
	std::vector< std::pair<PurchasableOffer, Quantity> > ret;
	FORC(it, offers_) {
		ret.push_back(make_pair(it->first, it->second));
	}
	return ret;
}

bool Selection::increasePurchasableCount(Id purchasable_id, int inc) {
	if(inc == 0)
		return false;
	if(!purchasable_quanities_.count(purchasable_id)) {
		purchasable_quanities_[purchasable_id] = inc;
		return true;
	}
	purchasable_quanities_[purchasable_id] += inc;
	return false;
}

bool Selection::decreasePurchasableCount(Id purchasable_id, int dec) {
	if(dec == 0)
		return false;
	if(!purchasable_quanities_.count(purchasable_id)
			|| purchasable_quanities_[purchasable_id] <= dec) {
		purchasable_quanities_.erase(purchasable_id);
		return true;
	}
	purchasable_quanities_[purchasable_id] -= dec;
	return false;
}

Price Selection::getTotalPrice() const {
	return total_price_;
}

void Selection::set_total_price(Price price) {
	total_price_ = price;
}

std::ostream& operator<<(std::ostream& os, const Selection& s) {
	Selection::OfferConfiguration config = s.toVector();
	FORC(it, s.offers_) {
		os << it->second << "x " << it->first.id() <<
				"(" << it->first.name() << ")" << std::endl;
	}
	os << "Price: " << s.getTotalPrice() << std::endl;
	return os;
}

