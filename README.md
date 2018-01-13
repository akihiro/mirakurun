# mirakurun

This is mirakurun for docker

## How to Use

```bash
docker run \
  -d -p 40772:40772 --restart=always --name mirakurun \
  -v /path/to/config:/usr/local/etc/mirakurun:ro \
  -v /path/to/db:/usr/local/var/db/mirakurun \
  -v /var/run/pcscd/pcscd.comm:/var/run/pcscd/pcscd.comm:rw \
  --device /dev/dvb/:/dev/dvb \
  mhiroaki/mirakurun
```

- Need run pcscd on host
- see also docker-compose.yml

## Configuration

example configuration files in /etc

- tunners.yml: PT3 for dvb driver:earth_pt3 (not chardev driver:pt3-drv) x2
- channels.yml: Tokyo area GR channels and BS/CS channels
- server.yml: minimal configuration
