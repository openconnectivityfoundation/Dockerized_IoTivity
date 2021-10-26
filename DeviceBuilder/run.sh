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

OUTPUT_DIR=/ocf_tooling/DeviceBuilder/mounted/output
INPUT_FILE=$1
DEVICE_TYPE=$2
TITLE=$3
mkdir -p $OUTPUT_DIR
echo "DeviceBuilder IoTivity-Lite Server arguments:"
echo "$1 $OUTPUT_DIR $2"

usage() {
  echo "devicebuilder container arguments usage:"
  echo "<input filename without path> <device type> [title]"
}

if [ ! -f mounted/$INPUT_FILE ]
then
  echo "Input file $INPUT_FILE does not exist."
  usage
  exit 1
fi

if [ -z "$DEVICE_TYPE" ]
then
  echo "Please specify a device type."
  usage
  exit 1
fi

sh DeviceBuilder_IotivityLiteServer.sh mounted/$INPUT_FILE $OUTPUT_DIR $DEVICE_TYPE $TITLE
