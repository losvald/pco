/*
 * brute_force_solver.h
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
 * brute_force_solver.h
 *
 *  Created on: May 27, 2010
 *      Author: losvald
 */

#ifndef BRUTE_FORCE_SOLVER_H_
#define BRUTE_FORCE_SOLVER_H_

#include "market.h"

#include <string>

namespace brute_force {

bool solve(Market& market, const std::string& filename,  Price price_limit);

}

#endif /* BRUTE_FORCE_SOLVER_H_ */
