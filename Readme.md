# Pixy Cam Streamer

This tool allows one to use a [Pixy CMUCam5](https://pixycam.com/pixy-cmucam5/) to stream to an RTMP server.  This can be used to stream footage the Pixy Cam captures and streams it to sites such as YouTube or PeerTube.

## Building

To build the code yourself, you need to install the following packages on a Linux operating system (Windows or MacOS not officially supported).

* g++
* SCons
* libboost-all-dev
* libusb-1.0-0-dev
* libpoco-dev
* libconfig++-dev

Remember to update and initialize all submodules (there's one located in ```libpixy/pixy```).

Then run ```scons build``` in the root of this repo.  You can also run ```scons debian_build``` to build a Debian installer package (.deb file).

There is a Docker file in the Docker directory named ```ubuntu_build_env.dockerfile``` that can be used to get a build environment, in case you don't run a Debian-based operating system.

## Running

TODO: Need to talk about configuration.