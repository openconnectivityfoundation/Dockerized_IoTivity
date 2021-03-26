# The ocfadmin/iotivity-builder container assumes that this file will be named
# "iotivity-app.mk" and will be located in /iotivity-app (as a bind mount or a volume)
# This Makefile is referred to from the /iotivity-lite/port/linux directory, so
# it accordingly references relative paths

# To include iotivity-lite/port/linux/Makefile which has important variables defined
include Makefile

# Full target path in container will be /iotivity-lite/port/linux/iotivity-app
APP_TARGET=iotivity-app/speaker_server

$(APP_TARGET): iotivity-app/speaker_server.c libiotivity-lite-server.a
	@mkdir -p $@_creds
	${CC} -o $@ $< libiotivity-lite-server.a -DOC_SERVER ${CFLAGS} ${LIBS}

# Optional "clean" routine that doesn't override the ioitivity-lite clean
.PHONY: cleanbuild
cleanbuild:
	rm -rf $(APP_TARGET) $(APP_TARGET)_creds
