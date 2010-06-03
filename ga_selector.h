/*
 * ga_selector.h
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
 * ga_selector.h
 *
 *  Created on: May 30, 2010
 *      Author: losvald
 */

#ifndef GA_SELECTOR_H_
#define GA_SELECTOR_H_

#include <ecf/ECF.h>
#include "ga_fitness_eval.h"

#include "flow_based_selector.h"
#include "purchasable_offer.h"

class GASelector : public FlowBasedSelector {
private:
	StateP state_;
	vector<const PurchasableOffer*> offers_;
	bool in_progress_;

public:
	GASelector(Market& market, PurchasableData& purchasable_data,
			const string& param_file, const string& argv0,
			int buy_offer_limit = -1);
	virtual bool start(
			const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint);
	virtual bool stop();
	const PurchasableOffer& getOffer(int n) const;

	bool isInProgress() const;
protected:
	bool isCurrentSelectionFeasible() const;
private:
	int calcOfferBuyLimit();

	friend class GAFitnessEvalOp;
};

#endif /* GA_SELECTOR_H_ */
