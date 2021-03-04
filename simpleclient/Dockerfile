FROM iotivity/base
COPY --chown=ocf:ocf iotivity-lite/port/linux/simpleclient /iotivity-apps
RUN mkdir /creds/simpleclient_creds && chown ocf:ocf /creds/simpleclient_creds && ln -s /creds/simpleclient_creds /iotivity-apps/simpleclient_creds
ENTRYPOINT ["/iotivity-apps/simpleclient"]
