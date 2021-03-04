DEBUG=1

.PHONY: base
base: base/Dockerfile
	cd base && \
	docker build -t ocf/base .

.PHONY: simpleserver
simpleserver: base simpleserver/Dockerfile
	cd iotivity-lite/port/linux && \
	DEBUG=$(DEBUG) make simpleserver
	docker build -t ocf/simpleserver -f simpleserver/Dockerfile .

.PHONY: simpleserver
simpleclient: base simpleclient/Dockerfile
	cd iotivity-lite/port/linux && \
	DEBUG=$(DEBUG) make simpleclient
	docker build -t ocf/simpleclient -f simpleclient/Dockerfile .
