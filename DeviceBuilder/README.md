# Containerized DeviceBuilder Image

## Introduction

This directory contains a definition of a container image of the OCF
[DeviceBuilder](https://github.com/openconnectivityfoundation/DeviceBuilder)
tool. This image can be used to generate code from DeviceBuilder input files,
and is meant to be used as a one-off command to execute the
`DeviceBuilder_IotivityLiteServer.sh` script. Volumes or bind-mounts must be
used to provide the input file and to retrieve the generated source files.

## Structure

### Notable Directories

* `/ocf_tooling`: Location of the `DeviceBuilder` source and its necessary
  dependencies
* `/ocf_tooling/DeviceBuilder`: Location of the DeviceBuilder source and the
  working directory of the image
* `/devbuilder`: Where source materials should be mounted as a bind mount or
  volume
  * This directory is symlinked from the `/ocf_tooling/DeviceBuilder` directory

## Basic Usage

All that is required to use this image is a DeviceBuilder input JSON file (refer
to some examples [here](https://openconnectivityfoundation.github.io/DeviceBuilder/DeviceBuilderInputFormat-file-examples/#examples-with-iotivity)).
This file should be provided to the container as a bind mount or volume. For
example, assume the following directory structure:

```
example
`-- speaker_model.json
```

To generate the IoTivity source from `speaker_model.json`, the following command
could be used:

```
$ docker run --rm -v /path/to/example:/devbuilder ocfadmin/devicebuilder speaker_model.json "oic.d.speaker"
```

The container will produce new files through the bind mount that might look like
the following:

```
example
|-- output
|   |-- code
|   |   |-- PICS.json
|   |   |-- readme.md
|   |   |-- server_introspection.dat
|   |   |-- server_introspection.dat.h
|   |   `-- simpleserver.c
|   |-- out_codegeneration_merged.swagger.json
|   |-- out_introspection_merged.swagger.json
|   |-- out_introspection_merged.swagger.json.cbor
|   `-- speaker_model.json
`-- speaker_model.json
```

Note that, when using bind mounts, the resulting output files (the `output`
directory) will be owned by `root`.
