CURDIR=$(shell pwd)
VOLUME=speaker_server
MAKE_TARGET=iotivity-app/speaker_server
DEV_IMAGE=ocfadmin/iotivity-builder
IOTIVITY_BUILD_ARGS=

.PHONY: volbuild volclean volcreate voldestroy build clean

# Build in a volume, which can be used in a runner image (see docker-compose.yml)
# Source code is copied into the volume
volbuild: volcreate iotivity-app/speaker_server.c iotivity-app/iotivity-app.mk
	docker create --rm --name iotivity-dev $(docker_envs) -v $(VOLUME):/iotivity-app $(DEV_IMAGE) "$(IOTIVITY_BUILD_ARGS)" $(MAKE_TARGET)
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
