FROM debian:buster-slim
RUN apt-get update && \
  apt-get install -y build-essential && \
  rm -rf /var/lib/apt/lists/*
COPY ["iotivity-lite", "/iotivity-lite"]
WORKDIR /iotivity-lite/port/linux/working-app
VOLUME /iotivity-lite/port/linux/working-app
ENTRYPOINT ["make"]
