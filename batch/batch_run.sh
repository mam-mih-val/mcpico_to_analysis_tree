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
echo "/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert -i $filelist -o output.root --output-tree atree -s $colliding_system -e $collision_energy --sample-reaction-plane --boost-to-lab --mcpico"

/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert \
                                                        -i $filelist \
                                                        -o output.root \
                                                        --output-tree atree \
                                                        -s $colliding_system \
                                                        -e $collision_energy \
                                                        --sample-reaction-plane \
                                                        --boost-to-lab \
                                                        --unigen

find . -name "output*.root" > list.txt
echo
date
echo "Running correction ..."
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
                                                                          -i list.txt \
                                                                          -t atree \
                                                                          -o correction_out.root \
                                                                          --yaml-config-file /scratch1/mmamaev/QnAnalysis/setups/bmatn/mc_corrections.yml \
                                                                          --yaml-config-name BMN
echo
date
echo "Running correlation ..."
/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrelate/QnAnalysisCorrelate \
                                                                              --input-file correction_out.root \
                                                                              --configuration-file /scratch1/mmamaev/QnAnalysis/setups/bmatn/mc_correlations.yml \
                                                                              --configuration-name _tasks \
                                                                              -o correlation_out.root


echo PROCESS FINISHED