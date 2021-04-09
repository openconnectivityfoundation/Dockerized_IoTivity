# IoTivity-Lite Builder Image

## Introduction

The `ocfadmin/iotivity-builder` image consists of the IoTivity-Lite code base
and necessary tools to compile IoTivity applications for Linux-based
distributions. This image is intended to be used for one-off compilation of
applications. Because of the transient nature of container filesystems, this
container image should be used with bind mounts or volumes to access source
files on the host.

## Architecture

The builder image is a Debian image that contains the IoTivity-Lite code base as
well as the tooling required to compile IoTivity applications. The primary
directories of interest include:

* `/iotivity-lite`: The IoTivity-Lite codebase
* `/iotivity-app`: An empty directory intended for use with a bind mount or
  volume
  * A symlink located at `/iotivity-lite/port/linux/iotivity-app` points to this
    directory; this is where application source is expected to reside

The default working directory of this image is `/iotivity-lite/port/linux`,
where the Linux-specific IoTivity-Lite build definitions are located (and
leveraged during the build process). Additionally, the client and server
IoTivity libraries (e.g., `libiotivity-lite-server.a`) are pre-compiled (with
default IoTivity-Lite settings) and included as part of the image.

## General Use

For an example use of the image as part of a build process, refer to the
[demo](./demo) directory. This document describes the use of the image for
building processes with example terminologies of that demo, and source files
generated with [OCF DeviceBuilder](https://github.com/openconnectivityfoundation/DeviceBuilder).

### Source Files

Generally, application source files should be located in a directory that can be
copied to a volume or used as a bind mount. Additionally, this directory should
include a Makefile that contains directives to compile the application. This
source code may import and leverage IoTivity API calls; for more information on
building IoTivity-Lite applications, refer to the [IoTivity documentation](https://iotivity.org/documentation/building-iotivity-linux).

#### Makefile

The source directory should include a Makefile that provides the recipe/rule for
compiling your source files. At its simplest, this might look like the
following:

```Makefile
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
```

Note that this makefile may not reflect the directory structure of the host, but
rather the directory structure of the build container, where the main source
directory is located at `/iotivity-app` (symlinked as
`/iotivity-lite/port/linux/iotivity-app`).

The image will assume that this Makefile is named `iotivity-app.mk`, unless
other specified with an `--env` argument (see below).

### Running the Image

With this structure in mind, the simplest execution of the image to build an
application could look like the following:

```bash
$ docker run --rm --env MAKEFILE=iotivity-app.mk -v /path/to/the/app:/iotivity-app ocfadmin/iotivity-builder <APP_TARGET>
```

This command would mount `/path/to/the/app` as a bind mount to the container and
execute a `make` with the specified target(s) as the final argument. Unlike the
typical `make` command, the target argument is required; if the default target
(of IoTivity-Lite) is desired, then "all" must be specified.

Additionally, in this case, the `--env MAKEFILE=iotivity-app.mk` can be omitted,
as the image assumes that the Makefile will be named `iotivity-app.mk`.

### Bind Mounts vs Volumes

Refer to the [Docker documentation](https://docs.docker.com/storage/volumes/)
for more information on volumes and how they differ from bind mounts.

In the case of the command above, a bind mount is used for the output target of
the build process. While this ensures that the binary that is built is on the
host system, a potential issue is that (on Linux systems), the output binary
will be owned by the root user.

Using volumes can help to work around this issue, but require additional steps
to copy source files to the volume and output binaries back to the host.
However, using volumes can also provide a convenient way to mount new binaries
and execute them with the `ocfadmin/iotivity-examples` image.

The code below could be used to create a volume, copy source files to it,
compile them to the output binary, and subsequently run the binary with the
`ocfadmin/iotivity-examples` image:

```bash
# Create volume
$ docker volume create myapp

# Prep container
$ docker create --rm --name builder -v myapp:/iotivity-app ocfadmin/iotivity-builder iotivity-app/my_app

# Copy source to volume
$ cd /path/to/the/app
$ docker cp . builder:/iotivity-app

# Run the build
$ docker start -a builder

# If compilation succeeded, create a runner for the output binary
$ docker run -i -t --name runner -v myapp:/iotivity-apps/new_app --entrypoint=/iotivity-apps/new_app/my_app ocfadmin/iotivity-examples

# (Optional): Copy binary back out of the volume
$ docker cp runner:/iotivity-apps/new_app/my_app .
```

Of course, the additional steps required to copy the binary are tedious to
execute by hand, and so a Makefile that somewhat automates the process may be
helpful. See the demo [Makefile](./demo/Makefile) as an example.

### Changing IoTivity Build Options

If different build [variables](https://iotivity.org/documentation/building-iotivity-linux)
are desired, they can be specified using the `--env` argument of `docker run`.
However, they may require recompiling the IoTivity-Lite libraries to take full
effect (i.e. to apply during all compilations).

For example, one could specify the following command to enable debug output and
IPv4 support for their build:

```bash
$ docker run --rm --env DEBUG=1 --env IPV4=1 -v /path/to/the/app:/iotivity-app ocfadmin/iotivity-builder cleanall <APP_TARGET>
```

Specifying the `cleanall` target before the app target ensures that the
libraries are rebuilt and any Lite feature variables are applied during
compilation.

### Build Environment Shell

Alternatively, this image could be used for a more persistent cli application
with which a developer could compile and manipulate different parts of the
IoTivity codebase manually. To do so, changing the entrypoint of the image is
necessary; for example:

```bash
$ docker run -i -t --name iot-dev --entrypoint=/bin/bash -v myapp:/iotivity-app ocfadmin/iotivity-builder

# Now in resulting shell
root@3866e36f7464:/iotivity-lite/port/linux# make -f ./iotivity-app/iotivity-app.mk iotivity-app/speaker_server
```

This method of running the image can be used for a more persistent development
environment. Additionally, this environment should be able to run compiled
applications.
