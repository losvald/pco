/*
 * buyer_request.h
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
 * buyer_request.h
 *
 *  Created on: May 26, 2010
 *      Author: losvald
 */

#ifndef BUYER_REQUEST_H_
#define BUYER_REQUEST_H_

#include "boost/shared_ptr.hpp"

#include "base_entity.h"
#include "base_types.h"
#include "constraint.h"
#include "purchasable.h"

#include <vector>
#include <iostream>

class BuyerRequest : public BaseEntity {
private:
	boost::shared_ptr< Constraint<Purchasable> > constraint_;
	int quantity_;
	int met_count_;
	bool disjunctive_;
public:
	BuyerRequest(Constraint<Purchasable>* purchasable_constraint,
			Quantity quantity, bool disjunctive = true);
	const Constraint<Purchasable>& constraint() const;
	bool incrementMetCount(int inc = 1);
	bool decrementMetCount(int dec = 1);
	bool set_met_count(int met_count);
	int met_count() const;
	bool isMet() const;
	int remainingUntilMet() const;
	Quantity quantity() const;
	bool isDisjunctive() const;
	virtual ~BuyerRequest();

	friend std::ostream& operator<<(std::ostream& os, const BuyerRequest& br);
};

#endif /* BUYER_REQUEST_H_ */
