/*
 * purchasable.cpp
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
 * Purchasable.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "purchasable.h"
#include "utils.h"

#include <iostream>

Purchasable::Purchasable() : type_(PRODUCT) { //XXX

}

double Purchasable::amount() const {
	return amount_;
}

PurchasableType Purchasable::type() const {
	return type_;
}

bool operator<(const Purchasable& a, const Purchasable& b) {
	return a.id() < b.id();
}

bool operator==(const Purchasable& a, const Purchasable& b) {
	return a.id() == b.id();
}

ostream& operator<<(ostream& os, const Purchasable& p) {
	string name = p.name();
	if(name.empty()) name = "Purchasable[";
	os << name << "[id=" << p.id() << "; props=[";
	bool first = true;
	FORC(it, p.props_) {
		if(first)
			first = false;
		else
			os << ", ";
		os << it->first << "=" << it->second;
	}
	os << "]]";
	return os;
}




