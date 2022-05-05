#!/bin/bash

DIR="$( cd "$( dirname "$0" )" && pwd )"

cd $DIR/../build

cmake -S $DIR/.. -B $DIR/../build

cd $DIR/../build; VERBOSE=1 make