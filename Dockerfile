FROM node:8

ADD patch /tmp/patch
RUN mkdir -p /usr/src/app \
 && cd /usr/src/app \
 && git clone https://github.com/Chinachu/Mirakurun.git . \
 && for i in /tmp/patch/*.patch; do patch -p1 <$i ; done \
 && DOCKER=YES npm install \
 && DOCKER=YES npm run build \
 && rm -rf node_modules \
 && DOCKER=YES npm install --production --unsafe

RUN apt-get update \
 && apt-get install -y --no-install-recommends libpcsclite-dev \
 && npm install arib-b25-stream-test -g --unsafe \
 && rm -f /usr/local/bin/arib-b25-stream-test \
 && cp -a /usr/local/lib/node_modules/arib-b25-stream-test/bin/b25 /usr/local/bin/arib-b25-stream-test

ADD tune++ /tmp/tune++
RUN cd /tmp/tune++ && make && make install

WORKDIR /usr/src/app/
ENV NODE_ENV production
CMD [ "node", "--max_old_space_size=256", "lib/server.js" ]
EXPOSE 40772
USER node
VOLUME /var/run/
