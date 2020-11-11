#!/bin/bash


for dir in /home/manuel/Dropbox/tracking/cuboid_log_videos/data_1_failures/*/ 
do
    dir=${dir%*/}      # remove the trailing "/"
    rosrun trifinger_object_tracking single_observation $dir /home/manuel/Dropbox/tracking/cuboid_log_videos/output_1_failures
done