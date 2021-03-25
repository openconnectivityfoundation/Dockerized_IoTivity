FROM debian:buster-slim
RUN apt-get update && \
  apt-get install -y git build-essential && \
  rm -rf /var/lib/apt/lists/*
COPY ["iotivity-lite", "/iotivity-lite"]
COPY ["development_environment/build.sh", "/iotivity-lite/port/linux"]
RUN ln -s /iotivity-app /iotivity-lite/port/linux/iotivity-app
WORKDIR /iotivity-lite/port/linux
VOLUME /iotivity-app
ENTRYPOINT ["/bin/sh", "build.sh"]
