/*
 * greedy_selector.cpp
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
 * greedy_selector.cpp
 *
 *  Created on: May 29, 2010
 *      Author: losvald
 */

#include "greedy_selector.h"
#include "string_utils.h"

#include <algorithm>

using namespace std;

namespace greedy_selector {
	static const char* MSG_TAG = "[GREEDY-SEL] ";
}

GreedySelector::GreedySelector(Market& market,
		const PurchasableData& purchasable_data)
: Selector(market, purchasable_data) {
}

GreedySelector::~GreedySelector() {
	// TODO Auto-generated destructor stub
}

Quantity GreedySelector::selectOffer(const PurchasableOffer & offer,
		Quantity quantity) {
	quantity = offer.seller()->buyOffer(offer.id(), quantity);
	selection_.addOffer(offer, quantity);
	return quantity;
}

Quantity GreedySelector::deselectOffer(const PurchasableOffer & offer,
		Quantity quantity) {
	quantity = std::min(quantity, selection_.getOfferCount(offer));
	quantity = offer.seller()->returnOffer(offer.id(), quantity);
	selection_.removeOffer(offer, quantity);
	return quantity;
}

bool GreedySelector::stop() {
	return false;
}

bool lessPrice(const PurchasableOffer& A, const PurchasableOffer& B) {
	if(A.price() != B.price())
		return A.price() < B.price();
	if(A.contents_size() != B.contents_size())
		return A.contents_size() < B.contents_size();
	return true;
}

bool GreedySelector::start(const std::vector<BuyerRequest> & purchasable_requests,
		const Constraint<Seller> & seller_constraint,
		const Constraint<Selection> & selection_constraint) {
	initStart(purchasable_requests, seller_constraint, selection_constraint);

	cout << greedy_selector::MSG_TAG << "Starting..." << endl;

	cout << greedy_selector::MSG_TAG << "Disj. requests: \n";
	cout << StringUtils::to_string(requests_.begin(), requests_.end()) << endl;
	cout << greedy_selector::MSG_TAG << "Non-disj. requests: \n";
	cout << StringUtils::to_string(non_disj_requests_.begin(), non_disj_requests_.end()) << endl;


//	struct OfferComparator {
//		bool operator()(const PurchasableOffer& A, const PurchasableOffer& B) {
//			if(A.price() != B.price())
//				return A.price() < B.price();
//			if(A.contents_size() != B.contents_size())
//				return A.contents_size() < B.contents_size();
//			return true;
//		}
//	} cmp;

	vector<const PurchasableOffer*> offers = market_.getAllOffers();
	offers_by_price_.clear();
	offers_by_price_.reserve(offers.size());
	FORC(it, offers) {
		offers_by_price_.push_back(**it);
	}

	sort(offers_by_price_.begin(), offers_by_price_.end(), lessPrice);

	cout << "Offers by price: " << endl;
	cout << StringUtils::to_string(offers_by_price_.begin(), offers_by_price_.end(), "--");

	FORC(o, offers_by_price_) {
		if(disjunctiveRequestsMet())
			break;

		cout << greedy_selector::MSG_TAG << "CONSIDERING offer: "
							<< o->id() << endl;
		FORC(r, requests_) {
//			cout << *r;
//			cout << "Meets " << o->id() << "? "
//					<< o->meetsConstraint(r->constraint()) << endl;
		}

		while(true) {
			bool met = false;
			FORC(r, requests_) {
				if(!r->isMet() && o->meetsConstraint(r->constraint())) {
//					cout << "Meets request: " << *r;
					int q = selectOffer(*o, r->remainingUntilMet());
					if(q > 0) {
						cout << greedy_selector::MSG_TAG << "greedy SELECT: ";
						cout << q << "x " << o->id() << endl;
						r->incrementMetCount(q);
						FORC(p, o->purchasables()) {
							addForMatch(*p, o->getQuantity(*p) * q);
							matchAsMuchAsPossible();
						}
						met = true;
						break;
					}
				}
			}
			if(!met)
				break;
		}
	}

	FORC(o, offers_by_price_) {
		if(nonDisjunctiveRequestsMet())
			break;
		while(true) {
			bool met = false;
			FORC(ndr, non_disj_requests_) {
				if(!ndr->isMet() && o->meetsConstraint(ndr->constraint())) {
					int q = selectOffer(*o, ndr->remainingUntilMet());
					if(q > 0) {
						cout << greedy_selector::MSG_TAG
								<< "greedy SELECT: ";
						cout << q << "x " << o->id() << endl;
						ndr->incrementMetCount(q);
						met = true;
						break;
					}
				}
			}
			if(!met) break;
		}
	}

	return isCurrentSelectionFeasible();
}

int GreedySelector::match(Id purchasable_id, int times) {
	if(unmatched_purchasables_.count(purchasable_id))
		times = std::min(unmatched_purchasables_.at(purchasable_id),
				times);
	else
		return 0;

	unmatched_purchasables_[purchasable_id] -= times;
	if(unmatched_purchasables_[purchasable_id] <= 0) {
		unmatched_purchasables_.erase(purchasable_id);
	}
	return times;
}

void GreedySelector::addForMatch(Id purchasable_id, int times) {
	if(!unmatched_purchasables_.count(purchasable_id))
		unmatched_purchasables_[purchasable_id] = 0;
	else
		unmatched_purchasables_[purchasable_id] += times;
}

void GreedySelector::matchAsMuchAsPossible() {
//	FORC(ndr, non_disj_requests_) {
//		if(!ndr->isMet()) {
//			vector<Id> v;
//			FORC(p, unmatched_purchasables_) {
//				v.push_back(p->first);
//			}
//		}
//	}
	FORC(r, requests_) {
		if(!r->isMet()) {
			vector<Id> v;
			FORC(p, unmatched_purchasables_) {
				v.push_back(p->first);
			}
			FORC(p, v) {
				if(r->constraint().isMet(*PurchasableData::instance()
				.getPurchasable(*p))) {
					int matched = match(*p, r->remainingUntilMet());
					r->incrementMetCount(matched);
				}
			}
		}
	}
}



