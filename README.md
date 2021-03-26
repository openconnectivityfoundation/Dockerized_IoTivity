# Dockerized IoTivity Resources

*Container-based solutions for both building and running IoTivity-Lite
applications.*

*Author: Andy Dolan (CableLabs)*

## Contents

* [Introduction](#introduction)
* [Examples Overview](#iotivity-lite-examples)
* [Development Environment Overview](#iotivity-lite-development-environment)
* [General Building](#general-building)

## Introduction

This repository contains definitions and scripts to build and run
container-based solutions for IoTivity-Lite demonstrations and build processes.
These images can be used to run [example applications](/examples) already
contained in IoTivity-Lite, as well as [build](/development_environment)
applications from source code without cloning and configuring Lite. Published
images are available at the `ocfadmin` organization account on [Docker Hub](https://hub.docker.com/u/ocfadmin).

**These images are considered to be prototypes,** and are still under
development.

## IoTivity-Lite Examples

The [examples](/examples) directory contains a `Dockerfile` that defines an
image containing IoTivity-Lite binaries that can be run as instantiated
containers to demonstrate the general use and interactions of OCF Devices.

See the examples [README](/examples/README.md) for more information.

## IoTivity-Lite Development Environment

The [development\_environment](/development_environment) directory contains a
definition of a container that can be used to compile IoTivity-Lite applications
from source, as well as a [demonstration](/development_environment/demo) of how
to do so with an example source file.

See the development environment [README](/development_environment/README.md) for
more information.

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

See the development environment demo [README](/development_environment/demo/README.md)
for an example of building IoTivity apps with the `iotivity-builder` image.
