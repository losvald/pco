#!/bin/bash
data_dir=""
suffix=$1
algo=bf
if [ $# -le 1 ]; then
	echo "(suffix) (algo)";
fi
if [ $# -ge 2 ]; then
	algo=$2
fi
echo "Running algorith ${algo} case with suffix: ${suffix}";
echo "Executing: ../Debug/pco -a solve -c ${data_dir}currencies.txt -p ${data_dir}purchasables${suffix} -s ${data_dir}sellers${suffix} -o ${data_dir}offers${suffix} -u ${data_dir}supplies${suffix} -r ${data_dir}requests${suffix} -g ${data_dir}parameters.txt -A $algo"
../Release/pco -a solve -c ${data_dir}currencies.txt -p ${data_dir}purchasables${suffix} -s ${data_dir}sellers${suffix} -o ${data_dir}offers${suffix} -u ${data_dir}supplies${suffix} -r ${data_dir}requests${suffix} -g ${data_dir}parameters.txt -A $algo
