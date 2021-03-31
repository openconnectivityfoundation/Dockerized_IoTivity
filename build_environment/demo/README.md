# Example Use of the `ocfadmin/iotivity-builder` Image

## Introduction

The files in this directory provide an example application that can be built
with the `ocfocfadmin/iotivity-builder` image. All relevant commands are called
in the [Makefile](./Makefile). Additionally, the resulting application can be
demoed with the [`docker-compose`](./docker-compose.yml) file.

## Source Files

The example application used in this demo is a speaker server generated with
[OCF DeviceBuilder](https://github.com/openconnectivityfoundation/DeviceBuilder).
Specifically, the input JSON file to generate `speaker_server.c` is as follows:

```json
[
  {
    "path": "/speaker",
    "rt": [ "oic.r.switch.binary", "oic.r.audio" ],
    "if": [ "oic.if.a", "oic.if.baseline" ]
  }
]
```

The associated Makefile, `iotivity-app.mk` is used within the builder image, per
the image [README](../README.md).

## Building

The primary [Makefile](./Makefile) performs a few different operations. It can
be used to build the application using the builder image with either a volume or
a bind mount.

To build the application with a volume, simply use `make`. Leveraging a volume
is the recommended way to build this example, as it can be used to run the demo
in a separate container.

To build the application with a bind mount, use `make build`.

The Makefile is also configured to allow specifying different IoTivity-Lite
build environment variables and/or application targets like so:

```bash
$ make IOTIVITY_BUILD_ARGS="DEBUG=1 IPV4=1" MAKE_TARGET="cleanall iotivity-app/speaker_server"
```

## Running Example (Built with Volume)

Additionally, this directory contains a [`docker-compose`](./docker-compose.yml)
file that can be used to spin up a quick instance of `speaker_server` after
building. This assumes the volume-based build process. A full execution of the
demo would look like the following:

```bash
# Build with builder image
$ make

# Spin up demo (OBT and speaker_server)
$ docker-compose up -d

# Observe initial output of speaker_server
$ docker logs speaker_server

# Take control of the OBT to discover and onboard the new app
$ docker attach obt
```

For more general instructions and requirements on the use of the
`ocfadmin/iotivity-examples` image (which is used as this runner), refer to the
image's [README](/examples/README.md).
