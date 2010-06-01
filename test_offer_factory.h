/*
 * test_offer_factory.h
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
 * test_offer_factory.h
 *
 *  Created on: May 22, 2010
 *      Author: losvald
 */

#ifndef TEST_OFFER_FACTORY_H_
#define TEST_OFFER_FACTORY_H_

#include "offer_factory.h"
#include "string_utils.h"
#include "random_utils.h"

#include <iostream>

using namespace std;

namespace test {

void testDiscounts() {
	OfferFactory::DefaultWholesalePricesVar wholesale_var(QuantityRange(3, 5),
			QuantityRange(300, 400),
			QuantityRange(0, 30), 25223);

	vector< pair<QuantityRange, Price> > v = wholesale_var.getWholesalePrices(100);

	cout << StringUtils::to_string(v.begin(), v.end());
}



}

#endif /* TEST_OFFER_FACTORY_H_ */
