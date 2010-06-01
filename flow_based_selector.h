/*
 * flow_based_selector.h
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
 * flow_based_selector.h
 *
 *  Created on: May 25, 2010
 *      Author: losvald
 */

#ifndef FLOW_BASED_SELECTOR_H_
#define FLOW_BASED_SELECTOR_H_

#include "selector.h"
#include "max_flow.h"

#include <set>
#include <vector>

class FlowBasedSelector : public Selector {
protected:
	static const Id src_ = 0;
	static const Id sink_ = 1;
	struct FlowExceptoin { };
private:
	struct EdgeComparator {
		bool operator()(const MyEdge& A, const MyEdge& B) {
			if(A.a != B.a)
				return A.a < B.a;
			return A.b < B.b;
		}
	};

	std::set<MyEdge, EdgeComparator> graph_;
	hash::RangeMapper<Id, Id> purchasable_node_mapper_;
	hash::RangeMapper<Id, Id> request_node_mapper_;

public:
	FlowBasedSelector(Market& market, PurchasableData& purchasable_data);
	virtual ~FlowBasedSelector();
	virtual bool start(
			const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint) = 0;
	virtual bool stop() = 0;
	virtual Quantity selectOffer(const PurchasableOffer& offer, Quantity times = 1);
	virtual Quantity deselectOffer(const PurchasableOffer& offer, Quantity times = 1);

	virtual bool disjunctiveRequestsMet() const;
	int calcMaxFlow() const;
protected:
	virtual void initStart(const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint);
private:
	void updateGraph(Id purchasable_id, Quantity quantity);
	void setEdge(Id from, Id to, int capacity);

//debug
protected:
	void printGraph() const;
};

#endif /* FLOW_BASED_SELECTOR_H_ */
