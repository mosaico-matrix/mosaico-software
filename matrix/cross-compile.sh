#!/bin/bash

# Check if Raspberry Pi is online
if ! ping -c 1 pi > /dev/null; then
  echo "Raspberry is offline"
  exit 1
else
  echo "Raspberry is online"
fi

# Run database and make the whole project
cmake .
make -j

# Check if make fails
if [ $? -ne 0 ]; then
  exit $?
fi

# Sync files with the raspberry
./sync.sh