/*
 * gen_io_utils.cpp
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
 * gen_io_utils.cpp
 *
 *  Created on: Apr 17, 2010
 *      Author: losvald
 */

#include "gen_io_utils.h"
#include "string_utils.h"

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

namespace ioconstants {
	static const char COMMENT_TAG = '#';
	static const int LINE_LENGTH = 4096;
	static const char* PRICE_RANGE_SEPARATOR = "-";
	static const char* MSG_TAG = "[GEN-IO-UTILS] ";
}

#include <iostream>

bool GenIOUtils::loadPriceRangeData(PriceRangeData& data,
			const std::string& filename) {
	static char buff[ioconstants::LINE_LENGTH];
	printf("%sLoading price ranges from \"%s\"...\n", ioconstants::MSG_TAG,
			filename.c_str());

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		fprintf(stderr, "%sFile \"%s\" does not exists\n", ioconstants::MSG_TAG,
				filename.c_str());
		return false;
	}

	int line = 0;
	while(fgets(buff, ioconstants::LINE_LENGTH, f)) {
		++line;
		int len = strlen(buff);
		buff[--len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		printf("Line: %s\n", buff);
		vector<string> tokens = StringUtils::split(buff, " ");
		cout << StringUtils::to_string(tokens.begin(), tokens.end()) << endl;;
		if(tokens.size() < 2) {
			fprintf(stderr, "Error in line #%d\n", line);
			return false;
		}
		int id = atoi(tokens[0].c_str());
		double min_value, max_value = 0;
		string currency_code;
		sscanf(tokens[1].c_str(), "%lf", &min_value);
		if(tokens[2] == ioconstants::PRICE_RANGE_SEPARATOR) {
			if(tokens.size() <= 4) {
				fprintf(stderr, "Error in line #%d\n", line);
				return false;
			}
			sscanf(tokens[3].c_str(), "%lf", &max_value);
			currency_code = tokens[4];
		}
		else {
			if(tokens.size() <= 2) {
				fprintf(stderr, "Error in line #%d\n", line);
				return false;
			}
			max_value = min_value;
			currency_code = tokens[2];
		}
		printf("Currency code: %s\n", currency_code.c_str());
		const Currency* currency = Currency::getCurrency(currency_code);
		if(currency == NULL) {
			fprintf(stderr, "Currency \"%s\" does not exist\n",
					currency_code.c_str());
			return false;
		}
		Price min_price = currency->getPrice(min_value),
				max_price = currency->getPrice(max_value);
		data.insert(make_pair(id, make_pair(min_price, max_price)));
	}
	fclose(f);
	return true;
}

bool GenIOUtils::savePriceRangeData(const std::string & filename,
		const PriceRangeData& data) {

	printf("%sSaving price ranges...\n", ioconstants::MSG_TAG);

	FILE *f = fopen(filename.c_str(), "w");
	FORC(it, data) {
		const Currency* currency = Currency::base_currency();
		if(currency == NULL) {
			fprintf(stderr, "Default currency does not exist\n");
			return false;
		}
		const PriceRange& range = it->second;
		double min_val = currency->getValue(range.first),
				max_val = currency->getValue(range.second);
		fprintf(f, "%d %lf %s %lf %s\n", it->first, min_val,
				ioconstants::PRICE_RANGE_SEPARATOR,
				max_val, currency->code().c_str());
	}

	fclose(f);
	return true;
}


