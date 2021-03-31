# Dockerized IoTivity-Lite Examples

*Containerized version of the standard simpleclient-simpleserver-onboarding_tool
demo.*

*Author: Andy Dolan (CableLabs)*

## Contents

* [Overview](#overview)
* [Dependencies](#dependencies)
  * [IoTivity-Lite Environment](#environment-and-iotivity-lite-build)
  * [Docker](#docker)
  * [IPv6 Support](#ipv6-support)
* [Building the Image](#building-the-image)
  * [Image Structure](#image-structure)
* [Using the Image](#using-the-image)
  * [Use of Volumes](#use-of-volumes)
* [Using Docker-Compose](#using-docker-compose)

## Overview

This repository contains the definitions necessary to build a container image
that is able to run IoTivity-Lite example applications. Specifically, the image
is packaged with `simpleclient`, `simpleserver`, and `onboarding_tool`, which
can be used together to demonstrate how a typical flow of discovery, onboarding,
provisioning, and normal operation can be done in OCF.

This project necessarily relies on [IoTivity-Lite](https://gitlab.iotivity.org/iotivity/iotivity-lite)
and is marked as a submodule in the root directory.

## Dependencies

### Environment and IoTivity-Lite Build

This project first builds IoTivity Apps natively and assumes a Debian Linux
based distribution (e.g., Ubuntu or vanilla Debian). See the Linux instructions
for building IoTivity-Lite [here](https://iotivity.org/documentation/building-iotivity-linux).
The development requirements are effectively the `build-essential` package on
Debian-based systems.

### Docker

Docker is assumed to be installed as the container image build tool.

### IPv6 Support

Additionally, the use of IPv6 is assumed in the default build, and IPv6 support
must be enabled for IoTivity example container instances to communicate with
each other. [This guide](https://docs.docker.com/config/daemon/ipv6/) details
how to enable IPv6 support in the Docker daemon. **Note** that Docker IPv6
support is only available for Linux hosts.

Alternatively, you may specify `IPV4=1` when building to enable IPv4 support in
the IoTivity build (see below).

## Building the Image

The [`Makefile`](/Makefile) is configured to first compile the IoTivity-Lite
binaries, then build the image based on the [`Dockerfile`](/examples/Dockerfile)
in this directory.

The default image and tag that is built is `ocfadmin/iotivity-examples:latest`.
These defaults can be overridden by specifying the variables within the `make`
command, as follows:

```bash
$ make IMAGE=demo/iotivity-examples TAG=foo
```

In this case, it is important to note that the default `cleanimage` routine will
not account for images that do not have the default name:tag of
`ocfadmin/iotivity-examples:latest`.

Note that different features in IoTivity-Lite can be enabled or disabled with
[variables](https://iotivity.org/documentation/building-iotivity-linux)
specified during the build process. These variables can be set during the build
of the container images defined here, either as environment variables for the
`make` command, or direct `make` arguments. For example, to include IPv4 support
and `DEBUG` logging in the image, one could use the following `make` command:

```bash
$ make cleaniotivity
$ make IOTIVITY_BUILD_ARGS="IPV4=1 DEBUG=1"
```

The version of IoTivity lite that is used can also be specified using the
`IOTIVITY_VERSION` variable. Changing this after initiating the IoTivity-Lite
submodule may require a `make deinit` before rebuilding, to ensure that the
proper version is used in builds.

### Image Structure

The final image has a file structure containing the following elements
(summarized):

* `/creds`: Contains the various `*_creds` directories for the individual apps.
* `/iotivity-apps`: Contains the actual binaries for each application, as well
  as symlinks for the corresponding `*_creds` directories, IE:

```bash
$ ls -ld /iotivity-apps/simpleserver_creds
/iotivity-apps/simpleserver_creds -> /creds/simpleserver_creds
```

These credential files are referenced by a symlink for cases where a
volume-based solution for the persistent storage of `/creds` is desired.

## Using the Image

Once built, the image can be instantiated to run any of the three applications.
To get a basic demo, the following commands could be used:

```bash
# Run in separate terminals...
$ docker run -it --name obt --entrypoint /iotivity-apps/onboarding_tool ocfadmin/iotivity-examples
$ docker run -it --name simpleserver --entrypoint /iotivity-apps/simpleserver ocfadmin/iotivity-examples
$ docker run -it --name simpleclient --entrypoint /iotivity-apps/simpleclient ocfadmin/iotivity-examples
```

If an instance needs to be restarted (e.g., after onboarding and provisioning
the client and server, the containers can be exited with ctrl-c and restarted
with the following:

```bash
$ docker start -ai <name of container>
```

### Use of Volume(s)

Note that in this simple scenario, the `/creds` directory and the corresponding
security materials that the IoTivity applications use to function only persist
with the container, meaning that once the container is destroyed (e.g., `docker
rm <container>`), the credentials no longer persist.

For the credentials to persist across different instances, the use of a
[volume](https://docs.docker.com/storage/volumes/) may be appropriate. To do so,
you may specify the `--mount` flag when executing the containerized examples.

## Using `docker-compose`

If you have `docker-compose` installed, you can also use it to quickly bootstrap
the example application containers with a simple `docker-compose up -d` from the
main directory, which uses the [`docker-compose.yml`](/docker-compose.yml)
located there. **Note** that this creates a network with IPv6 specified, but
that IPv6 is not officially supported in v3 compose files (this seems to be
working, however).

Once the containers are up, it is helpful to use `docker-compose logs -f
simpleclient simpleserver` in one terminal and `docker attach obt` in another to
perform onboarding and observe results. However, because `simpleclient` tries to
discover `simpleserver` immediately when it runs, it is helpful to restart these
containers individually after provisioning, i.e.

```bash
$ docker restart simpleserver
$ docker restart simpleclient
```

Note that the `onboarding_tool` will initially output a main menu that may not
be visible when first running `docker attach obt`. If this occurs, simply type
`0` and hit enter after attaching to reprint the main menu.
