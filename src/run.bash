#!/bin/bash
for i in {10..32..1}
  do
    rosrun trifinger_object_tracking single_observation ../data/cube_image_set_trifingerone/00$i
 done