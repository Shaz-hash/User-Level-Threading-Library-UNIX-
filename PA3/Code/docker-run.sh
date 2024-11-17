#/bin/bash

PWD=$(pwd)
CMD="docker run -it --rm -v $PWD:/home/os-fall-2023 os-fall-2023:latest"

echo "Creating a container and attaching it to current shell"
$CMD