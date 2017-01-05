FROM ubuntu:xenial

RUN export DEBIAN_FRONTEND=noninteractive \
 && apt-get update \
 && apt-get install -y --no-install-recommends pcscd libpcsclite1 libpcsclite-dev libccid pcsc-tools build-essential curl git-core libssl-dev yasm libtool autoconf pkg-config ca-certificates apt-transport-https \
 && curl -vsL https://deb.nodesource.com/setup_6.x | bash -x - \
 && apt-get install -y --no-install-recommends nodejs \
 && update-alternatives --install /usr/bin/node node /usr/bin/nodejs 10 \
 && apt-get clean \
 && rm -rf /var/cache/apt/archives/* /var/lib/apt/lists/* 

RUN npm install pm2 -g

# hack for mirakurun's install script
RUN ln -sf /bin/true /usr/bin/pm2
RUN npm install mirakurun -g --unsafe --production
RUN npm install arib-b25-stream-test -g --unsafe

EXPOSE 40772
CMD [ "pm2-docker", "processes.json" ]
WORKDIR /usr/lib/node_modules/mirakurun
