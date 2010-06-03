/*
 * ga_fitness_eval.cpp
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
 * ga_fitness_eval.cpp
 *
 *  Created on: May 31, 2010
 *      Author: losvald
 */

#include <ecf/ECF.h>
#include "ga_fitness_eval.h"
#include "purchasable_offer.h"

#include <vector>
#include <iostream>
#include <cmath>


using namespace std;

void debugFitnessElements(double v, double ubound, int max_q, int intValue) {
	cout << max_q << "*" << v << "/" << ubound << " = " << intValue << endl;
	cin.get();
}

GAFitnessEvalOp::GAFitnessEvalOp(GASelector& selector, double ubound)
: selector_(selector), ubound_(ubound) {

}

FitnessP GAFitnessEvalOp::evaluate(IndividualP individual)
{
	FitnessP fitness = static_cast<FitnessP> (new FitnessMin);
	double value = infinitePrice;

	BinaryP bin = boost::dynamic_pointer_cast<Binary> (individual->getGenotype());

	if(selector_.isInProgress()) {
		selector_.clearSelection();
		for(uint i = 0; i < bin->realValue.size(); ++i) {
//			int intVal = (int) bin->realValue.at(i);
//			if(intVal < 0) intVal = 0;
			int intVal = getSelectCount(bin, i, ubound_);
//			debugFitnessElements(bin->realValue.at(i), ubound_,
//					selector_.getInitialSelectLimit(selector_.getOffer(i)), intVal);
			selector_.selectOffer(selector_.getOffer(i), intVal);
		}

		if(selector_.isCurrentSelectionFeasible()) {
			value = selector_.selection().getTotalPrice();
			selector_.updateBestSelection(selector_.selection());
		}
		else {
			const int C = 100;
			int flow = selector_.calcMaxFlow();
			int total = selector_.total_disj_req_quantity();
			value = selector_.worst_acceptable_price()+1
					+ C*(total*total - flow*flow);

			double d = 0;
			for(uint i = 0; i < bin->realValue.size(); ++i) {
				const PurchasableOffer& offer = selector_.getOffer(i);
//				d += pow(selector_.selection().getOfferCount(offer), 2.0);
				d += pow(selector_.getInitialSelectLimit(offer)
						-selector_.selection().getOfferCount(offer), 2.0);
			}
			value += C - log(1+d);
		}
	}

	fitness->setValue(value);
	return fitness;
}

Quantity GAFitnessEvalOp::getSelectCount(BinaryP bin, int n, double ubound) {
	const PurchasableOffer& offer = selector_.getOffer(n);
	int max_q = selector_.getInitialSelectLimit(offer);
	int ret = (int) ((1+max_q) * bin->realValue.at(n) / ubound);
	if(ret > max_q)
		ret = max_q;
	else if(ret < 0)
		ret = 0;
	return (Quantity) ret;
}
