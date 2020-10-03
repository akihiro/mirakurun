FROM node:14-alpine AS builder
RUN apk -U add --no-cache git make gcc g++ autoconf automake libc-dev pcsc-lite-dev linux-headers

FROM builder AS app
ENV VERSION=3.3.1
ARG DOCKER=YES
RUN npm install -g mirakurun@${VERSION}

FROM builder AS b25-builder
RUN npm install arib-b25-stream-test -g --unsafe

FROM builder AS tune-builder
WORKDIR /tune++
COPY tune++ .
RUN make

FROM node:14-alpine
RUN apk add --no-cache pcsc-lite-libs
COPY --from=tune-builder /tune++/tune++ /bin/tune++
COPY --from=b25-builder /usr/local/lib/node_modules/arib-b25-stream-test/bin/b25 /bin/arib-b25-stream-test
COPY --from=app /usr/local/lib/node_modules/mirakurun /app

WORKDIR /app/
ENV NODE_ENV production
CMD [ "node", "--max_old_space_size=256", "lib/server.js" ]
EXPOSE 40772
USER 1001
VOLUME /var/run/
