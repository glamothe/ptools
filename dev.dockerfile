# Dockerfile to create a development envirionnement for ptools.
#
# Author: Benoist LAURENT <benoist.laurent@gmail.com>
# Date: 2016/03/30
# Version: 1.0.0
#
# Usage:
#     $ # Docker container creation (first time only).
#     $ docker build -t ptools:dev -f dev.dockerfile .
#     $
#     $ # Work in docker environment (from ptools local repository).
#     $ docker run -it --rm -v $(pwd):/src/ptools ptools:dev /bin/bash

FROM debian:latest

# Install dependecies.
RUN apt-get update && apt-get install -y \
    g++ \
    git \
    libboost-dev \
    python-dev \
    python-pip

# Install Cython
RUN pip install cython


# Set working directory.
WORKDIR /src


# Compile and install ptools libf2c.
RUN git clone http://github.com/ptools/ptools_dep && \
    cd ptools_dep/libf2c2-20090411 && \
    make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c" && \
    make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c" libf2c.a && \
    cp f2c.h    /usr/include && \
    cp libf2c.a /usr/lib

COPY docker-entrypoint.sh /
ENTRYPOINT ["/docker-entrypoint.sh"]