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

FROM debian:buster-slim
ARG IOTIVITY_TAG
ARG IOTIVITY_BUILD_ARGS
RUN apt-get update && \
  apt-get install -y git build-essential && \
  rm -rf /var/lib/apt/lists/* && \
  git clone --branch $IOTIVITY_TAG --depth 1 https://github.com/iotivity/iotivity-lite.git /iotivity-lite
COPY ["build.sh", "/iotivity-lite/port/linux"]
WORKDIR /iotivity-lite/port/linux
RUN make $IOTIVITY_BUILD_ARGS libiotivity-lite-server.a libiotivity-lite-client.a libiotivity-lite-client-server.a && \
  ln -s /iotivity-app /iotivity-lite/port/linux/iotivity-app
VOLUME /iotivity-app
ENTRYPOINT ["/bin/sh", "build.sh"]
