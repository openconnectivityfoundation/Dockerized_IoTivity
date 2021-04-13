FROM alpine/git AS builder
WORKDIR /ocf_tooling
RUN git clone https://github.com/openconnectivityfoundation/DeviceBuilder.git && \
  git clone https://github.com/openconnectivityfoundation/IoTDataModels.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/core.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/core-extensions.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/certification.git --branch master && \
  git clone https://github.com/openconnectivityfoundation/swagger2x.git --branch master

FROM python:3.8-slim
COPY --from=builder /ocf_tooling /ocf_tooling
WORKDIR /ocf_tooling/DeviceBuilder
RUN pip3 install -U -r requirements.txt && mkdir /devbuilder && \
  ln -s /devbuilder /ocf_tooling/DeviceBuilder/mounted
VOLUME /devbuilder
COPY run.sh /ocf_tooling/DeviceBuilder
ENTRYPOINT ["/bin/sh", "./run.sh"]
