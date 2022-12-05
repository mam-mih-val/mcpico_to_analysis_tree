#!/bin/bash
#$ -l h=!(ncx111.jinr.ru|ncx113.jinr.ru|ncx116.jinr.ru|ncx123.jinr.ru|ncx143.jinr.ru|ncx149.jinr.ru|ncx152.jinr.ru|ncx172.jinr.ru|ncx205.jinr.ru|ncx223.jinr.ru|ncx231.jinr.ru)

format='+%Y/%m/%d-%H:%M:%S'

date $format
echo "JOB IS RUNNING on $HOSTNAME"

job_num=$(($SGE_TASK_ID))
filelist=$lists_dir/$(ls $lists_dir | sed "${job_num}q;d")

cd $output_dir
mkdir -p $job_num
cd $job_num

source /cvmfs/nica.jinr.ru/sw/os/login.sh
module add GCC-Toolchain/

source /scratch1/mmamaev/bmn_environment.sh
echo
date
echo "Running conversion ..."
echo "/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert -i $filelist -o output.root --output-tree atree -s $colliding_system -e $collision_energy --sample-reaction-plane --boost-to-lab --$data_format"

/scratch1/mmamaev/mcpico_to_analysis_tree/build/convert \
                                                        -i $filelist \
                                                        -o output.root \
                                                        --output-tree atree \
                                                        -s $colliding_system \
                                                        -e $collision_energy \
                                                        --sample-reaction-plane \
                                                        --boost-to-lab \
                                                        --$data_format

find . -name "output*.root" > list.txt
echo; date
echo "Running QA step ..."
input_list=\"list.txt\"
output_qa=\"qa.root\"
root -q "/scratch1/mmamaev/mcpico_to_analysis_tree/macro/run_qa.cc($input_list, $output_qa)"
echo; date
#echo "Running correction ..."
#/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
#                                                                          -i list.txt \
#                                                                          -t atree \
#                                                                          -o correction_out.root \
#                                                                          --yaml-config-file /scratch1/mmamaev/mcpico_to_analysis_tree/qnanalysis/mc_corrections.yml \
#                                                                          --yaml-config-name BMN
#echo; date
#echo "Running correlation ..."
#/scratch1/mmamaev/QnAnalysis/build/src/QnAnalysisCorrelate/QnAnalysisCorrelate \
#                                                                              --input-file correction_out.root \
#                                                                              --configuration-file /scratch1/mmamaev/mcpico_to_analysis_tree/qnanalysis/mc_correlations.yml \
#                                                                              --configuration-name _tasks \
#                                                                              -o correlation_out.root

rm output*.root list.txt correction_out.root

echo PROCESS FINISHED