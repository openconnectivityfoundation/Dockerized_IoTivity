#!/bin/sh

usage() {
  echo "Must specify which example to run! Built-in (default) options are:\nsimpleserver\nsimpleclient\nonboarding_tool"
}

if [ $# -lt 1 ]
then
  usage
  exit 1
fi

EXECUTABLE="$(find -type f -executable -name $1)"

if [ -z "$EXECUTABLE" ]
then
  echo "Failed to find executable: $1"
  usage
  exit 1
fi

echo "Running $EXECUTABLE"
exec "$EXECUTABLE"
