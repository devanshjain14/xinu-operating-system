FROM debian:latest

MAINTAINER Ezra Kissel <ezkissel@indiana.edu>

EXPOSE 67

RUN apt-get update
RUN dpkg --add-architecture armel
RUN dpkg --add-architecture i386
RUN apt-get update && apt-get -y install apt-utils sudo git make bridge-utils dnsmasq flex bison vim gawk zlib1g-dev
RUN apt-get -y install crossbuild-essential-i386 crossbuild-essential-armel gcc-arm-none-eabi qemu-system-x86 qemu-system-arm

RUN export uid=1000 gid=1000 && \
    useradd -rm -d /home/xinu -s /bin/bash -G sudo -u ${uid} xinu && \
    echo "xinu ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/xinu && \
    chmod 0440 /etc/sudoers.d/xinu && \
    chown -R ${uid} /home/xinu && \
    chown -R ${uid} /opt

USER xinu
ENV HOME /home/xinu
WORKDIR $HOME

ADD --chown=xinu xinu.key ./.ssh/id_rsa
ADD --chown=xinu xinu.pub ./.ssh/id_rsa.pub

RUN ssh-keyscan github.iu.edu >> ~/.ssh/known_hosts
RUN git clone git@github.iu.edu:SICE-OS/xinu

ADD --chown=xinu Makedefs.arm ./xinu/compile/Makedefs

ADD run.sh .
CMD bash ./run.sh
