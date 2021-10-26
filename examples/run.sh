#!/bin/sh

# Copyright 2021 Open Connectivity Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
