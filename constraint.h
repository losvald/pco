/*
 * constraint.h
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
 * Constraint.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include "base_entity.h"
#include "constraint_modifier.h"

#include <cstdlib>

template<typename T>
class Constraint : public BaseEntity {

public:
	virtual bool isMet(const T& purchasable) const {
		return false;
	}
};

template<typename T>
class ComplexConstraint : public Constraint<T> {
private:
	const Constraint<T>* a_;
	const Constraint<T>* b_;
	const Constraint<T>* c_;
	const int type;
	const int modifier_;
public:
	ComplexConstraint(const Constraint<T>& a,
			UnaryConstraintModifier unaryConstraintModifier)
	: a_(&a), b_(NULL), c_(NULL), type(1), modifier_(unaryConstraintModifier) {
	}
	ComplexConstraint(const Constraint<T>& a, const Constraint<T>& b,
			BinaryConstraintModifier binaryConstraintModifier)
	: a_(&a), b_(&b), c_(NULL), type(2), modifier_(binaryConstraintModifier) {
	}
	ComplexConstraint(const Constraint<T>& a, const Constraint<T>& b,
			const Constraint<T>& c,
			TernaryConstraintModifier ternaryConstraintModifier)
	: a_(&a), b_(&b), c_(&c), type(3), modifier_(ternaryConstraintModifier) {
	}
	virtual bool isMet(const T& object) const {
		switch(modifier_) {
		case NOT:
			return !a_->isMet(object);
		case AND:
			return a_->isMet(object) && b_->isMet(object);
		case OR:
			return a_->isMet(object) || b_->isMet(object);
		case XOR:
			return a_->isMet(object) ^ b_->isMet(object);
		case IF:
			return a_->isMet(object) ? b_->isMet(object)
					: c_->isMet(object);
		default:
			return false;
		}
	}
};

template<typename T>
class NoConstraint : public Constraint<T> {
public:
	NoConstraint() {
	}
	bool isMet(const T& object) const {
		return true;
	}
};

template<typename T>
class EqualsConstraint : public Constraint<T> {
private:
	const T& other_;
public:
	EqualsConstraint(const T& equalsTo) : other_(equalsTo) {
	}
	virtual bool isMet(const T& object) const {
		return object == other_;
	}
};

template<typename T>
class BoundedRangeConstraint : public Constraint<T> {
private:
	const T& from_;
	const T& to_;
public:
	BoundedRangeConstraint(const T& fromInclusive, const T& toExclusive)
	: from_(fromInclusive), to_(toExclusive) {
	}
	virtual bool isMet(const T& object) const {
		return !(object < from_) && object < to_;
	}
};


#endif /* CONSTRAINT_H_ */
