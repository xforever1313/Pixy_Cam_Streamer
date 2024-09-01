FROM ubuntu:24.04

RUN apt update -y && \
    apt upgrade -y && \
    apt install -y gcc scons libboost-dev libusb-1.0-0-dev libpoco-dev

