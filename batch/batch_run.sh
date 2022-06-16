#!/bin/bash
#$ -l h=!(ncx182.jinr.ru|ncx211.jinr.ru)

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SGE_TASK_ID))
filelist=$lists_dir/$(ls $lists_dir | sed "${job_num}q;d")

cd $output_dir
mkdir -p $job_num
cd $job_num

source /cvmfs/nica.jinr.ru/sw/os/login.sh
module add GCC-Toolchain/

source /scratch1/mmamaev/fairsoft/install/bin/thisroot.sh
echo
date
echo "Running conversion ..."
echo "/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert -i $filelist --input-tree events -o output.root --output-tree atree -s $colliding_system -e $collision_energy --sample-reaction-plane --boost-to-lab --unigen"
/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert \
                                                        -i $filelist \
                                                        --input-tree events \
                                                        -o output.root \
                                                        --output-tree atree \
                                                        -s $colliding_system \
                                                        -e $collision_energy \
                                                        --sample-reaction-plane \
                                                        --boost-to-lab \
                                                        --unigen

echo PROCESS FINISHED