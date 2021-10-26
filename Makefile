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

IOTIVITY_VERSION=2.2.4.1
EXAMPLE_IMAGE=ocfadmin/iotivity-examples
BUILD_IMAGE=ocfadmin/iotivity-builder
DEVICEBUILDER_IMAGE=ocfadmin/devicebuilder
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool
IOTIVITY_BUILD_ARGS="IPV4=1"

.PHONY: all examples dev devicebuilder cleaniotivity cleanimage cleanall
all: examples dev devicebuilder

examples: examples/Dockerfile $(BINARIES) examples/run.sh
	docker build -t $(EXAMPLE_IMAGE):$(TAG) -f $< .

dev: build_environment/Dockerfile build_environment/build.sh
	docker build \
		--build-arg IOTIVITY_TAG=$(IOTIVITY_VERSION) \
		--build-arg IOTIVITY_BUILD_ARGS="$(IOTIVITY_BUILD_ARGS)" \
		-t $(BUILD_IMAGE):$(TAG) build_environment

devicebuilder: DeviceBuilder/Dockerfile
	cd DeviceBuilder && \
		docker build -t $(DEVICEBUILDER_IMAGE):$(TAG) .

iotivity-lite/.git:
	git submodule update --init $(@D)
	cd iotivity-lite && git checkout $(IOTIVITY_VERSION)

$(BINARIES): iotivity-lite/.git
	cd iotivity-lite/port/linux && \
		make $(IOTIVITY_BUILD_ARGS) $@

cleaniotivity:
	cd iotivity-lite/port/linux && \
		make cleanall

cleanimage:
	docker image rm -f $(EXAMPLE_IMAGE):$(TAG)
	docker image rm -f $(BUILD_IMAGE):$(TAG)
	docker image rm -f $(DEVICEBUILDER_IMAGE):$(TAG)
	docker image prune -f

cleanall: cleaniotivity cleanimage

deinit: cleanall
	git submodule deinit -f --all
