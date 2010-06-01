/*
 * buyer_request.cpp
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
 * buyer_request.cpp
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#include "buyer_request.h"

BuyerRequest::BuyerRequest(Constraint<Purchasable>* purchasable_constraint,
		Quantity quantity, bool disjunctive)
: constraint_(purchasable_constraint), quantity_(quantity), met_count_(0),
  disjunctive_(disjunctive) {
}

BuyerRequest::~BuyerRequest() {
	// TODO Auto-generated destructor stub
}

const Constraint<Purchasable>& BuyerRequest::constraint() const {
	return *constraint_.get();
}

Quantity BuyerRequest::quantity() const {
	return quantity_;
}

bool BuyerRequest::isDisjunctive() const {
	return disjunctive_;
}

bool BuyerRequest::incrementMetCount(int inc) {
	return set_met_count(met_count() + inc);
}

bool BuyerRequest::decrementMetCount(int dec) {
	return set_met_count(met_count() - dec);
}

bool BuyerRequest::set_met_count(int met_count) {
	bool was_met = isMet();
	met_count_ = met_count;
	return was_met ^ isMet();
}

int BuyerRequest::met_count() const {
	return met_count_;
}

bool BuyerRequest::isMet() const {
	return met_count() >= quantity();
}

int BuyerRequest::remainingUntilMet() const {
	if(isMet()) return 0;
	return quantity() - met_count();
}

std::ostream& operator<<(std::ostream& os, const BuyerRequest& br) {
	os << "Request #" << br.id() << ": " << br.name();
	os << "(met " << br.met_count() << "/" << br.quantity() << ")" << endl;
	return os;
}
