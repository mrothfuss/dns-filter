#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "buffer.h"

#define UDP_LISTEN  1
#define UDP_CONNECT 2

struct endpoint {
	int fd;
	struct addrinfo *info;
};

int
udp_close(struct endpoint *ep) {
	freeaddrinfo(ep->info);
	close(ep->fd);
	return 1;
}

int
udp_open(struct endpoint *ep, char *address, char *port, int mode) {
	struct addrinfo hints, *ai, *p;
	int rv;

	// translate / resolve network address
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	if(mode == UDP_LISTEN) {
		hints.ai_flags = AI_PASSIVE;
	}
	if((rv = getaddrinfo(address, port, &hints, &ai)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 0;
	}

	// try binding available addresses
	for(p = ai; p != NULL; p = p->ai_next) {
		if((rv = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			continue;
		}
		if(mode == UDP_LISTEN) {
			if(bind(rv, p->ai_addr, p->ai_addrlen) < 0) {
				close(rv);
				continue;
			}
		}
		break;
	}

	// couldn't bind
	if(p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 0;
	}

	ep->fd = rv;
	ep->info = ai;

	return 1;
}

int
check_request(uint8_t *buffer, int length) {
	bf_data bf;
	/*
	uint16_t id;
	uint16_t flags;
	uint16_t questions;
	uint16_t answer_rr;
	uint16_t authority_rr;
	uint16_t additional_rr;
	*/
	uint8_t segment_len;
	char segment_dat[256];
	int drop = 0;

	bf.buffer = buffer;
	bf.size = length;
	bf.cursor = 12;

	/*
	id = bf_r16be(&bf); // 0
	flags = bf_r16be(&bf); // 2
	questions = bf_r16be(&bf); // 4
	answer_rr = bf_r16be(&bf); // 6
	authority_rr = bf_r16be(&bf); // 8
	additional_rr = bf_r16be(&bf); // 12
	printf("id: %04X\n", id);
	printf("flags: %04X\n", flags);
	printf("questions: %04X\n", questions);
	printf("answer_rr: %04X\n", answer_rr);
	printf("authority_rr: %04X\n", authority_rr);
	printf("additional_rr: %04X\n", additional_rr);
	*/

	while((segment_len = bf_r8(&bf)) != 0) {
		if(segment_len >= sizeof(segment_dat)) return 0;
		bf_rstr(&bf, segment_dat, segment_len);
		segment_dat[segment_len] = 0;
		printf("%s ", segment_dat);
		if(0 == strcmp(segment_dat, "doubleclick")) drop = 1;
		if(0 == strcmp(segment_dat, "googleadservices")) drop = 1;
		if(0 == strcmp(segment_dat, "googlesyndication")) drop = 1;
		if(0 == strcmp(segment_dat, "googletagservices")) drop = 1;
		if(0 == strcmp(segment_dat, "google-analytics")) drop = 1;
		if(0 == strcmp(segment_dat, "googletagmanager")) drop = 1;
		if(0 == strcmp(segment_dat, "adtechus")) drop = 1;
		if(0 == strcmp(segment_dat, "postrelease")) drop = 1;
		if(0 == strcmp(segment_dat, "ntv")) drop = 1;
		if(0 == strcmp(segment_dat, "advertising")) drop = 1;
		if(0 == strcmp(segment_dat, "moatads")) drop = 1;
		if(0 == strcmp(segment_dat, "imrworldwide")) drop = 1;
		if(0 == strcmp(segment_dat, "crwdcntrl")) drop = 1;
		if(0 == strcmp(segment_dat, "adnxs")) drop = 1;
		if(0 == strcmp(segment_dat, "analytics")) drop = 1;
		if(0 == strcmp(segment_dat, "foresee")) drop = 1;
		if(0 == strcmp(segment_dat, "adzerk")) drop = 1;
		if(0 == strcmp(segment_dat, "adsafeprotected")) drop = 1;
		if(0 == strcmp(segment_dat, "quantserve")) drop = 1;
		if(0 == strcmp(segment_dat, "krxd")) drop = 1;
		if(0 == strcmp(segment_dat, "amazon-adsystem")) drop = 1;
		if(0 == strcmp(segment_dat, "doubleverify")) drop = 1;
		if(0 == strcmp(segment_dat, "spotxchange")) drop = 1;
		if(0 == strcmp(segment_dat, "openx")) drop = 1;
		if(0 == strcmp(segment_dat, "adnxs")) drop = 1;
		if(0 == strcmp(segment_dat, "bidswitch")) drop = 1;
		if(0 == strcmp(segment_dat, "alexa")) drop = 1;
		if(0 == strcmp(segment_dat, "mookie1")) drop = 1;
		if(0 == strcmp(segment_dat, "scorecardresearch")) drop = 1;
	}
	// 2 byte type
	// 2 byte class

	if(drop) {
		printf("- DROPPED\n");
		// rewrite dns header
		bf.cursor = 2; bf_w16be(&bf, 0x8180); // flags
		bf.cursor = 6; bf_w16be(&bf, 2); // answers
		// move cursor to end of buffer
		bf.size = 1024;
		bf.cursor = length;
		bf_wstr(&bf, "\xc0\x0c\x00\x01\x00\x01\x00\x00\x0d\x8b\x00\x04\x7f\x00\x00\x01", 16);
		drop = bf.cursor;
	} else {
		printf("\n");
	}
	fflush(NULL);
	return drop;
}

int main(void)
{
	struct endpoint local, public;
	uint8_t buffer[1024];
	ssize_t length;
	int check_res;
	struct timeval tv;

	if(!udp_open(&local, NULL, "53", UDP_LISTEN)) {
		printf("could not listen\n");
		return -1;
	}
	if(!udp_open(&public, "8.8.8.8", "53", UDP_CONNECT)) {
		printf("could not connect\n");
		udp_close(&local);
		return -1;
	}

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	if(setsockopt(public.fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		 perror("setsockopt");
		 udp_close(&public);
		 udp_close(&local);
		 return -1;
	}

	struct sockaddr_storage return_addr;
	socklen_t return_len;

	return_len = sizeof(return_addr);

	while((length = recvfrom(local.fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&return_addr, &return_len)) > 0) {
		if((check_res = check_request(buffer, length)) > 0) {
			sendto(local.fd, buffer, check_res, 0, (struct sockaddr *)&return_addr, return_len);
			continue;
		}
		
		sendto(public.fd, buffer, length, 0, public.info->ai_addr, public.info->ai_addrlen);
		length = recvfrom(public.fd, buffer, sizeof(buffer), 0, NULL, NULL);
		if(length <= 0) continue;

		sendto(local.fd, buffer, length, 0, (struct sockaddr *)&return_addr, return_len);
	}

	udp_close(&local);
	udp_close(&public);
	return 0;
}
