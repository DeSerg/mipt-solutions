#!/bin/sh

javac -cp /usr/lib/hadoop/hadoop-common.jar:/usr/lib/hadoop-mapreduce/hadoop-mapreduce-client-core.jar -d  ./build ./src/sergcode/WordCount.java && \
jar cf wc.jar -C ./build sergcode && \
hadoop jar wc.jar sergcode.WordCount /user/parallels/testTask/input /user/parallels/testTask/output
