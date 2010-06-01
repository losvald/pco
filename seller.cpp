/*
 * seller.cpp
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
 * PurchasableProvider.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "seller.h"
#include "string_utils.h"

Seller::Seller() : default_currency_(NULL) {
	init();
}

Seller::Seller(const Currency* defaultCurrency)
: default_currency_(defaultCurrency) {
	init();
}

const PurchasableOffer& Seller::offer(Id offer_id) const {
	return all_offers_.at(offer_id);
}

const Currency* Seller::default_currency() const {
	return default_currency_;
}

void Seller::set_default_currency(const Currency* currency) {
	default_currency_ = currency;
}

Price Seller::shipping_cost() const {
	return shipping_cost_;
}

void Seller::set_shipping_cost(Price price) {
	shipping_cost_ = price;
}

Price Seller::subscription_fee() const {
	return subscription_fee_;
}

void Seller::set_subscription_fee(Price price) {
	subscription_fee_ = price;
}

bool Seller::addOffer(const PurchasableOffer& offer) {
	FORC(id, offer.purchasables()) {
		offers_[*id].insert(offer.id());
	}
	return all_offers_.insert(make_pair(offer.id(), offer)).second;
}

bool Seller::removeOffer(Id offer_id) {
	bool found = hasOffer(offer_id);
	if(found) {
		FORC(id, offer(offer_id).purchasables()) {
			offers_[*id].erase(offer_id);
		}
		all_offers_.erase(offer_id);
	}
	return found;
}

bool Seller::offers(Id purchasable_id) const {
	return offers_.count(purchasable_id);
}

set<const PurchasableOffer*> Seller::getOffers(Id purchasable_id) const {
	set<const PurchasableOffer*> ret;
	FORC(it, offers_.at(purchasable_id))
		ret.insert(&all_offers_.at(*it));
	return ret;
}

set<const PurchasableOffer*> Seller::getOffers(
			const Constraint<Purchasable>& constraint) const {
	set<const PurchasableOffer*> s;
	FORC(it, all_offers_) {
		if(it->second.meetsConstraint(constraint)) {
			s.insert(&it->second);
		}
	}
	return s;
}

Quantity Seller::getQuantity(Id purchasable_id) const {
	if(!supplies_.count(purchasable_id))
		return 0;
	return supplies_.at(purchasable_id);
}

vector<const PurchasableOffer*> Seller::allOffers() const {
	vector<const PurchasableOffer*> ret;
	FORC(it, all_offers_) {
		ret.push_back(&it->second);
	}
	return ret;
}

const Supply& Seller::supplies() const {
	return supplies_;
}

void Seller::addSupply(Id purchasableId, Quantity quantity) {
	if(quantity <= 0)
		return ;
	if(!supplies_.count(purchasableId))
		supplies_.insert(make_pair(purchasableId, quantity));
	else {
		supplies_[purchasableId] += quantity;
	}
}

void Seller::clearAllSupplies() {
	supplies_.clear();
}

void Seller::clearSupply(Id purchasable_id) {
	supplies_.erase(purchasable_id);
}

Quantity Seller::buyOffer(Id offer_id, Quantity times) {
	if(times <= 0) return 0;
	times = std::min(times, maxQuantity(offer_id));
	const PurchasableOffer& offer = all_offers_.at(offer_id);
	FORC(p, offer.purchasables()) {
		supplies_[*p] -= times * offer.getQuantity(*p);
	}
	return times;
}

int Seller::returnOffer(Id offer_id, Quantity times) {
	if(times <= 0) return 0;
	const PurchasableOffer& offer = all_offers_.at(offer_id);
	FORC(p, offer.purchasables()) {
		if(!supplies_.count(*p))
			supplies_[*p] = 0;
		supplies_[*p] += times * offer.getQuantity(*p);
	}
	return times;
}

Quantity Seller::maxQuantity(Id offer_id) const {
	if(!all_offers_.count(offer_id))
		return 0;
	const PurchasableOffer& offer = all_offers_.at(offer_id);
	Quantity ret = ::maxQuantity;
	FORC(p, offer.purchasables()) {
		ret = std::min(ret, getQuantity(*p) / offer.getQuantity(*p));
	}
	return ret;
}

//private methods

bool Seller::hasOffer(Id offer_id) {
	return all_offers_.count(offer_id);
}

void Seller::init() {
	shipping_cost_ = zeroPrice;
	subscription_fee_ = zeroPrice;
}


bool operator<(const Seller& a, const Seller& b) {
	return a.id() < b.id();
}

bool operator==(const Seller& a, const Seller& b) {
	return a.id() == b.id();
}

ostream& operator<<(ostream& os, const Seller& s) {
	os << "Seller #" << s.id() << ": " << s.name() << endl;
	os << "Default Currency: " << *s.default_currency() << endl;
	vector<PurchasableOffer> offers;
	FORC(it, s.all_offers_)
		offers.push_back(it->second);
	os << "Offers(" << offers.size() << "): " << endl;
	os << StringUtils::to_string(offers.begin(), offers.end(), "--\n", "", "");
	os << endl;
	os << "Supplies: " << endl;
	FORC(it, s.supplies_) {
		os << it->second << "x " << it->first << endl;
	}
	os << endl;
	return os;
}


