/*
 * flow_based_selector.cpp
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
 * flow_based_selector.cpp
 *
 *  Created on: May 25, 2010
 *      Author: losvald
 */

#include "flow_based_selector.h"

using namespace std;

namespace flow_based_selector {
	static const char* MSG_TAG = "[FLOW-SEL] ";
}

FlowBasedSelector::FlowBasedSelector(Market& market,
		PurchasableData& purchasable_data)
: Selector(market, purchasable_data),
  purchasable_node_mapper_(2), request_node_mapper_(2) {
}

FlowBasedSelector::~FlowBasedSelector() {
	// TODO Auto-generated constructor stub

}

void FlowBasedSelector::initStart(const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint) {

	cout << flow_based_selector::MSG_TAG << "INIT START" << endl;
	Selector::initStart(purchasable_requests,
			seller_constraint, selection_constraint);
	graph_.clear();
	request_node_mapper_.clear();
	FORC(r, purchasable_requests) {
		if(r->isDisjunctive()) {
			request_node_mapper_.add(r->id());
			Id h = request_node_mapper_.hash(r->id());
			cout << "HASH: " << r->id() << "->"
					<< h
					<< "( " << request_node_mapper_.isHashed(r->id()) << endl;
			cout << "DEHASH: " << h << "<-"
								<< request_node_mapper_.dehash(h)
								<< endl;
			Id hashed_id = request_node_mapper_.hash(r->id());
			graph_.insert(MyEdge(hashed_id, sink_, r->quantity()));
		}
	}
	purchasable_node_mapper_ = hash::RangeMapper<Id, Id>(2
			+request_node_mapper_.size());
	cout << "Purch. node mapper from: " << purchasable_node_mapper_.from() << endl;
}

Quantity FlowBasedSelector::deselectOffer(const PurchasableOffer & offer,
		Quantity quantity) {
	quantity = min(quantity, selection_.getOfferCount(offer));
	quantity = offer.seller()->returnOffer(offer.id(), quantity);
	selection_.removeOffer(offer, quantity);

//	cout << flow_based_selector::MSG_TAG << "Deselected: "
//				<< quantity << "x " << offer.id() << endl;

	FORC(p, offer.purchasables()) {
		updateGraph(*p, selection_.getPurchasableQuantity(*p));
	}

	return quantity;
}

Quantity FlowBasedSelector::selectOffer(const PurchasableOffer & offer,
		Quantity quantity) {
	quantity = offer.seller()->buyOffer(offer.id(), quantity);
	selection_.addOffer(offer, quantity);

//	cout << flow_based_selector::MSG_TAG << "Selected: "
//			<< quantity << "x " << offer.id() << endl;

	FORC(p, offer.purchasables()) {
		updateGraph(*p, selection_.getPurchasableQuantity(*p));
	}

	return quantity;
}

void FlowBasedSelector::updateGraph(Id purchasable_id, Quantity quantity) {
//	cout << "Update graph: " << quantity << "x " << purchasable_id << endl;

	purchasable_node_mapper_.add(purchasable_id);
	Id hashed_pid = purchasable_node_mapper_.hash(purchasable_id);
//	cout << "Update edge: " << src_ << " " << hashed_pid << " " << quantity << endl;
	setEdge(src_, hashed_pid, quantity);

	set<MyEdge, EdgeComparator>::iterator it_from = graph_.lower_bound(
						MyEdge(hashed_pid, 2, -0x3f3f3f3f));
	bool were_edges = (it_from != graph_.end() && it_from->a == hashed_pid);
	if(quantity == 0) {
		vector<MyEdge> to_remove;

		for(set<MyEdge, EdgeComparator>::iterator it = it_from;
				it != graph_.end() && it->a == it_from->a; ++it) {
//			cout << "Edge: " << it->a << " " << it->b << " " << it->w << endl;
			to_remove.push_back(*it);
		}
		FORC(it, to_remove) {
			setEdge(it->a, it->b, 0);
		}
	}
	else if(!were_edges) {
//		cout << flow_based_selector::MSG_TAG << "New purchasable: "
//				<< purchasable_id << endl;
		FORC(r, requests_) {
			const Purchasable* p = purchasable_data_.getPurchasable(
					purchasable_id);
			if(p == NULL) {
				//TODO
			}
			if(r->constraint().isMet(*p)) {
				setEdge(hashed_pid, request_node_mapper_.hash(r->id()),
						r->quantity());
			}
		}
	}

}

void FlowBasedSelector::setEdge(Id from, Id to, int capacity) {
	MyEdge edge(from, to, capacity);
	set<MyEdge, EdgeComparator>::iterator it = graph_.lower_bound(edge);
	if(it != graph_.end() && it->a == edge.a && it->b == edge.b) {
		graph_.erase(it);
	}
	if(capacity != 0)
		graph_.insert(edge);
}

int FlowBasedSelector::calcMaxFlow() const {
	vector<MyEdge> v;
	v.reserve(graph_.size());
	FORC(it, graph_) {
		v.push_back(*it);
	}
	return find_max_flow(v, src_, sink_);
}

bool FlowBasedSelector::disjunctiveRequestsMet() const {
	int max_flow = calcMaxFlow();
	if(max_flow > total_disj_req_quantity_) {
		throw FlowExceptoin();
	}
	return max_flow == total_disj_req_quantity_;
}

//debug

void FlowBasedSelector::printGraph() const {
	cout << flow_based_selector::MSG_TAG << "PRINT GRAPH" << endl;
	cout << selection_;
	FORC(it, graph_) {
		Id from = it->a, to = it->b;
		if(from == src_) {
			to = purchasable_node_mapper_.dehash(to);
		}
		else if(to == sink_) {
			from = request_node_mapper_.dehash(from);
		}
		else {
			from = purchasable_node_mapper_.dehash(from);
			to = request_node_mapper_.dehash(to);
		}

		if(from  == src_)
			cout << "src";
		else {
			cout << from;
		}
		cout << " ";
		if(it->b  == sink_)
			cout << "snk";
		else {
			cout << to;
		}
		cout << " " << it->w << "\t(" << it->a << " " << it->b
				<< " " << it->w << ")" << endl;
	}
}
