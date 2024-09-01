FROM ubuntu:24.04

RUN apt-get update -y && \
    apt-get upgrade && \
    apt-get install gcc scons libboost-dev libusb-1.0-0-dev libpoco-dev

