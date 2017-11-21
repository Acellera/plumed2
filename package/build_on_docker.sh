#! /bin/bash

# NOTE: This script has to be run from the root of Acellera/plumed.git

set -e

IMAGE=image4plumed2
CONDA_PATH=/opt/miniconda3

# Build an image
docker build --tag $IMAGE docker

# Create and start a container
ID=$(docker create --rm $IMAGE)
docker start $ID

# Copy the repository into the container
docker cp . $ID:/tmp/conda-plumed2.git

# Build a package
docker exec --tty $ID $CONDA_PATH/bin/conda build package

# Copy the built package from the container
docker cp $ID:$CONDA_PATH/conda-bld/linux-64/ .

# Stop the container
docker stop $ID
