/*
 * base_entity.h
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
 * base_entity.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef BASE_ENTITY_H_
#define BASE_ENTITY_H_

#include <string>
#include <iostream>

typedef int Id;

class BaseEntity {
public:
	static const Id UNDEFINED_ID = -1LL;
private:
	Id id_;
	std::string name_;

public:
	BaseEntity();
	BaseEntity(Id id);
	BaseEntity(Id id, const std::string& name);
	Id id() const;
	void set_id(Id id);
	std::string name() const;
	void set_name(const std::string& name);
protected:
	void init(Id id);
	void init(Id, const std::string& name);

	friend std::ostream& operator<<(std::ostream& os,
			const BaseEntity& base_entity);

//	friend class EntityFactory;
};

#endif /* BASE_ENTITY_H_ */
