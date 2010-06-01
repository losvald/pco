/*
 * base_types.h
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
 * base_types.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef BASE_TYPES_H_
#define BASE_TYPES_H_

#include <utility>

typedef long long Price;
static const Price zeroPrice = 0LL;
static const Price infinitePrice = 0x3f3f3f3f3f3f3f3fLL;

typedef int Quantity;
typedef std::pair<Quantity, Quantity> QuantityRange;
static const Quantity maxQuantity = 0x3f3f3f3f;
static const Quantity unlimitedQuantity = -1;

struct InvalidQuantityRangeException { };

#endif /* BASE_TYPES_H_ */
