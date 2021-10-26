#!/bin/bash

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

if [ -z "$MAKEFILE" ]; then
  MAKEFILE=iotivity-app.mk
fi

if [ -z "$*" ]; then
  echo "Specify a build target or \"all\" to build all"
  exit 1
fi

ln -s iotivity-app/iotivity-app.mk $MAKEFILE
make -f iotivity-app.mk $*
