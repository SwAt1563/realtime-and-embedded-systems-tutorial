FROM accetto/ubuntu-vnc-xfce-opengl-g3:latest




USER root
ENV HOME /root

# to include man pages
RUN sed -i 's:^path-exclude=/usr/share/man:#path-exclude=/usr/share/man:' \
        /etc/dpkg/dpkg.cfg.d/excludes

RUN apt-get update && apt -y --no-install-recommends install \
    build-essential \
    gdb  \
    gcc \
    cmake \
    man  \
    man-db \
    manpages-posix \
    freeglut3-dev \
    libglu1-mesa-dev \
    mesa-common-dev


COPY . /Project

WORKDIR /Project/


# VNC connect with container on port 5901
EXPOSE 5901

