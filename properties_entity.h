/*
 * properties_entity.h
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
 * properties_entity.h
 *
 *  Created on: Apr 13, 2010
 *      Author: losvald
 */

#ifndef PROPERTIES_ENTITY_H_
#define PROPERTIES_ENTITY_H_

#include "base_entity.h"
#include "property.h"
#include <map>

class PropertiesEntity : public BaseEntity {
protected:
	std::map<string, string> props_;
public:
	PropertiesEntity();
	PropertiesEntity(Id id);
	PropertiesEntity(Id id, std::string name);
	void addProperty(const Property& property);
	void setProperty(const std::string& key, const std::string& value);
	void clearProperties();
	const std::map<string, string>& properties() const;
};

#endif /* PROPERTIES_ENTITY_H_ */
