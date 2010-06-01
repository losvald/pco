/*
 * manual_selector.h
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
 * manual_selector.h
 *
 *  Created on: May 29, 2010
 *      Author: losvald
 */

#ifndef MANUAL_SELECTOR_H_
#define MANUAL_SELECTOR_H_

#include "flow_based_selector.h"

class ManualSelector : public FlowBasedSelector {
public:
	ManualSelector(Market& market, PurchasableData& purchasable_data);
	virtual ~ManualSelector();

	virtual bool start(
			const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint);
	virtual bool stop();
};

namespace test {
	void testManualSelector(Market& market, const string& filename);
}

#endif /* MANUAL_SELECTOR_H_ */
