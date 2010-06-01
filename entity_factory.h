/*
 * entity_factory.h
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
 * EntityFactory.h
 *
 *  Created on: Apr 16, 2010
 *      Author: losvald
 */

#ifndef ENTITY_FACTORY_H_
#define ENTITY_FACTORY_H_

#include <cstdio>
#include <cstdlib>
#include <ctime>

template<typename T>
class EntityFactory {
private:
	unsigned seed_val_;
	unsigned* seed_;
public:
	EntityFactory() {
		init((unsigned)time(NULL));
	}
	EntityFactory(unsigned seed) {
		init(seed);
	}
	virtual T createEntity() = 0;
	unsigned seed_value() const { return seed_val_; }
	virtual ~EntityFactory() {
	}
protected:
	unsigned* seed() { return seed_; }
private:
	void init(unsigned seed) {
		seed_val_ = seed;
		seed_ = &seed_val_;
		printf("Created factory with seed: %u!\n", seed);
	}
};

#endif /* ENTITY_FACTORY_H_ */
