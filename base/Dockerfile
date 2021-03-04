FROM debian:buster-slim
RUN useradd -r ocf && mkdir /iotivity-apps && chown ocf:ocf /iotivity-apps && mkdir /creds && chown ocf:ocf /creds
WORKDIR /iotivity-apps
USER ocf
