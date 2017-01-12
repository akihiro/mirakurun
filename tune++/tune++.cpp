/*
 * Copyright 2017 Hiroaki Mizuguchi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <linux/dvb/frontend.h>
#include <linux/dvb/dmx.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <functional>

enum isdb_type {
	ISDB_T,
	ISDB_S,
};

struct channel {
	const char name[7];
	uint8_t type;
	uint32_t frequency; // freqno or freq (terrestrial Hz, BSCS110 kHz)
};

static constexpr channel channels[] = {
	{"13", ISDB_T, 473142857},
	{"14", ISDB_T, 479142857},
	{"15", ISDB_T, 485142857},
	{"16", ISDB_T, 491142857},
	{"17", ISDB_T, 497142857},
	{"18", ISDB_T, 503142857},
	{"19", ISDB_T, 509142857},
	{"20", ISDB_T, 515142857},
	{"21", ISDB_T, 521142857},
	{"22", ISDB_T, 527142857},
	{"23", ISDB_T, 533142857},
	{"24", ISDB_T, 539142857},
	{"25", ISDB_T, 545142857},
	{"26", ISDB_T, 551142857},
	{"27", ISDB_T, 557142857},
	{"28", ISDB_T, 563142857},
	{"29", ISDB_T, 569142857},
	{"30", ISDB_T, 575142857},
	{"31", ISDB_T, 581142857},
	{"32", ISDB_T, 587142857},
	{"33", ISDB_T, 593142857},
	{"34", ISDB_T, 599142857},
	{"35", ISDB_T, 605142857},
	{"36", ISDB_T, 611142857},
	{"37", ISDB_T, 617142857},
	{"38", ISDB_T, 623142857},
	{"39", ISDB_T, 629142857},
	{"40", ISDB_T, 635142857},
	{"41", ISDB_T, 641142857},
	{"42", ISDB_T, 647142857},
	{"43", ISDB_T, 653142857},
	{"44", ISDB_T, 659142857},
	{"45", ISDB_T, 665142857},
	{"46", ISDB_T, 671142857},
	{"47", ISDB_T, 677142857},
	{"48", ISDB_T, 683142857},
	{"49", ISDB_T, 689142857},
	{"50", ISDB_T, 695142857},
	{"51", ISDB_T, 701142857},
	{"52", ISDB_T, 707142857},
	{"53", ISDB_T, 713142857},
	{"54", ISDB_T, 719142857},
	{"55", ISDB_T, 725142857},
	{"56", ISDB_T, 731142857},
	{"57", ISDB_T, 737142857},
	{"58", ISDB_T, 743142857},
	{"59", ISDB_T, 749142857},
	{"60", ISDB_T, 755142857},
	{"61", ISDB_T, 761142857},
	{"62", ISDB_T, 767142857},
	{"BS-1",  ISDB_S, 1049480},
	{"BS-3",  ISDB_S, 1087840},
	{"BS-5",  ISDB_S, 1126200},
	{"BS-7",  ISDB_S, 1164560},
	{"BS-9",  ISDB_S, 1202920},
	{"BS-11", ISDB_S, 1241280},
	{"BS-13", ISDB_S, 1279640},
	{"BS-15", ISDB_S, 1318000},
	{"BS-17", ISDB_S, 1356360},
	{"BS-19", ISDB_S, 1394720},
	{"BS-21", ISDB_S, 1433080},
	{"BS-23", ISDB_S, 1471440},
	{"ND-2",  ISDB_S, 1613000},
	{"ND-4",  ISDB_S, 1653000},
	{"ND-6",  ISDB_S, 1693000},
	{"ND-8",  ISDB_S, 1733000},
	{"ND-10", ISDB_S, 1773000},
	{"ND-12", ISDB_S, 1813000},
	{"ND-14", ISDB_S, 1853000},
	{"ND-16", ISDB_S, 1893000},
	{"ND-18", ISDB_S, 1933000},
	{"ND-20", ISDB_S, 1973000},
	{"ND-22", ISDB_S, 2013000},
	{"ND-24", ISDB_S, 2053000}
};

bool tune(int adapter, const channel& ch, uint16_t tsid = 0) {
	char filepath[64];
	snprintf(filepath, sizeof(filepath), "/dev/dvb/adapter%d/frontend0", adapter);
	int fd;
	if ((fd = open(filepath, O_RDWR)) < 0) {
		perror("open");
		return false;
	}

	struct dvb_frontend_info info;
	if (ioctl(fd, FE_GET_INFO, &info) < 0) {
		perror("ioctl FE_GET_INFO");
		return false;
	}
	if (info.type == FE_QPSK) {
		if(ch.type != ISDB_S) {
			fprintf(stderr, "#%d adapter is ISDB-S tuner.\n", adapter);
			return false;
		}
	} else if (info.type == FE_OFDM) {
		if(ch.type != ISDB_T) {
			fprintf(stderr, "#%d adapter is ISDB-T tuner.\n", adapter);
			return false;
		}
		tsid = 0;
	} else {
		fprintf(stderr, "Unknown type of adapter\n");
		return false;
	}

	struct dtv_property prop[3];
	struct dtv_properties props;
	prop[0].cmd = DTV_FREQUENCY;
	prop[0].u.data = ch.frequency;
	prop[1].cmd = DTV_STREAM_ID;
	prop[1].u.data = tsid;
	prop[2].cmd = DTV_TUNE;
	props.props = prop;
	props.num = 3;
	if ((ioctl(fd, FE_SET_PROPERTY, &props)) < 0) {
		perror("ioctl FE_SET_PROPERTY");
		return false;
	}

	prop[0].cmd = DTV_STAT_CNR;
	props.num = 1;

	fe_status_t status;
	for (int i = 0; i < 8; i++) {
		if ((ioctl(fd, FE_READ_STATUS, &status) < 0) || (ioctl(fd, FE_GET_PROPERTY, &props) < 0)) {
			perror("ioctl FE_READ_STATUS / FE_GET_PROPERTY");
			return false;
		}
		if ((status & FE_HAS_LOCK) && prop[0].u.st.stat[0].svalue) {
			fprintf(stderr, "#%d Successfully tuned to %s, tsid: 0x%x, %f dB\n",
				adapter, ch.name, tsid, ((double)prop[0].u.st.stat[0].svalue)/10000.0);
			return true;
		}
		usleep(125 * 1000);
	}
	fprintf(stderr, "#%d Failed to tune %s, tsid: 0x%x, status 0x%02X\n",
		adapter, ch.name, tsid, status);
	return false;
}

bool track(int adapter_nr) {
	char file[64];
	snprintf(file, sizeof(file), "/dev/dvb/adapter%d/demux0", adapter_nr);
	int fd;
	if ((fd = open(file, O_RDWR)) < 0) {
		perror("open");
		return false;
	}

	const struct dmx_pes_filter_params filter = {
		0x2000,
		DMX_IN_FRONTEND,
		DMX_OUT_TS_TAP,
		DMX_PES_VIDEO,
		DMX_IMMEDIATE_START,
	};
	if (ioctl(fd, DMX_SET_PES_FILTER, &filter) < 0) {
		perror("ioctl DMX_SET_PES_FILTER");
		return false;
	}
	return true;
}

void usage(const char* progname) {
	fprintf(stderr,
		"Usage: %s adapter channel[_tsid]\n"
		"Arguments:\n"
		"\tadapter  : 0,1,2,3,... : select /dev/dvb/adapterN/\n"
		"\tchannel  : 13,14,..,62, BS-1,3,5,..,23, ND-2,4,6,..,24\n"
		"\tts_id       : 0x4000 or 16384: hex or dec(default 0)\n"
		"\t              this option is ignored on ISDB-T"
		"\nSatellite channels:\n",
		progname);
	for (const auto& p : channels) {
		if (p.type == isdb_type::ISDB_S)
			fprintf(stderr, "\t%s : %d kHz\n", p.name, p.frequency);
	}
	fprintf(stderr, "\nTerrestrial channels:\n");
	for (const auto& p : channels) {
		if (p.type == isdb_type::ISDB_T)
			fprintf(stderr, "\t%s : %d Hz\n", p.name, p.frequency);
	}
}

int main(int argc, char *argv[]) {
	uint32_t adapter;
	uint16_t tsid = 0;
	if (argc != 3 && argc != 4) {
		usage(argv[0]);
		return 1;
	}
	sscanf(argv[1],"%d", &adapter);
	std::string channel_id = {argv[2]};
	auto it = std::find(channel_id.cbegin(), channel_id.cend(), '_');
	if (it != channel_id.cend()) {
		std::string newstr(channel_id.cbegin(), it);
		++it;
		std::stringstream t(std::string(it, channel_id.cend()));
		int n = strnlen(&*it, 16);
		if (n == 16)
			fprintf(stderr, "tsid is too long\n");
		if (n >= 3 && it[0] == '0' && it[1] == 'x'){ 
			t >> std::hex;
		}
		t >> tsid;
		channel_id = newstr;
	}

	const auto ch = std::find_if(std::begin(channels), std::end(channels),
		[&channel_id](const channel& x){return channel_id == x.name;}
	);
	if (ch == std::end(channels)) {
		fprintf(stderr, "Unknown channel: %s\n", channel_id.c_str());
		return -1;
	}

	if (!tune(adapter, *ch, tsid))
		return 1;
	if (!track(adapter))
		return -1;
	while (1)
		sleep(3);
	return 0;
}
