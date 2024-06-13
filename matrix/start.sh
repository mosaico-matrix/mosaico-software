# Script to install all the dependencies and start the application on the pi

## Check for python installation
if ! [ -x "$(command -v python3)" ]; then
  echo 'Error: python3 is not installed.' >&2

  # Prompt user to install python3
  echo 'Do you want to install python3? (y/n)'
  read response
  if [ "$response" == "y" ]; then
    sudo apt-get install python3
  else
    echo 'Exiting script'
    exit 1
  fi
fi
echo 'Python3 is installed'

## Check for pip installation
if ! [ -x "$(command -v pip3)" ]; then
  echo 'Error: pip3 is not installed.' >&2

  # Prompt user to install pip3
  echo 'Do you want to install pip3? (y/n)'
  read response
  if [ "$response" == "y" ]; then
    sudo apt-get install python3-pip
  else
    echo 'Exiting script'
    exit 1
  fi
fi

## Create virtual env and install dependencies
#cd ../networking



## Compile the application
cmake .
make

echo 'Application compiled'

## Start the bluetooth service
#python3 ble-server.py &

## Start the application
./main
