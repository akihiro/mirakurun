FROM node:8-alpine as app

RUN apk -U add --no-cache git make gcc g++ autoconf automake libc-dev pcsc-lite-dev linux-headers

WORKDIR /usr/src/app
ADD patch /tmp/patch
RUN git clone https://github.com/Chinachu/Mirakurun.git . \
 && xargs rm -rf <.dockerignore
RUN for i in /tmp/patch/*.patch; do patch -p1 <$i ; done
ARG DOCKER=YES
RUN npm install \
 && npm run build \
 && rm -rf node_modules \
 && npm install --production --unsafe

RUN npm install arib-b25-stream-test -g --unsafe

ADD tune++ /tmp/tune++
RUN cd /tmp/tune++ && make && make install

WORKDIR /
RUN mkdir -p /chroot/usr/local/bin /chroot/usr/src/
RUN mv /usr/src/app /chroot/usr/src/app
RUN mv /usr/local/lib/node_modules/arib-b25-stream-test/bin/b25 /chroot/usr/local/bin/arib-b25-stream-test
RUN mv /tmp/tune++/tune++ /chroot/usr/local/bin/tune++

FROM node:8-alpine
RUN apk add --no-cache pcsc-lite-libs
COPY --from=app /chroot /
WORKDIR /usr/src/app/
ENV NODE_ENV production
CMD [ "node", "--max_old_space_size=256", "lib/server.js" ]
EXPOSE 40772
USER node
VOLUME /var/run/
