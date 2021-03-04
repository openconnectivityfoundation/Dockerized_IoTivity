FROM iotivity/base
COPY --chown=ocf:ocf iotivity-lite/port/linux/onboarding_tool /iotivity-apps
RUN mkdir /creds/onboarding_tool_creds && chown ocf:ocf /creds/onboarding_tool_creds && ln -s /creds/onboarding_tool_creds /iotivity-apps/onboarding_tool_creds
ENTRYPOINT ["/iotivity-apps/onboarding_tool"]
