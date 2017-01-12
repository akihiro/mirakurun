# mirakurun

This is mirakurun for docker

## How to Use

```bash
docker run --cap-add SYS_ADMIN --cap-add SYS_NICE --device /dev/dvb/:/dev/dvb -v $PWD/etc/:/usr/local/etc/mirakurun/:ro -v /etc/localtime:/etc/localtime:ro -v /var/run:/var/run:rw -v /var/run/pcscd/pcscd.comm:/var/run/pcscd/pcscd.comm:rw -p 40772:40772 akihiro/mirakurun
```

- Add capabillity for ```nice -10``` and ```renice```
- see also docker-compose.yml

## Configuration

example configuration files in /etc

- tunners.yml: PT3 for dvb driver:earth_pt3 (not chardev driver:pt3-drv) x2
- channels.yml: Tokyo area GR channels and BS/CS channels
- server.yml: minimal configuration
