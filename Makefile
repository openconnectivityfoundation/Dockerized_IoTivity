IMAGE=iotivity/examples

.PHONY: all examples clean cleanall
all: examples

examples: examples/Dockerfile
	cd iotivity-lite/port/linux && \
	make simpleserver simpleclient onboarding_tool
	docker build -t $(IMAGE) -f $< .

clean:
	cd iotivity-lite/port/linux && \
	make cleanall

cleanall: clean
	docker image rmi -f $(IMAGE)
