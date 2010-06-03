#!/bin/bash
data_dir=""

if [ $# -le 1 ]; then
	echo "(suffix_from)(suffix_to)";
fi
suffix=$1
suffix2=$2
cp ${data_dir}purchasables${suffix} ${data_dir}purchasables${suffix2}
cp ${data_dir}sellers${suffix} ${data_dir}sellers${suffix2}
cp ${data_dir}offers${suffix} ${data_dir}offers${suffix2}
cp ${data_dir}supplies${suffix} ${data_dir}supplies${suffix2}
cp ${data_dir}requests${suffix} ${data_dir}requests${suffix2}
cp ${data_dir}price-ranges${suffix} ${data_dir}price-ranges${suffix2}
