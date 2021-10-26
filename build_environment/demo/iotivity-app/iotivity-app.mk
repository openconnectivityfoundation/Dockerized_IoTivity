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

### IoTivity Builder Container Makefile Example ###
# The ocfadmin/iotivity-builder container assumes that this file will be named
# "iotivity-app.mk" and will be located in /iotivity-app (as a bind mount or a volume)
# This Makefile is referred to from the /iotivity-lite/port/linux directory, so
# it accordingly references relative paths

# To include iotivity-lite/port/linux/Makefile which has important variables defined
include Makefile

# Full target path in container will be /iotivity-lite/port/linux/iotivity-app
APP_TARGET=iotivity-app/speaker_server

$(APP_TARGET): iotivity-app/speaker_server.c libiotivity-lite-server.a
	@mkdir -p $@_creds
	${CC} -o $@ $< libiotivity-lite-server.a -DOC_SERVER ${CFLAGS} ${LIBS}

# Optional "clean" routine that doesn't override the ioitivity-lite clean
.PHONY: cleanbuild
cleanbuild:
	rm -rf $(APP_TARGET) $(APP_TARGET)_creds
