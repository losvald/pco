/*
 * random_utils.cpp
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
 * random_utils.cpp
 *
 *  Created on: Apr 22, 2010
 *      Author: losvald
 */

#include "random_utils.h"

unsigned RandomUtils::default_seed_val_ = new_seed_val();
unsigned* RandomUtils::seed_ = &RandomUtils::default_seed_val_;

unsigned RandomUtils::new_seed_val() {
	return RandomUtils::new_seed_val(&default_seed_val_);
}

unsigned RandomUtils::new_seed_val(const unsigned* address_offset) {
	return (unsigned)(time(NULL) + address_offset);
}


