#!/bin/bash
for i in {36..99..1}
  do
    rosrun trifinger_object_tracking single_observation ../data/cube_image_set_p4_2020-09-16/00$i
 done