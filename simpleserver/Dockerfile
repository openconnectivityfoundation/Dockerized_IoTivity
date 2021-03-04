FROM iotivity/base
COPY --chown=ocf:ocf iotivity-lite/port/linux/simpleserver /iotivity-apps
RUN mkdir /creds/simpleserver_creds && chown ocf:ocf /creds/simpleserver_creds && ln -s /creds/simpleserver_creds /iotivity-apps/simpleserver_creds
ENTRYPOINT ["/iotivity-apps/simpleserver"]
