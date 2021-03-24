BASE_IMAGE=iotivity-base
EXAMPLE_IMAGE=ocfadmin/iotivity-examples
TAG=latest
BINARIES=simpleserver simpleclient onboarding_tool

.PHONY: all base examples clean cleanimage cleanall
all: examples

examples: examples/Dockerfile base $(BINARIES)
	docker build -t $(EXAMPLE_IMAGE):$(TAG) -f $< .

base: base/Dockerfile
	cd base && \
	docker build -t $(BASE_IMAGE) .

iotivity-lite/.git:
	git submodule update --init $(@D)

$(BINARIES): iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $@

clean:
	cd iotivity-lite/port/linux && \
	make cleanall
	docker image rm -f $(BASE_IMAGE)

cleanimage:
	docker image rm -f $(EXAMPLE_IMAGE):$(TAG)

cleanall: clean cleanimage
