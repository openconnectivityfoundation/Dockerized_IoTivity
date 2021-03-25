EXAMPLE_IMAGE=ocfadmin/iotivity-examples
DEVELOPMENT_IMAGE=ocfadmin/iotivity-development
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool

.PHONY: all examples dev cleaniotivity cleanimage cleanall
all: examples

examples: examples/Dockerfile $(BINARIES)
	docker build -t $(EXAMPLE_IMAGE):$(TAG) -f $< .

dev: development_environment/Dockerfile
	docker build -t $(DEVELOPMENT_IMAGE):$(TAG) development_environment

iotivity-lite/.git:
	git submodule update --init $(@D)

$(BINARIES): iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $@

cleaniotivity:
	cd iotivity-lite/port/linux && \
	make cleanall

cleanimage:
	docker image rm -f $(EXAMPLE_IMAGE):$(TAG)
	docker image rm -f $(DEVELOPMENT_IMAGE):$(TAG)

cleanall: cleaniotivity cleanimage
