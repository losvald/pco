/*
 * main.cpp
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
 * Main.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

//ga includes
#include <ecf/ECF.h>

#include "utils.h"
#include "base_types.h"
#include "market.h"
#include "purchasable_data.h"
#include "entity_io_utils.h"
#include "string_utils.h"
#include "ga_solver.h"
#include "greedy_solver.h"
#include "brute_force_solver.h"

//gen include
#include "generator.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Market market;

#include "string_utils.h"


#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


#include <string>
#include <iostream>
using namespace std;

namespace po = boost::program_options;
typedef string filename;

bool checkFilename(filename f){
	return f != "";
}

/*
void printVarMap( po::variables_map varMap ){
	cout << "varmap:\n";
	for( po::variables_map::iterator it = varMap.begin(); it != varMap.end(); ++it){
		cout << it->first << " " << it-> second.as<string>() << "\n";
	}
}
*/


bool loadFiles(filename currencies, filename purchasables,
		filename sellers, filename offers, filename supplies,
		filename requests) {
	if(!EntityIOUtils::loadExchangeRates(currencies)) {
		cerr << "Error loading currencies" << endl;
		return false;
	}
	if(!EntityIOUtils::loadPurchasables(PurchasableData::instance(), purchasables)) {
		cerr << "Error loading purchasables" << endl;
		return false;
	}
	if(!EntityIOUtils::loadSellers(market, sellers)) {
		cerr << "Error loading sellers" << endl;
		return false;
	}
	if(!EntityIOUtils::loadOffers(market, offers)) {
		cerr << "Error loading offers" << endl;
		return false;
	}
	if(!EntityIOUtils::loadSupplies(market, supplies)) {
		cerr << "Error loading supplies" << endl;
		return false;
	}
	return true;
}

bool runGA(string argv0, filename ga_param_file, filename requests_file,
		Price price_limit) {
	return ga::solve(market, requests_file, ga_param_file, argv0, price_limit);
}

