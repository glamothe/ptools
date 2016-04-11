# Dockerfile to create a development envirionnement for ptools.
#
# Author: Benoist LAURENT <benoist.laurent@gmail.com>
# Date: 2016/03/30
# Version: 1.0.0
#
# Usage:
#     $ # Docker container creation (first time only).
#     $ docker build -t ptools:dev /path/to/ptools/dev
#     $
#     $ # Work in docker environment (from ptools local repository).
#     $ docker run --rm -v $(pwd):/src/ptools ptools:dev

FROM debian:latest

# Install system dependecies.
COPY requirements_system.txt /opt/
RUN apt-get update && cat /opt/requirements_system.txt | xargs apt-get install -y

# Install python dependencies
COPY requirements_python.txt /opt/
RUN pip install --upgrade pip && pip install -r /opt/requirements_python.txt


# Set working directory.
WORKDIR /src


# Compile and install ptools libf2c.
RUN git clone http://github.com/ptools/ptools_dep && \
    cd ptools_dep/libf2c2-20090411 && \
    make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c" && \
    make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c" libf2c.a && \
    cp f2c.h    /usr/include && \
    cp libf2c.a /usr/lib

COPY entrypoint.sh /
ENTRYPOINT ["/entrypoint.sh"]