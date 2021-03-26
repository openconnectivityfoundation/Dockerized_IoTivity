#!/bin/bash
if [ -z "$MAKEFILE" ]; then
  MAKEFILE=iotivity-app.mk
fi

if [ -z "$*" ]; then
  echo "Specify a build target or \"all\" to build all"
  exit 1
fi

ln -s iotivity-app/iotivity-app.mk $MAKEFILE
make -f iotivity-app.mk $*
