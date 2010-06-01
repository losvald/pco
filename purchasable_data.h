/*
 * purchasable_data.h
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
 * purchasable_data.h
 *
 *  Created on: Apr 11, 2010
 *      Author: losvald
 */

#ifndef PURCHASABLE_DATA_H_
#define PURCHASABLE_DATA_H_

#include "purchasable.h"
#include <map>
#include <vector>

class PurchasableData {
private:
	std::map<Id, Purchasable> purchasables_;
	static PurchasableData instance_;
public:
	PurchasableData();
//	bool inputPurchasables(char *filename);
//	bool outputPurchasables(char *filename) const;
	void clearPurchasables();
	const Purchasable* getPurchasable(Id id) const;
	std::vector<const Purchasable*> getPurchasables() const;
	static PurchasableData& instance();
	void addPurchasable(const Purchasable& p);
	int size() const;
private:
	void init();

	friend class EntityIOUtils;
};

#endif /* PURCHASABLE_DATA_H_ */
