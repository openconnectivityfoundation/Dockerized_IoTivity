IOTIVITY_VERSION=2.2.0
EXAMPLE_IMAGE=ocfadmin/iotivity-examples
BUILD_IMAGE=ocfadmin/iotivity-builder
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool
IOTIVITY_BUILD_ARGS=

.PHONY: all examples dev cleaniotivity cleanimage cleanall
all: examples dev

examples: examples/Dockerfile $(BINARIES)
	docker build -t $(EXAMPLE_IMAGE):$(TAG) -f $< .

dev: build_environment/Dockerfile build_environment/build.sh
	docker build \
		--build-arg IOTIVITY_TAG=$(IOTIVITY_VERSION) \
		--build-arg IOTIVITY_BUILD_ARGS="$(IOTIVITY_BUILD_ARGS)" \
		-t $(BUILD_IMAGE):$(TAG) build_environment

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
	docker image prune -f

cleanall: cleaniotivity cleanimage

deinit: cleanall
	git submodule deinit -f --all
