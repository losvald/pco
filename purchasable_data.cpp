/*
 * purchasable_data.cpp
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
 * purchasable_data.cpp
 *
 *  Created on: Apr 11, 2010
 *      Author: losvald
 */

#include "purchasable_data.h"
#include "purchasable.h"
#include "utils.h"
#include "string_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

PurchasableData PurchasableData::instance_;

PurchasableData::PurchasableData() {
	init();
}

void PurchasableData::init() {
	//TODO
}

PurchasableData& PurchasableData::instance() {
	return PurchasableData::instance_;
}

void PurchasableData::addPurchasable(const Purchasable& p) {
	purchasables_.insert(make_pair(p.id(), p));
}

void PurchasableData::clearPurchasables() {
	purchasables_.clear();
}

const Purchasable* PurchasableData::getPurchasable(Id id) const {
	return (purchasables_.find(id) != purchasables_.end())
			? &purchasables_.at(id) : NULL;
}

vector<const Purchasable*> PurchasableData::getPurchasables() const {
	vector<const Purchasable*> ret;
	FORC(it, purchasables_)
		ret.push_back(&it->second);
	return ret;
}

int PurchasableData::size() const {
	return purchasables_.size();
}
