/*
 * currency.h
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
 * Currency.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef CURRENCY_H_
#define CURRENCY_H_

#include "base_entity.h"
#include "price_utils.h"
#include "entity_io_utils.h"

#include <string>
#include <map>
#include <iostream>

class Currency : public BaseEntity {
private:
	static const Currency* base_;
	static const double defaultPriceUnitsPerUnitValue = 100;
	static double price_units_per_unit_value_;
	static std::map<std::string, Currency> currencies_;

	std::string code_;
	double relative_rate_;
public:
	Currency(std::string code, double relativeRate);
	const std::string& code() const;
	double relative_rate() const;
	double getValue(Price price) const;
	Price getPrice(double value_in_currency) const;
	Price convert(const Currency& to) const;
	static const Currency* setBase(const Currency& baseCurrency,
			double priceUnitsPerUnitValue);
	static const Currency* getCurrency(const std::string& code);
	static const Currency* base_currency();

	static const std::map<std::string, Currency>& currencies();
	friend std::ostream& operator<<(std::ostream& os, const Currency& c);

	friend class EntityIOUtils;
};


#endif /* CURRENCY_H_ */
