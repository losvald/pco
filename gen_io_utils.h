/*
 * gen_io_utils.h
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
 * gen_io_utils.h
 *
 *  Created on: Apr 17, 2010
 *      Author: losvald
 */

#ifndef GEN_IO_UTILS_H_
#define GEN_IO_UTILS_H_

#include "price_range_data.h"

class GenIOUtils {
public:
	static bool loadPriceRangeData(PriceRangeData& data,
			const std::string& filename);

	static bool savePriceRangeData(const std::string& filename,
			const PriceRangeData& data);
};

#endif /* GEN_IO_UTILS_H_ */
