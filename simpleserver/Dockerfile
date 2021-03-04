FROM ocf/base
COPY --chown=ocf:ocf iotivity-lite/port/linux/simpleserver /iotivity-apps
RUN mkdir /iotivity-apps/simpleserver_creds && chown ocf:ocf /iotivity-apps/simpleserver_creds
USER ocf
WORKDIR /iotivity-apps
ENTRYPOINT ["/iotivity-apps/simpleserver"]
