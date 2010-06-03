/*
 * ga_selector.cpp
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
 * ga_selector.cpp
 *
 *  Created on: May 30, 2010
 *      Author: losvald
 */

#include "ga_selector.h"


using namespace std;

namespace ga_selector {

static const char* MSG_TAG = "[GA-SEL] ";

typedef string filename;

int state_argc;
char** state_argv = NULL;
const filename ga_filename = "ga-params.txt";

char** vector_to_valist (const std::vector<std::string>& vect)
{
	char** ret = new char*[2];
	for(int i = 0; i < 2; ++i)
		ret[i] = new char[500];
	strcpy(ret[0], vect[0].c_str());
	strcpy(ret[1], vect[1].c_str());
	return ret;

	/*
	std::auto_ptr<char*> valist (new char* [vect.size()]);
	std::transform (vect.begin(), vect.end(),
			const_cast<const char**> (valist.get()),
			std::mem_fun_ref (&std::string::c_str));
	return valist.release();
	*/
}

bool checkFilename(filename f){
	return f != "";
}

bool createStateArgs(filename argv0, filename param_file) {
	printf("Creating state args from(%s, %s)\n",
			argv0.c_str(), param_file.c_str());
	if(!checkFilename(param_file))
		return false;
//	if(state_argv != NULL) {
//		delete state_argv;
//	}
	vector<string> v;
	v.push_back(argv0);
	v.push_back(param_file);
	state_argc = v.size();
	state_argv = vector_to_valist(v);
	return true;
}

bool setParameters(int lbound, int ubound, int n, filename input_filename) {
	static const int LINE_LENGTH = 4096;
	static char buff[LINE_LENGTH];
	static const int N_PROPS = 3;
	static const char* LBOUND_PROP = "\"lbound\"";
	static const char* UBOUND_PROP = "\"ubound\"";
	static const char* DIMENSION_PROP = "\"dimension\"";
	static string props[] = {LBOUND_PROP, UBOUND_PROP, DIMENSION_PROP};
	static char prop_val[64];

	if(lbound > ubound) {
		cerr << "lbound > ubound" << endl;
		return false;
	}

	FILE *fin = fopen(input_filename.c_str(), "r");
	FILE *fout = fopen(ga_filename.c_str(), "w");

	if(fin == NULL) {
		cerr << "Wrong parameters file\n";
		return false;
	}
	while(fgets(buff, 1024, fin)) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0) continue;
		string line = buff;
		for(int p = 0; p < N_PROPS; ++p) {
			string prop = props[p];
			size_t ind = line.find(props[p]);
			int replace_from = 0, replace_cnt = 0;
			if(ind != string::npos) {
				replace_from = ind+prop.length()+1;
				for(int i = replace_from; line[i] != '<'; ++i)
					++replace_cnt;

				if(prop == LBOUND_PROP) {
					sprintf(prop_val, "%d", lbound);
				}
				else if(prop == UBOUND_PROP) {
					sprintf(prop_val, "%d", ubound);
				}
				else if(prop == DIMENSION_PROP) {
					sprintf(prop_val, "%d", n);
				}
				line.replace(replace_from, replace_cnt, prop_val);
			}
		}
		fprintf(fout, "%s", line.c_str());
	}

	fclose(fin);
	fclose(fout);
	return true;
}

void debugDimensions(StateP state) {
	vector<GenotypeP> genotypes = state->getGenotypes();
	for(uint i = 0; i < genotypes.size(); ++i) {
		printf("Size of genotype #%u: %u\n", genotypes[i]->getGenotypeId(),
				genotypes[i]->getGenomeSize());
	}
}

StateP init_ga(int dimension, int ubound, string argv0,
		filename input_param_file, GASelector& selector) {

	if(!setParameters(0, ubound, dimension, input_param_file)) {
		cerr << "Error setting parameters" << endl;
		return StateP();
	}

	printf("Done setting parameters\n");
	if(!createStateArgs(argv0, ga_filename)) {
		fprintf(stderr, "Failed to create arguments for GA state\n");
		return StateP();
	}
	printf("State arguments: %s %s\n", state_argv[0], state_argv[1]);

	StateP state = static_cast<StateP> (new State);

	// set the evaluation operator
	state->setEvalOp(static_cast<EvaluateOpP> (new GAFitnessEvalOp(selector,ubound) ));

	debugDimensions(state);

	if(!state->initialize(state_argc, state_argv))
		return StateP();

	return state;
}


} //namespace ga_selector

int GASelector::calcOfferBuyLimit() {
	int ret = 0;
	for(int i = 0; i < (int)offers_.size(); ++i) {
		const PurchasableOffer& offer = getOffer(i);
		ret = std::max(ret, offer.seller()->maxQuantity(offer.id()));
	}
	return ret;
}

GASelector::GASelector(Market& market,
		PurchasableData& purchasable_data,
		const string& param_file, const string& argv0,
		int buy_offer_limit)
: FlowBasedSelector(market, purchasable_data), in_progress_(false) {

	offers_ = market.getAllOffers();
	int n = offers_.size();

	int ubound = calcOfferBuyLimit(); //XXX po novom moze i bilo koji broj
	cout << ga_selector::MSG_TAG << "Offer buy limit: " << ubound << endl;
	if(buy_offer_limit > 0)
		ubound = buy_offer_limit+1;
	state_ = ga_selector::init_ga(n, ubound, argv0, param_file, *this);
	cout << ga_selector::MSG_TAG << "Constructed selector." << endl;
}

bool GASelector::start(
		const std::vector<BuyerRequest>& purchasable_requests,
		const Constraint<Seller>& seller_constraint,
		const Constraint<Selection>& selection_constraint) {
	in_progress_ = true;
	initStart(purchasable_requests, seller_constraint, selection_constraint);

	cout << ga_selector::MSG_TAG << "Running GA selection..." << endl;
	if(state_ == StateP()) {
		cerr << ga_selector::MSG_TAG << "Cannot start because state is invalid!\n";
		in_progress_ = false;
		return false;
	}
	bool status = state_->run();
	setSelection(best_selection());
	in_progress_ = false;
	return status && isCurrentSelectionFeasible();
}

bool GASelector::isCurrentSelectionFeasible() const {
	return FlowBasedSelector::isCurrentSelectionFeasible()
	&& selection_.getTotalPrice() <= worst_acceptable_price();
}

const PurchasableOffer& GASelector::getOffer(int n) const {
	return *offers_.at(n);
}

bool GASelector::isInProgress() const {
	return in_progress_;
}

bool GASelector::stop() {
	return false;
}
