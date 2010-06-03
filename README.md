# Purchasable Cost Optimizer #

PCO is a system that minimizes the cost of getting the sufficient
supplies of purchasables (products or services) by accepting *a set of
offers* from possibly *different sellers*.

The system is pretty generic, so it can a wide range of real-world
scenarios. An offer is basically a set of pairs `(purchaseable,
quantity)`, made by a seller for a *variable* `price` (depends on the
multiplicity of the accepted offer). Each seller has a set of offers
and a set of purchaseables in (un)limited *quantity*. Therefore, deals
like "Buy 1 get another 50% off" or "Get $30 off item1 when you buy 2x
item2", as well as wholesale deals are easily supported.

PCO uses a genetic algorithm to find a *multiset* of offers that yield
a near-optimal cost. For optimality comparison, a greedy and an exact
brute-force solver are also implemented.

## Installation ##
### Dependencies ###
- [ECF](http://gp.zemris.fer.hr/ecf/)
- [Boost](http://www.boost.org/)

For a convenient usage, you also need `bash`.  (On Windows, `cygwin`
would suffice.)

### Compilation ###
From the top-level directory, run

    make

to generate the binary executable file `pco`.

If you compile without using the makefile, make sure you add the
following link flags:

    -lecf
    -lboost_program_options-mt

For example,

    g++ *.cpp -o pco -O3 -lecf -lboost_program_options-mt -Wall -Wno-deprecated

## Usage ##

Please run the program with the `--help` for the list of supported options.

### Running the examples ###
You can run some examples with the following scripts in the `data` directory:

    example{1,2}-{bf,greedy,ga}.sh

where the suffix

* "bf" means run the exhaustive search (brute-force) algorithm
* "greedy" means run the greedy algorithm
* "ga" means run the genetic algorithm

e.g. `example2-ga.sh`.

### Custom input ###

    ../pco -c currencies.txt -p purchasables-example.1 -s sellers-example.1 \
	  -o offers-example.1 -u supplies-example.1 -r requests-example.1 \
	  -a solve -A ga -g parameters.txt

If your inputs file have the prefixes as in the command above
("purchasables", "sellers", etc.), you can use the wrapper script as
follows.

    ./run.sh -example.1 ga

### Tuning the genetic algorithm ###

The genetic algorithm parameters are set as in the ECF framework; the
`ga-params.txt` is automatically generated from the given parameters
file (scripts use `parameters.txt`).

*Note*: The Binary genotype's `dimension`, `lbound` and `rbound`
paremeters are optional; they will be automatically set at runtime.

You can set the upper limit on the total price with the `-l` option,
in which case solution candidates with higher price will be considered
infeasible by the genetic algorithm. By choosing it carefully, you
could improve the optimality of the algorithm. As a safe minimum, you
can use the output from the greedy algorithm. By default, this is a
huge number.

### Generating input files ###

    ../pco -a generate -c currencies.txt -p purchasables-example.3 \
	  -P price-ranges-example.3 -s sellers-example.3 \
	  -o offers-example.3 -u supplies-example.3 \
	  -m 100 -M 200 -n 5

The above command would generate the files: `sellers-example.3`,
`offers-example.3`, `supplies-example.3`, such that there exist 5
offers and at least one seller with supplies (product quantities)
between 100 and 200.  You can ensure deterministic output by setting
the random seed with the `-S` flag (otherwise, the seed is derived
from system time and stack addresses).

If the file names meet the requirement for running the `run.sh`, you
you above is equivalent to:

    ./gen.sh -example.3 100 200 5

Finally, there is a script to conveniently copy the input files.  For
example, to create a modified version of example 3, we could run

    ./copy.sh -example.3 -custom-example.3

and modify some of the files that end with `-custom-example.3`.
