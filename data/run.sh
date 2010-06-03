#!/bin/bash
data_dir=""
suffix=$1
algo=bf
limit=100000000000000000
if [ $# -le 2 ]; then
	echo "(suffix) (algo) (price-limit)";
fi
if [ $# -ge 2 ]; then
	algo=$2
fi
if [ $# -ge 3 ]; then
	limit=$3;
fi
if [ $# -lt 2 ]; then
	exit 1;
fi

echo "Running algorithm ${algo} on case with suffix: ${suffix} with price limit ${limit}";
echo "Executing: ../pco -a solve -c ${data_dir}currencies.txt -p ${data_dir}purchasables${suffix} -s ${data_dir}sellers${suffix} -o ${data_dir}offers${suffix} -u ${data_dir}supplies${suffix} -r ${data_dir}requests${suffix} -g ${data_dir}parameters.txt -A $algo -l ${limit}"
../pco -a solve -c ${data_dir}currencies.txt -p ${data_dir}purchasables${suffix} -s ${data_dir}sellers${suffix} -o ${data_dir}offers${suffix} -u ${data_dir}supplies${suffix} -r ${data_dir}requests${suffix} -g ${data_dir}parameters.txt -A $algo -l ${limit}
