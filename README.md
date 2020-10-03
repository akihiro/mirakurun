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
  --user 1:44 \
  mhiroaki/mirakurun
```

- Need run pcscd on host
- see also docker-compose.yml
- require uid option since default uid is set to 1001
  - root or 1:44(debian case: video:44 group can read access to the devices)

## Configuration

example configuration files in /etc

- tunners.yml: PT3 for dvb driver:earth_pt3 (not chardev driver:pt3-drv) x2
- channels.yml: Tokyo area GR channels and BS/CS channels
- server.yml: minimal configuration
