FROM debian:buster-slim
RUN apt-get update && \
  apt-get install -y git build-essential && \
  rm -rf /var/lib/apt/lists/* && \
  git clone --branch 2.2.0 --depth 1 https://gitlab.iotivity.org/iotivity/iotivity-lite.git /iotivity-lite
COPY ["build.sh", "/iotivity-lite/port/linux"]
WORKDIR /iotivity-lite/port/linux
RUN make libiotivity-lite-server.a libiotivity-lite-client.a libiotivity-lite-client-server.a && \
  ln -s /iotivity-app /iotivity-lite/port/linux/iotivity-app
VOLUME /iotivity-app
ENTRYPOINT ["/bin/sh", "build.sh"]
