FROM chinachu/mirakurun

RUN apk update
RUN apk add -u make gcc g++ pcsc-lite-dev libc-dev
RUN npm install arib-b25-stream-test -g --unsafe

ADD tune++ /tmp/tune++
RUN apk add -u linux-headers
RUN cd /tmp/tune++ && make && make install

ADD [ "etc/", "/usr/local/etc/mirakurun/"]
