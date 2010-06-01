/*
 * currency.cpp
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
 * Currency.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "currency.h"

#include <cstdio>
#include <cstring>
using namespace std;

const Currency* Currency::base_ = NULL;
double Currency::price_units_per_unit_value_
= Currency::defaultPriceUnitsPerUnitValue;

std::map<std::string, Currency> Currency::currencies_;

Currency::Currency(string code, double relativeRate)
: code_(code), relative_rate_(relativeRate) {
	if(base_ != NULL)
		cout << *base_;
	else cout << "(null)" << endl;
}

const string& Currency::code() const {
	return code_;
}

double Currency::relative_rate() const {
	return relative_rate_;
}

Price Currency::getPrice(double value) const {
	if(base_ == NULL)
		return -1;
	return value * base_->relative_rate() / relative_rate()
			* price_units_per_unit_value_;
}

double Currency::getValue(Price price) const {
	if(base_ == NULL)
		return -1;
	return price * relative_rate() / base_->relative_rate()
			/ price_units_per_unit_value_;
}

const Currency* Currency::setBase(const Currency& baseCurrency, double priceUnitsPerUnitValue
		= Currency::defaultPriceUnitsPerUnitValue) {
	const Currency* previous = base_;
	price_units_per_unit_value_ = priceUnitsPerUnitValue;
	base_ = &baseCurrency;
	return previous;
}

const Currency* Currency::getCurrency(const string& code) {
	if(currencies_.count(code))
		return &currencies_.at(code);
	return NULL;
}

const Currency* Currency::base_currency() {
	return base_;
}

//test

const map<std::string, Currency>& Currency::currencies() {
	return currencies_;
}

ostream& operator<<(ostream& os, const Currency& c) {
	os << "Code=" << c.code() << "; rate=" << c.relative_rate()
			<< "; " << endl;
	return os;
}

