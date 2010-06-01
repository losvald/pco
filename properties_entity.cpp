/*
 * properties_entity.cpp
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
 * properties_entity.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: losvald
 */

#include "properties_entity.h"
#include "property.h"

PropertiesEntity::PropertiesEntity() {
}

PropertiesEntity::PropertiesEntity(Id id) : BaseEntity(id) {

}
PropertiesEntity::PropertiesEntity(Id id, std::string name) : BaseEntity(id, name) {
}

void PropertiesEntity::addProperty(const Property& property) {
	props_.insert(make_pair(property.name(), property.value()));
}

void PropertiesEntity::setProperty(const std::string& key,
		const std::string& value) {
	addProperty(Property(key, value));
}

void PropertiesEntity::clearProperties() {
	props_.clear();
}

const std::map<string, string>& PropertiesEntity::properties() const {
	return props_;
}
