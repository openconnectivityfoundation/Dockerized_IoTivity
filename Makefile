DEBUG=1

.PHONY: base
base: base/Dockerfile
	docker build -t ocf/base -f base/Dockerfile .

.PHONY: simpleserver
simpleserver: base simpleserver/Dockerfile
	cd iotivity-lite/port/linux && \
	DEBUG=$(DEBUG) make simpleserver
	docker build -t ocf/simpleserver -f simpleserver/Dockerfile .
