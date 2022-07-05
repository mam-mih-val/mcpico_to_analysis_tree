#!/bin/bash

file_list=$1
output_dir=$2
n_files=$3
colliding_system=$4
collision_energy=$5
data_format=$6


time=14:00:00
partition=cpu

output_dir=${output_dir}/
lists_dir=${output_dir}/lists/
log_dir=${output_dir}/log/

mkdir -p $output_dir
mkdir -p $log_dir
mkdir -p $lists_dir

split -l $n_files -d -a 3 --additional-suffix=.list "$file_list" $lists_dir

n_runs=$(ls $lists_dir/*.list | wc -l)

job_range=1-$n_runs

echo file list: $file_list
echo output_dir: $output_dir
echo log_dir: $log_dir
echo lists_dir: $lists_dir
echo n_runs: $n_runs
echo job_range: $job_range

sbatch  -J QnAnalysis \
      -t $time \
      -p $partition \
      -a $job_range \
      -e ${log_dir}/%A_%a.e \
      -o ${log_dir}/%A_%a.o \
      --export=output_dir=$output_dir,file_list=$file_list,lists_dir=$lists_dir,colliding_system=$colliding_system,collision_energy=$collision_energy,data_format=$data_format \
      -- /mnt/pool/nica/7/mam2mih/soft/basov/mcpico_to_analysis_tree/batch/batch_run.sh

echo JOBS HAVE BEEN SUBMITTED!
