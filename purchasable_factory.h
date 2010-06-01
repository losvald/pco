/*
 * purchasable_factory.h
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
 * purchasable_factory.h
 *
 *  Created on: Apr 17, 2010
 *      Author: losvald
 */

#ifndef PURCHASABLE_FACTORY_H_
#define PURCHASABLE_FACTORY_H_

#include "entity_factory.h"
#include "purchasable.h"

#include "price_utils.h"
#include <string>

class PurchasableFactory : public EntityFactory<Purchasable> {
private:

public:
	PurchasableFactory(const std::string& name);
	virtual Purchasable createEntity();
	virtual ~PurchasableFactory();
};

#endif /* PURCHASABLE_FACTORY_H_ */
