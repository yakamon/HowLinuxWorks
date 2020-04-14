ARG UBUNTU_VERSION=16.04
FROM ubuntu:${UBUNTU_VERSION}

RUN apt update -y && apt upgrade -y
RUN apt install -y \
    binutils \
    build-essential \
    curl \
    git \
    libbz2-dev \
    libffi-dev \
    liblzma-dev \
    libncurses5-dev \
    libncursesw5-dev \
    libreadline-dev \
    libsqlite3-dev \
    libssl-dev \
    llvm \
    make \
    strace \
    sysstat \
    tk-dev \
    wget \
    xz-utils \
    zlib1g-dev \
    zsh
RUN apt autoremove -y

USER root
WORKDIR /root

ENV SETUP="/root/setup.sh"
COPY ./root/setup.sh ${SETUP}
RUN ${SETUP}
