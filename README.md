# RapidEvent

RapidEvent is an event builder for heavy-ion collisions, assembling tracks from
ROOT files generated by [RapidSim](https://github.com/gcowan/RapidSim) to
create an event.

## Installation

RapidEvent relies on having a working ROOT installation.

After cloning this repository, source ROOT an build:

```shell
$ git clone https://github.com/g-ricart/RapidEvent.git
$ source path/to/ROOT/installation/bin/thisroot.sh
$ cd RapidEvent/
$ make
```

## Usage

```shell
$ export RAPIDEVENT_ROOT=<install location>
$ mkdir output/
$ cd output/
$ ../bin/RapidEvent <event config file name> <nb of events to generate>
```
