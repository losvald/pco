/*
 * base_entity.cpp
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
 * base_entity.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#include "base_entity.h"

#include <cstdio>

BaseEntity::BaseEntity() {
	init(UNDEFINED_ID, "");
}

BaseEntity::BaseEntity(Id id) {
	init(id);
}

BaseEntity::BaseEntity(Id id, const std::string& name) {
	init(id, name);
}

int BaseEntity::id() const {
	return id_;
}

void BaseEntity::set_id(int id) {
	id_ = id;
}

std::string BaseEntity::name() const {
	return name_;
}

void BaseEntity::set_name(const std::string& name) {
	name_ = name;
}

void BaseEntity::init(Id id, const std::string& name) {
	set_id(id);
	set_name(name);
}

void BaseEntity::init(Id id) {
	static char buff[20];
	sprintf(buff, "%d", id); //TODO
	init(id, buff);
}

std::ostream& operator<<(std::ostream& os, const BaseEntity& b) {
	os << "BaseEntity(id=" << b.id() << "; name=" << b.name() << ")";
	return os;
}
