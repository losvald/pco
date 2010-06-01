#!/bin/bash
data_dir=""
if [ $# -lt 4 ]; then
	echo "(suffix) (min-supply) (max-supply) (n-offers)";
fi
suffix=$1
sup_min=$2
sup_max=$3
n=$4
echo "Generating case with suffix: ${suffix}";
../Release/pco -a generate -c ${data_dir}currencies.txt -p ${data_dir}purchasables${suffix} -P ${data_dir}price-ranges${suffix} -s ${data_dir}sellers${suffix} -o ${data_dir}offers${suffix} -u ${data_dir}supplies${suffix} -m ${sup_min} -M ${sup_max} -n ${n}
