EXAMPLE_IMAGE=ocfadmin/iotivity-examples
DEVELOPMENT_IMAGE=ocfadmin/iotivity-development
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool
LIBRARIES=libiotivity-lite-server.a libiotivity-lite-client.a libiotivity-lite-client-server.a

.PHONY: all examples dev clean cleanimage cleanall
all: examples

examples: examples/Dockerfile $(BINARIES)
	docker build -t $(EXAMPLE_IMAGE):$(TAG) -f $< .

dev: development_environment/Dockerfile $(LIBRARIES)
	docker build -t $(DEVELOPMENT_IMAGE):$(TAG) -f $< .

iotivity-lite/.git:
	git submodule update --init $(@D)

$(BINARIES): iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $@

$(LIBRARIES): iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $@

clean:
	cd iotivity-lite/port/linux && \
	make cleanall

cleanimage:
	docker image rm -f $(EXAMPLE_IMAGE):$(TAG)
	docker image rm -f $(DEVELOPMENT_IMAGE):$(TAG)

cleanall: clean cleanimage
