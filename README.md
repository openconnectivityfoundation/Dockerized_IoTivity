# Dockerized IoTivity-Lite Examples

*Containerized version of the standard simpleclient-simpleserver-onboarding_tool
demo.*

*Author: Andy Dolan (CableLabs)*

## Overview

This repository contains the definitions necessary to build a container image
that is able to run IoTivity-Lite example applications. Specifically, the image
is packaged with `simpleclient`, `simpleserver`, and `onboarding_tool`, which
can be used together to demonstrate how a typical flow of discovery, onboarding,
provisioning, and normal operation can be done in OCF.

This project necessarily relies on [IoTivity-Lite](https://gitlab.iotivity.org/iotivity/iotivity-lite)
and is marked as a submodule.

## Dependencies

### Environment and IoTivity-Lite Build

This project first builds IoTivity Apps natively and assumes a Debian Linux
based distribution (e.g., Ubuntu or vanilla Debian). See the Linux instructions
for building IoTivity-Lite [here](https://iotivity.org/documentation/building-iotivity-linux).
The development requirements are effectively the `build-essential` package on
Debian-based systems.

### Docker

Docker is assumed to be installed. Additionally, the use of IPv6 is assumed in
the default build, and IPv6 support must be enabled for IoTivity example
container instances to communicate with each other. [This guide](https://docs.docker.com/config/daemon/ipv6/)
details how to enable IPv6 support in the Docker daemon. **Note** that IPv6
support is only available for Linux hosts.

Alternatively, you may specify `IPV4=1` when building to enable IPv4 support in
the IoTivity build (see below).

## Building the Image

The [`Makefile`](./Makefile) is configured to first compile the IoTivity-Lite
binaries, then build the image based on the [`Dockerfile`](./examples/Dockerfile).

### Image Structure

The final image has a file structure containing the following elements
(summarized):

* `/creds`: Contains the various `*_creds` directories for the individual apps.
* `/iotivity-apps`: Contains the actual binaries for each application, as well
  as symlinks for the corresponding `*_creds` directories, IE:

```
$ ls -ld /iotivity-apps/simpleserver_creds
/iotivity-apps/simpleserver_creds -> /creds/simpleserver_creds
```

These credential files are referenced by a symlink for cases where a
volume-based solution for the persistent storage of `/creds` is desired.

## Using the Image

Once built, the image can be instantiated to run any of the three applications.
To get a basic demo, the following commands could be used:

```
# Run in separate terminals...
$ docker run -it --name obt --entrypoint /iotivity-apps/onboarding_tool iotivity/examples
$ docker run -it --name simpleserver --entrypoint /iotivity-apps/simpleserver iotivity/examples
$ docker run -it --name simpleclient --entrypoint /iotivity-apps/simpleclient iotivity/examples
```

If an instance needs to be restarted (e.g., after onboarding and provisioning
the client and server, the containers can be exited with ctrl-c and restarted
with the following:

```
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
