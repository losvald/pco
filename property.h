/*
 * property.h
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
 * Property.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include "property.h"

#include <string>
using namespace std;

class Property {
private:
	string name_;
	string value_;
public:
	Property();
	Property(const string& name, const string& value);
	string name() const;
	void set_name(const string& name);
	string value() const;
	void set_value(const string& value);
};

#endif /* PROPERTY_H_ */
