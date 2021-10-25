FROM debian:buster-slim
RUN apt-get update && \
  apt-get install -y tini && \
  rm -rf /var/lib/apt/lists/* && \
  useradd -r ocf && \
  mkdir /iotivity-apps && chown ocf:ocf /iotivity-apps && \
  mkdir /creds && chown ocf:ocf /creds && \
  mkdir /creds/onboarding_tool_creds && chown ocf:ocf /creds/onboarding_tool_creds && \
  ln -s /creds/onboarding_tool_creds /iotivity-apps/onboarding_tool_creds && \
  mkdir /creds/simpleserver_creds && chown ocf:ocf /creds/simpleserver_creds && \
  ln -s /creds/simpleserver_creds /iotivity-apps/simpleserver_creds && \
  mkdir /creds/simpleclient_creds && chown ocf:ocf /creds/simpleclient_creds && \
  ln -s /creds/simpleclient_creds /iotivity-apps/simpleclient_creds
COPY --chown=ocf:ocf ["iotivity-lite/port/linux/onboarding_tool", "iotivity-lite/port/linux/simpleserver", "iotivity-lite/port/linux/simpleclient", "examples/run.sh", "/iotivity-apps/"]
WORKDIR /iotivity-apps
USER ocf
# Current IoTivity-Lite examples don't properly handle SIGTERM when running as PID 1
# tini is an init wrapper to handle this purpose
ENTRYPOINT ["tini", "-g", "--", "/iotivity-apps/run.sh"]
