/*
 * selector.h
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
 * Selector.h
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#ifndef SELECTOR_H_
#define SELECTOR_H_

#include "base_types.h"
#include "time_counter.h"
#include "market.h"
#include "seller.h"
#include "constraint.h"
#include "selection.h"
#include "purchasable_offer.h"
#include "hash_utils.h"
#include "buyer_request.h"
#include "purchasable_data.h"

#include <vector>
#include <map>

class Selector : virtual public TimeCounter {
protected:
	Market& market_;
	const PurchasableData& purchasable_data_;

	Selection selection_;
	Selection best_selection_;
	std::vector<BuyerRequest> requests_;
	std::vector<BuyerRequest> non_disj_requests_;
	Quantity total_disj_req_quantity_;
	Constraint<Seller> seller_constraint_;
	Constraint<Selection> selection_constraint_;
	hash::RangeMapper<Id, Id> non_disj_constr_mapper_;
	std::map<Id, Quantity> initial_select_limits_;
	Price worst_acceptable_price_;
public:
	Selector(Market& market, const PurchasableData& purchasable_data);
	Market& market() const;
	const Selection& selection() const;
	const Selection& best_selection() const;
	virtual bool start(
			const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint) = 0;
	virtual bool stop() = 0;
	virtual bool isRunning() const;
	virtual Quantity total_disj_req_quantity() const;
	Price worst_acceptable_price() const;
	void set_worst_acceptable_price(Price price);
protected:
	virtual Quantity selectOffer(const PurchasableOffer& offer, Quantity times = 1)
	= 0;
	virtual Quantity deselectOffer(const PurchasableOffer& offer, Quantity times = 1)
	= 0;
	virtual Quantity deselectAllOffers(const PurchasableOffer& offer);
	virtual Quantity getInitialSelectLimit(const PurchasableOffer& offer) const;
	virtual void initStart(const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint);
	virtual void clearSelection();
	virtual void setSelection(const Selection& selection);
	virtual bool disjunctiveRequestsMet() const;
	virtual bool nonDisjunctiveRequestsMet() const;
	virtual bool isCurrentSelectionFeasible() const;
	virtual void set_best_selection(const Selection& selection);
	virtual bool updateBestSelection(const Selection& selection);
private:
};

#endif /* SELECTOR_H_ */
