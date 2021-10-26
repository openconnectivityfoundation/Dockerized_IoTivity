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

CURDIR=$(shell pwd)
VOLUME=speaker_server
MAKE_TARGET=iotivity-app/speaker_server
DEV_IMAGE=ocfadmin/iotivity-builder
IOTIVITY_BUILD_ARGS=

.PHONY: volbuild volclean volcreate voldestroy build clean

# Build in a volume, which can be used in a runner image (see docker-compose.yml)
# Source code is copied into the volume
volbuild: volcreate iotivity-app/speaker_server.c iotivity-app/iotivity-app.mk
	docker create --rm --name iotivity-dev -v $(VOLUME):/iotivity-app $(DEV_IMAGE) "$(IOTIVITY_BUILD_ARGS)" $(MAKE_TARGET)
	cd iotivity-app && \
		docker cp . iotivity-dev:/iotivity-app
	docker start -a iotivity-dev

volclean: volcreate
	docker run --rm -v $(VOLUME):/iotivity-app $(DEV_IMAGE) cleanbuild

volcreate:
	docker volume create $(VOLUME)

voldestroy:
	docker volume rm $(VOLUME)

# Build and clean using a bind mount for the source directory; results in root permission on output files
build:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) "$(IOTIVITY_BUILD_ARGS)" $(MAKE_TARGET)

clean:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) cleanbuild
