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

FROM alpine/git AS builder
WORKDIR /ocf_tooling
RUN git clone https://github.com/openconnectivityfoundation/DeviceBuilder.git && \
  git clone https://github.com/openconnectivityfoundation/IoTDataModels.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/core.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/core-extensions.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/certification.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/swagger2x.git --branch master

FROM python:3.8-slim
COPY --from=builder /ocf_tooling /ocf_tooling
WORKDIR /ocf_tooling/DeviceBuilder
RUN pip3 install -U -r requirements.txt && mkdir /devbuilder && \
  ln -s /devbuilder /ocf_tooling/DeviceBuilder/mounted
VOLUME /devbuilder
COPY run.sh /ocf_tooling/DeviceBuilder
ENTRYPOINT ["/bin/sh", "./run.sh"]