void parseOptions( int ac, char** av ) {
	string action="gensolve", algo;
	filename currencies, purchasables, supplies, sellers, offers, requests, priceRanges;
	filename ga_param;
	Price price_limit = -1;
	int seed=-1, numOffers, supplyMin=-1, supplyMax=-1;

	po::options_description desc("Available options");
	desc.add_options()
		("help,h", "print help message")
		("action,a"           , po::value<string>(&action), "desired program action (generate | solve | gensolve ) ")

		("currencies-file,c"  , po::value<filename>(&currencies)              , "currencies input file")
		("purchasables-file,p", po::value<filename>(&purchasables)            , "purchasables input file")
		("supplies-file,u"    , po::value<filename>(&supplies)                , "supplies input file")
		("sellers-file,s"     , po::value<filename>(&sellers)                 , "sellers input file")
		("requests-file,r"    , po::value<filename>(&requests)                , "requests input file")
		("offers-file,o"	  , po::value<filename>(&offers)                  , "offers input file")
		("ga-param-file,g"	  , po::value<filename>(&ga_param)                , "ga param input file")
		("price-limit,l"      , po::value<Price>(&price_limit)                   , "limit")

		("price-ranges-file,P", po::value<filename>(&priceRanges)             , "price ranges output file if generating")
		("seed,S"               , po::value<int>(&seed)                       , "seed number")
		("num-offers,n"       , po::value<int>(&numOffers)                    , "number of offers")
		("supply-min,m"       , po::value<int>(&supplyMin)                    , "supply minimum value")
		("supply-max,M"       , po::value<int>(&supplyMax)                    , "supply maximum value")

		("algorithm,A"        , po::value<string>(&algo)                      , "algorithm to use for solving")
	;

	po::variables_map varMap;
	try{
		po::store( po::parse_command_line(ac, av, desc) , varMap);
	}
	catch( po::error e ) { cerr << e.what() << endl; exit(1); }

	po::notify(varMap);

	//printVarMap(varMap);
	if(varMap.count("help") ){
		cerr << desc << endl;
		exit(1);
	}
	if(varMap.count("action") == 0)
		cerr << "Doing default action (= " << action << ")\n";

	if(action != "generate" && action != "gensolve" && action != "solve"){
		cerr << "Unknown action " << action <<"\n";
		exit(1);
	}

	if(action == "generate" || action =="gensolve") {
		if( !checkFilename(currencies) ){
			cerr << "Unspecified currencies file\n";
			exit(1);
		}
		if( !checkFilename(purchasables) ){
			cerr << "Unspecified purchasables file\n";
			exit(1);
		}
		if( !checkFilename(supplies) ) {
			cerr << "Unspecified supplies file\n";
			exit(1);
		}
		if( !checkFilename(priceRanges) ) {
			cerr << "Unspecified price-ranges file\n";
			exit(1);
		}
		if( !checkFilename(offers) ){
			cerr << "Unspecified offers file\n";
			exit(1);
		}
		if( supplyMin == -1 || supplyMax == -1){
			cerr << "Unspecified supply min/max \n";
			exit(1);
		}
		if( supplyMin > supplyMax){
			cerr << "Invalid supply range\n";
			exit(1);
		}
		if( numOffers <= 0 ) {
			cerr << "Invalid number of offers\n";
			exit(1);
		}
		if( !varMap.count("num-offers")){
			cerr << "Unspecified number of offers\n";
			exit(1);
		}

		cerr << "Generating ... \n";
		if(seed == -1) {
			seed = RandomUtils::new_seed_val();
		}
		if(!generate(market, currencies, purchasables,
				priceRanges, sellers,
				offers, supplies,
				supplyMin, supplyMax,
				numOffers, seed)) {
			cerr << "Generation FAILED" << endl;
			exit(1);
		}
	}
	if(action == "gensolve"){
		if( varMap.count("algorithm")==0 ){
			cerr << "Unspecified algorithm\n";
			exit(1);
		}
		if( algo == "greedy" ) {
			cerr << "Running greedy\n"; //TODO
		}
		else if( algo == "ga" )
			cerr << "Running ga\n"; // TODO
		else if( algo == "bf" )
			cerr << "Running bf\n"; // TODO
		else {
			cerr << "Invalid algorithm specified\n";
			exit(1);
		}
	}
	if(action == "solve"){
		if( !checkFilename(currencies) ){
			cerr << "Unspecified currencies file\n";
			exit(1);
		}
		if( !checkFilename(purchasables) ){
			cerr << "Unspecified purchasables file\n";
			exit(1);
		}

		if( !checkFilename(supplies) ){
			cerr << "Unspecified supplies file\n";
			exit(1);
		}

		if( !checkFilename(sellers) ){
			cerr << "Unspecified sellers file\n";
			exit(1);
		}
		if( !checkFilename(requests) ){
			cerr << "Unspecified requests file\n";
			exit(1);
		}
		if( !checkFilename(offers) ){
			cerr << "Unspecified offers file\n";
			exit(1);
		}

		if( varMap.count("algorithm")==0 ){
			cerr << "Unspecified algorithm\n";
			exit(1);
		}
		if( algo == "ga" && !checkFilename(ga_param)) {
			cerr << "Unspecified GA parameters file\n";
			exit(1);
		}


		if( algo != "greedy" && algo != "greedy-fast" && algo != "ga"
				&& algo != "bf" && algo != "manual") {
			cerr << "Invalid algorithm specified\n";
			exit(1);
		}

		if(!loadFiles(currencies, purchasables, sellers, offers, supplies, requests)) {
			cerr << "Failed to load input files" << endl;
			exit(1);
		}
		if(price_limit == -1)
			price_limit = infinitePrice;

		if( algo == "greedy" ) {
			cerr << "Running greedy\n";
			greedy::solve2(market, requests, price_limit);
		}
		else if( algo == "greedy-fast" ) {
			cerr << "Running greedy (fast version)\n";
			greedy::solve(market, requests, price_limit);
		}
		else if( algo == "ga" ) {
			cerr << "Running ga\n";
			runGA(av[0], ga_param, requests, price_limit);
		}
		else if( algo == "bf" ) {
			cerr << "Running bf\n";
			brute_force::solve(market, requests, price_limit);
		}
		else if( algo == "manual" ) {
			cerr << "Running manual selection\n";
			test::testManualSelector(market, requests);
		}

	}
}

int main(int argc, char **argv) {
	parseOptions(argc, argv);

	printf("MAIN EXITED NORMALLY\n");
	return 0;
}
