# Dockerized IoTivity Resources

*Container-based solutions for both building and running IoTivity-Lite
applications.*

*Author: Andy Dolan (CableLabs)*

## Contents

* [Introduction](#introduction)
* [Examples Overview](#iotivity-lite-examples)
* [Build Environment Overview](#iotivity-lite-build-environment)
* [General Building](#general-building)

## Introduction

This repository contains definitions and scripts to build and run
container-based solutions for IoTivity-Lite demonstrations and build processes.
These images can be used to run [example applications](/examples) already
contained in IoTivity-Lite, as well as [build](/build_environment)
applications from source code without cloning and configuring Lite. Published
images are available at the `ocfadmin` organization account on [Docker Hub](https://hub.docker.com/u/ocfadmin).

**These images are considered to be prototypes,** and are still under
development.

## IoTivity-Lite Examples

The [examples](/examples) directory contains a `Dockerfile` that defines an
image containing IoTivity-Lite binaries that can be run as instantiated
containers to demonstrate the general use and interactions of OCF Devices.

See the examples [README](/examples/README.md) for more information.

## IoTivity-Lite Build Environment

The [build\_environment](/build_environment) directory contains a
definition of a container that can be used to compile IoTivity-Lite applications
from source, as well as a [demonstration](/build_environment/demo) of how
to do so with an example source file.

See the build environment [README](/build_environment/README.md) for more
information.

## DeviceBuilder Image

The [DeviceBuilder](/DeviceBuilder) directory contains a definition of a
containerized OCF [DeviceBuilder](https://github.com/openconnectivityfoundation/DeviceBuilder)
which can be used to generate IoTivity-Lite source files from input model files.

## General Building

This repository has one primary [Makefile](/Makefile) that can be used to build
the images. This file is intended to be used from the root directory. A Debian
Linux environment is assumed, and Docker is assumed to be installed. There are
additional requirements to build the examples image that can be found in the
examples [README](/examples/README.md#dependencies).

To make all images, a simple `make` can be used. Individual images can be made
with their corresponding make recipes (e.g. `make examples` or `make dev`).

## General Use

To run a simple demo of the `iotivity-examples` image, simply use a
`docker-compose up -d` in the main directory. See further instructions in the
examples [README](/examples/README.md#using-docker-compose).

See the build environment demo [README](/build_environment/demo/README.md) for
an example of building IoTivity apps with the `iotivity-builder` image.
