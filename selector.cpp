/*
 * selector.cpp
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
 * Selector.cpp
 *
 *  Created on: Apr 23, 2010
 *      Author: losvald
 */

#include "selector.h"

using namespace std;

Selector::Selector(Market& market, const PurchasableData& purchasable_data)
: market_(market), purchasable_data_(purchasable_data),
  total_disj_req_quantity_(0),
  non_disj_constr_mapper_(0) {
	selection_.clear();
	//calculate max offer select limits if selection contains only that offer
	vector<const PurchasableOffer*> offers = market.getAllOffers();
	FORC(o, offers) {
		initial_select_limits_[(*o)->id()] = (*o)->seller()->maxQuantity((*o)->id());
	}
}

Market& Selector::market() const {
	return market_;
}

const Selection& Selector::selection() const {
	return selection_;
}

bool Selector::isRunning() const {
	return isTimeRunning();
}

Quantity Selector::deselectAllOffers(const PurchasableOffer& offer) {
	Quantity ret = 0;
	FORC(it, selection_.offers()) {
		ret += deselectOffer(it->first, it->second);
	}
	return ret;
}

Quantity Selector::getInitialSelectLimit(const PurchasableOffer& offer) const {
	if(!initial_select_limits_.count(offer.id()))
		return 0;
	return initial_select_limits_.at(offer.id());
}

Quantity Selector::total_disj_req_quantity() const {
	return total_disj_req_quantity_;
}

void Selector::initStart(const std::vector<BuyerRequest>& purchasable_requests,
			const Constraint<Seller>& seller_constraint,
			const Constraint<Selection>& selection_constraint) {
	seller_constraint_ = seller_constraint;
	selection_constraint_ = selection_constraint;
	requests_.clear();
	total_disj_req_quantity_ = 0;
	non_disj_requests_.clear();
	non_disj_constr_mapper_.clear();
	FORC(r, purchasable_requests) {
		if(r->isDisjunctive()) {
			requests_.push_back(*r);
			total_disj_req_quantity_ += r->quantity();
		}
		else {
			non_disj_requests_.push_back(*r);
			non_disj_constr_mapper_.add(r->id());
		}
	}
	clearSelection();
	Selection best_sel = selection_;
	best_sel.set_total_price(infinitePrice);
	set_best_selection(best_sel);
}

bool Selector::disjunctiveRequestsMet() const {
	FORC(r, requests_) {
		if(!r->isMet())
			return false;
	}
	return true;
}

bool Selector::nonDisjunctiveRequestsMet() const {
	FORC(ndr, non_disj_requests_) {
		if(!ndr->isMet())
			return false;
	}
	return true;
}

void Selector::clearSelection() {
	//backup first (because map will be modified)
	vector< pair<PurchasableOffer, Quantity> > v;
	v.reserve(selection_.offers().size());
	FORC(it, selection_.offers()) {
		v.push_back(make_pair(it->first, it->second));
	}
	FORC(o, v) {
		deselectOffer(o->first, o->second);
	}
}

void Selector::setSelection(const Selection& selection) {
	clearSelection();

	//backup first (because map will be modified)
	vector< pair<PurchasableOffer, Quantity> > v;
	v.reserve(selection_.offers().size());
	FORC(it, selection_.offers()) {
		v.push_back(make_pair(it->first, it->second));
	}

	FORC(it, v) {
		selection_.addOffer(it->first,
				selectOffer(it->first, it->second));
	}
}

bool Selector::isCurrentSelectionFeasible() const {
	return disjunctiveRequestsMet() && nonDisjunctiveRequestsMet();
}

void Selector::set_best_selection(const Selection& selection) {
	best_selection_ = selection;
}

const Selection& Selector::best_selection() const {
	return best_selection_;
}

bool Selector::updateBestSelection(const Selection& selection) {
	if(selection.getTotalPrice() < best_selection().getTotalPrice()) {
		set_best_selection(selection);
		cout << "[SELECTOR] Updated best selection: " << endl;
		cout << best_selection() << endl;
		return true;
	}
	return false;
}

