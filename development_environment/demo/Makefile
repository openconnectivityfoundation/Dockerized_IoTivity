CURDIR=$(shell pwd)
VOLUME=speaker_server
MAKE_TARGET=iotivity-app/speaker_server
DEV_IMAGE=ocfadmin/iotivity-builder

.PHONY: build clean volbuild volume copysource

build:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) $(MAKE_TARGET)

clean:
	docker run --rm -v $(CURDIR)/iotivity-app:/iotivity-app $(DEV_IMAGE) cleanbuild

volbuild: volume iotivity-app/speaker_server.c iotivity-app/iotivity-app.mk
	docker create --rm --name iotivity-dev -v $(VOLUME):/iotivity-app $(DEV_IMAGE) $(MAKE_TARGET)
	cd iotivity-app && \
		docker cp . iotivity-dev:/iotivity-app
	docker start -a iotivity-dev

volume:
	docker volume create $(VOLUME)

volclean:
	docker volume rm $(VOLUME)
