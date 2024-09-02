FROM ubuntu:24.04

RUN apt update -y && \
    apt upgrade -y && \
    apt install -y g++ scons libboost-all-dev libusb-1.0-0-dev libpoco-dev

