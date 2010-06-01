/*
 * purchasable_offer.cpp
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
 * PurchasableOffer.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "purchasable_offer.h"
#include "purchasable_data.h"

#include <iostream>

PurchasableOffer::PurchasableOffer() {
	init(NULL, NULL);
}

PurchasableOffer::PurchasableOffer(Id id) : PropertiesEntity(id) {
	init(NULL, NULL);
}

PurchasableOffer::PurchasableOffer(Id id, Seller& seller,
		const Currency& currency) : PropertiesEntity(id) {
	init(&seller, &currency);
}

PurchasableOffer::PurchasableOffer(Id id, Seller& seller)
: PropertiesEntity(id) {
	init(&seller);
}

void PurchasableOffer::set_currency(const Currency& currency) {
	currency_ = &currency;
}

const Currency& PurchasableOffer::currency() const {
	return *currency_;
}

Price PurchasableOffer::price() const {
	return price_;
}

void PurchasableOffer::set_price(Price price) {
	price_ = price;
}

Price PurchasableOffer::shipping() const {
	return shipping_;
}

void PurchasableOffer::set_shipping(Price price) {
	shipping_ = price;
}

const std::map< std::pair<Quantity, Quantity>, Price >&
PurchasableOffer::wholesale_prices() const {
	return wholesale_prices_;
}

void PurchasableOffer::set_wholesale_price(const std::pair<Quantity, Quantity>
		quantityRangeInclusive, Price price) {
	wholesale_prices_[quantityRangeInclusive] = price;
}

Price PurchasableOffer::wholesalePrice(Quantity quantity) const {
	if(quantity == 0)
		return 0;
	if(quantity == 1)
		return price();
	Price regular_price = quantity * price_;
	if(wholesale_prices_.empty())
		return regular_price;

	__typeof(wholesale_prices_.begin()) it = wholesale_prices_
				.upper_bound(make_pair<Quantity, Quantity>(quantity,
						maxQuantity));
	if(it == wholesale_prices_.begin())
		return regular_price;
	--it;
	if(quantity < it->first.first || quantity > it->first.second)
		return regular_price;
	return quantity * it->second;
}

const set<Id>& PurchasableOffer::purchasables() const {
	return purchasables_;
}

void PurchasableOffer::addPurchasable(const Purchasable& purchasable,
		Quantity quantity = 1) {
	purchasables_.insert(purchasable.id());
	counter_[purchasable.id()] += quantity;
	contents_size_ += quantity;
}

void PurchasableOffer::clearPurchasables() {
	purchasables_.clear();
	counter_.clear();
	contents_size_ = 0;
}

bool PurchasableOffer::contains(const Purchasable& p) const {
	return purchasables_.find(p.id()) != purchasables_.end();
}

bool PurchasableOffer::meetsConstraint(const Constraint<Purchasable>& constraint) const {
	FORC(it, purchasables_) {
		const Purchasable* p = PurchasableData::instance().getPurchasable(*it);
		if(p != NULL && constraint.isMet(*p))
			return true;
	}
	return false;
}

Quantity PurchasableOffer::getQuantity(const Purchasable& p) const {
	return getQuantity(p.id());
}

Quantity PurchasableOffer::getQuantity(Id purchasable_id) const {
	if(counter_.count(purchasable_id))
		return counter_.at(purchasable_id);
	return 0;
}

Quantity PurchasableOffer::contents_size() const {
	return contents_size_;
}

Seller* PurchasableOffer::seller() const {
	return seller_;
}

void PurchasableOffer::set_seller(Seller* seller) {
	seller_ = seller;
}

void PurchasableOffer::init(Seller* seller) {
	init(seller, seller->default_currency());
}

void PurchasableOffer::init(Seller* seller, const Currency* currency) {
	seller_ = seller;
	currency_ = currency;
	contents_size_ = 0;
}

bool operator<(const PurchasableOffer& a, const PurchasableOffer& b) {
	if(a.purchasables_ != b.purchasables_)
		return a.purchasables_ < b.purchasables_;
	return a.price_ < b.price_;
}

bool operator==(const PurchasableOffer& a, const PurchasableOffer& b) {
	return a.purchasables_ == b.purchasables_
			&& a.price_ == b.price_;
}
#include <cstdio>
#include "string_utils.h"
ostream& operator<<(ostream& os, const PurchasableOffer& offer) {
//	os << "---------OFFER BEGIN------------" << endl;
	os << "Offer #" << offer.id() << ": " << offer.name() << endl;
	os << "Seller: ";
	if(offer.seller_ == NULL)
		os << "N/A";
	else
		os << offer.seller_->id() << " - " << offer.seller_->name();
	os << endl;
	FORC(id, offer.purchasables()) {
		const Purchasable& p = *PurchasableData::instance().getPurchasable(*id);
		os << offer.getQuantity(p) << "x " << p << endl;
	}
	os << "Total: " << offer.price() << "\t = \t"
			<< offer.currency().getValue(offer.price())
			<< " " << offer.currency().code() << endl;
	os << "Wholesale discounts: " << endl;
	FORC(it, offer.wholesale_prices_) {
		os << it->first << ": " << offer.currency().getValue(it->second)
				<< " " << offer.currency().code() << endl;
	}
//	os << "Wholesale prices: " << endl;
//	for(Quantity q = 1; q < 25; ++q) {
//		os << offer.currency().getValue(offer.wholesalePrice(q))
//				<< " " << offer.currency().code() << " for " << q << endl;
//	}
//	os << "Debug-wholesale: " << endl;
//	FORC(p, offer.wholesale_prices_) {
//		os << StringUtils::to_string(p->first, "-", "", "")
//		<< ": " << p->second << endl;
//	}
//	os << "----------OFFER END----------------" << endl;
	return os;
}
