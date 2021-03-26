include Makefile

iotivity-app/speaker_server: iotivity-app/speaker_server.c libiotivity-lite-server.a
	@mkdir -p $@_creds
	${CC} -o $@ $< libiotivity-lite-server.a -DOC_SERVER ${CFLAGS} ${LIBS}

.PHONY: cleanbuild
cleanbuild:
	rm -rf iotivity-app/speaker_server iotivity-app/speaker_server_creds
