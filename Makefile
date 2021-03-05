IMAGE=iotivity/examples
BINARIES=simpleserver simpleclient onboarding_tool

.PHONY: all examples clean cleanall
all: examples

examples: examples/Dockerfile iotivity-lite/.git
	cd iotivity-lite/port/linux && \
	make $(BINARIES)
	docker build -t $(IMAGE) -f $< .

iotivity-lite/.git:
	git submodule update --init $(@D)

clean:
	cd iotivity-lite/port/linux && \
	make cleanall

cleanall: clean
	docker image rmi -f $(IMAGE)
