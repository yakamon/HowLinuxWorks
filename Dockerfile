ARG UBUNTU_VERSION=16.04
FROM ubuntu:${UBUNTU_VERSION}

RUN apt update -y && apt upgrade -y

# Handson requirements
RUN apt install -y \
    binutils \
    build-essential \
    strace \
    sysstat \
    zsh

# Pyenv dependencies
RUN apt install -y \
    # build-essential \
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
    python-openssl \
    tk-dev \
    wget \
    xz-utils \
    zlib1g-dev

RUN apt autoremove -y

USER root
WORKDIR /root

ENV SETUP="/root/setup.sh"
COPY setup.sh ${SETUP}
RUN ${SETUP}
