#!/bin/sh

hadoop jar /usr/lib/hadoop-mapreduce/hadoop-streaming.jar \
-input /user/parallels/streaming/input \
-output /user/parallels/streaming/output \
-mapper ./map.py \
-reducer ./reduce.py \
-file /usr/lib/hadoop−mapreduce/hadoop−streaming.jar \
-file ./reducer.py \
-file ./map.py \
