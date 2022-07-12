#!/bin/bash

format='+%Y/%m/%d-%H:%M:%S'

date $format

job_num=$(($SLURM_ARRAY_TASK_ID))
filelist=$lists_dir/$(ls $lists_dir | sed "${job_num}q;d")

cd $output_dir
mkdir -p $job_num
cd $job_num

#source /mnt/pool/nica/7/mam2mih/soft/basov/fairsoft/install/bin/thisroot.sh
#echo
#date
#echo "Running conversion ..."
#echo "/mnt/pool/nica/7/mam2mih/soft/basov/mcpico_to_analysis_tree/build/convert -i $filelist -o output.root --output-tree atree -s $colliding_system -e $collision_energy --sample-reaction-plane --boost-to-lab --$data_format"
#
#/mnt/pool/nica/7/mam2mih/soft/basov/mcpico_to_analysis_tree/build/convert \
#                                                        -i $filelist \
#                                                        -o output.root \
#                                                        --output-tree atree \
#                                                        -s $colliding_system \
#                                                        -e $collision_energy \
#                                                        --sample-reaction-plane \
#                                                        --boost-to-lab \
#                                                        --$data_format
#
#find . -name "output*.root" > list.txt
#echo
#date
#echo "Running correction ..."
#/mnt/pool/nica/7/mam2mih/soft/basov/QnAnalysis/build/src/QnAnalysisCorrect/QnAnalysisCorrect \
#                                                                          -i list.txt \
#                                                                          -t atree \
#                                                                          -o correction_out.root \
#                                                                          --yaml-config-file /mnt/pool/nica/7/mam2mih/soft/basov/QnAnalysis/setups/bmatn/mc_corrections.yml \
#                                                                          --yaml-config-name BMN
echo
date
echo "Running correlation ..."
/mnt/pool/nica/7/mam2mih/soft/basov/QnAnalysis/build/src/QnAnalysisCorrelate/QnAnalysisCorrelate \
                                                                              --input-file correction_out.root \
                                                                              --configuration-file /mnt/pool/nica/7/mam2mih/soft/basov/QnAnalysis/setups/bmatn/mc_correlations.yml \
                                                                              --configuration-name _tasks \
                                                                              -o correlation_out.root


echo PROCESS FINISHED