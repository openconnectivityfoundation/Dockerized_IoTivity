#!/bin/sh

usage() {
  echo "Must specify which example to run! Options are:\nsimpleserver\nsimpleclient\nonboarding_tool"
}

if [ $# -lt 1 ]
then
  usage
  exit 1
fi

if [ "$1" != "simpleserver" -a "$1" != "simpleclient" -a "$1" != "onboarding_tool" ]
then
  echo "Unknown executable: $1"
  usage
  exit 1
fi

echo "Running $1"
exec "./$1"
