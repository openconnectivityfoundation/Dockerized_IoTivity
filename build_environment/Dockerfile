FROM debian:buster-slim
ARG IOTIVITY_TAG
ARG IOTIVITY_BUILD_ARGS
RUN apt-get update && \
  apt-get install -y git build-essential && \
  rm -rf /var/lib/apt/lists/* && \
  git clone --branch $IOTIVITY_TAG --depth 1 https://gitlab.iotivity.org/iotivity/iotivity-lite.git /iotivity-lite
COPY ["build.sh", "/iotivity-lite/port/linux"]
WORKDIR /iotivity-lite/port/linux
RUN make $IOTIVITY_BUILD_ARGS libiotivity-lite-server.a libiotivity-lite-client.a libiotivity-lite-client-server.a && \
  ln -s /iotivity-app /iotivity-lite/port/linux/iotivity-app
VOLUME /iotivity-app
ENTRYPOINT ["/bin/sh", "build.sh"]
