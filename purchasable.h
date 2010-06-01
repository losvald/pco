/*
 * purchasable.h
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
 * BaseEntity.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef PURCHASABLE_H_
#define PURCHASABLE_H_

#include "properties_entity.h"
#include "utils.h"

#include <map>
#include <string>
using namespace std;

enum PurchasableType {
	PRODUCT,
	SERVICE
};

class Purchasable : public PropertiesEntity {
private:
	double amount_;
	PurchasableType type_;
public:
	Purchasable();
	double amount() const;
	PurchasableType type() const;
	friend bool operator<(const Purchasable& a, const Purchasable& b);
	friend bool operator==(const Purchasable& a, const Purchasable& b);
	friend ostream& operator<<(ostream& os, const Purchasable& p);
};

#endif /* PURCHASABLE_H_ */
