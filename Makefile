IMAGE=ocfadmin/iotivity-examples
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool

.PHONY: all examples clean cleanimage cleanall
all: examples

examples: examples/Dockerfile iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $(BINARIES)
	docker build -t $(IMAGE):$(TAG) -f $< .

iotivity-lite/.git:
	git submodule update --init $(@D)

clean:
	cd iotivity-lite/port/linux && \
	make cleanall

cleanimage:
	docker image rmi -f $(IMAGE):$(TAG)

cleanall: clean cleanimage
