CURDIR=$(shell pwd)
VOLUME=speaker_server
MAKE_TARGET=iotivity-app/speaker_server
DEV_IMAGE=ocfadmin/iotivity-builder
ENV_ARGS=

# Allows us to specify multiple build environment variables for IoTivity-Lite, such as "DEBUG=1 IPV4=1"
ifdef ENV_ARGS
	docker_envs := $(foreach arg, $(ENV_ARGS), $(addprefix --env ,$(arg)))
endif

.PHONY: build clean volbuild volume copysource

# Build in a volume, which can be used in a runner image (see docker-compose.yml)
# Source code is copied into the volume
volbuild: volume iotivity-app/speaker_server.c iotivity-app/iotivity-app.mk
	docker create --rm --name iotivity-dev $(docker_envs) -v $(VOLUME):/iotivity-app $(DEV_IMAGE) $(MAKE_TARGET)
	cd iotivity-app && \
		docker cp . iotivity-dev:/iotivity-app
	docker start -a iotivity-dev

volume:
	docker volume create $(VOLUME)

volclean:
	docker volume rm $(VOLUME)

# Build and clean using a bind mount for the source directory; results in root permission on output files
build:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) $(MAKE_TARGET)

clean:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) cleanbuild

