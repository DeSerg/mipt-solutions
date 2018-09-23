#!/bin/bash

function check {
  if [ $? -ne 0 ]
  then
    echo "error"
    exit 1
  fi
}

mkisofs -R -o $1 $2 >/dev/null 2>&1
check

chown $3 $1 && chgrp $4 $1 >/dev/null 2>&1
check

echo "done"

