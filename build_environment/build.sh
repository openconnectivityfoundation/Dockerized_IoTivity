#!/bin/bash

ln -s iotivity-app/iotivity-app.mk iotivity-app.mk
if [ -z "$*" ]; then
  echo "Specify a build target or \"all\" to build all"
  exit 1
fi
make -f iotivity-app.mk $*
