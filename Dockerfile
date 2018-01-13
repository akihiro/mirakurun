FROM node:8 as node
ADD extract /
RUN /extract /node /usr/local/bin/node

FROM node:8 as app
ADD patch /tmp/patch
RUN mkdir -p /usr/src/app \
 && cd /usr/src/app \
 && git clone https://github.com/Chinachu/Mirakurun.git . \
 && xargs rm -rf <.dockerignore \
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

RUN mkdir -p -m 777 /chroot/tmp/ /chroot/var/run/ /chroot/var/tmp/
COPY passwd /chroot/etc/
COPY group /chroot/etc/
ADD extract /
COPY --from=node /node/ /node/
RUN /extract -e /node /chroot /bin/cat
RUN /extract -e /node /chroot /usr/local/bin/tune++
RUN /extract -e /node /chroot /usr/local/bin/arib-b25-stream-test
RUN cp -a /etc/ld.so.cache /chroot/etc/ld.so.cache
RUN rm -rf /chroot/etc/ld.so.conf /chroot/etc/ld.so.conf.d/
RUN mkdir -p /chroot/usr/src/ && cp -a /usr/src/app/ /chroot/usr/src/app/
ADD etc/ /chroot/usr/local/etc/mirakurun/

FROM scratch
COPY --from=node /node/ /
COPY --from=app /chroot/ /
WORKDIR /usr/src/app/
ENV NODE_ENV production
CMD [ "node", "--max_old_space_size=256", "lib/server.js" ]
EXPOSE 40772
USER node
VOLUME /var/run/
