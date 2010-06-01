/*
 * ga_fitness_eval.h
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
 * ga_fitness_eval.h
 *
 *  Created on: May 31, 2010
 *      Author: losvald
 */

#ifndef GA_FITNESS_EVAL_H_
#define GA_FITNESS_EVAL_H_

#include "ga_selector.h"
#include "base_types.h"

#include <vector>

class GASelector;

class GAFitnessEvalOp : public EvaluateOp {
private:
	GASelector& selector_;
	double ubound_;
public:
	GAFitnessEvalOp(GASelector& selector, double ubound_);
	FitnessP evaluate(IndividualP individual);
private:
	Quantity getSelectCount(BinaryP bin, int n, double ubound);
};
typedef boost::shared_ptr<GAFitnessEvalOp> GAFitnessEvalOpP;

#endif /* GA_FITNESS_EVAL_H_ */
