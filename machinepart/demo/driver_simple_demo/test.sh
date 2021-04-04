#!/bin/bash

DEVICE_PATH="/dev/test"

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

echo ======== test simple driver ========
DATA="some data"

echo ======== write operation ===========
echo INFO: write \"$DATA\" to \"$DEVICE_PATH\"
echo $DATA > $DEVICE_PATH

echo ======== read operation ============
RESULT=$(cat $DEVICE_PATH)
echo INFO: read from \"$DEVICE_PATH\", result: \"$RESULT\"

echo ======== compare result ============
echo DATA: $DATA
echo RESULT: $RESULT
if [[ $DATA == $RESULT ]]
then
  echo === PASSED ===
else
  echo === FAILED ===
fi

