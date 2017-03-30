/*
 * Sigma Firmware debug helper: dump a sigma firmware binary blob
 * into some human readable strings.
 *
 * Written by Mike Frysinger <vapier@gentoo.org>
 * Copyright (C) 2009-2010 Analog Devices Inc.
 * Licensed under the ADI BSD license:
 *  http://docs.blackfin.uclinux.org/doku.php?id=adi_bsd
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>

struct sigma_firmware {
	unsigned char magic[7];
	uint8_t version;
	uint32_t crc;
};
#define SIZEOF_sf (7 + 1 + 4)
struct sigma_action {
	uint8_t instr;
	uint8_t len_hi;
	uint16_t len;
	uint16_t addr;
	unsigned char payload[];
};
#define LEN_sa(sa) ((uint32_t)(((sa)->len_hi << 16) | le16toh((sa)->len)))
#define LEN_sa_payload(sa) (LEN_sa(sa) ? LEN_sa(sa) - 2 : 0)
#define SIZEOF_sa(sa) (1 + 1 + 2 + 2 + LEN_sa_payload(sa) + LEN_sa_payload(sa) % 2)

void dump(const unsigned char *buf, int x)
{
	while (x--)
		printf("'%c'%s ", *buf++, x ? "," : "");
}

void dumpx(const unsigned char *buf, int x)
{
	while (x--)
		printf("0x%02x%s ", *buf++, x ? "," : "");
}

const char *decode_inst(unsigned int inst)
{
	switch (inst) {
		case 0: return "writexbytes";
		case 1: return "writesingle";
		case 2: return "writesafeload";
		case 3: return "delay";
		case 4: return "pllwait";
		case 5: return "noop";
		case 6: return "end";
		default: return "???";
	}
}

int main(int argc, char *argv[])
{
	int i, j;
	FILE *fp;
	const char *file;
	char buf[1024];
	struct sigma_firmware *sf;
	struct sigma_action *sa;
	size_t bytes;

	memset(buf, 0xad, sizeof(buf));

	printf("sizeof(sf) = %zu vs SIZEOF_sf = %i\n", sizeof(*sf), SIZEOF_sf);
	printf("sizeof(sa) = %zu\n", sizeof(*sa));

	for (i = 1; i < argc; ++i) {
		file = argv[i];
		printf("firmware blob decode: %s\n", file);
		fp = fopen(file, "r");
		bytes = fread(buf, 1, sizeof(buf), fp);

		sf = (void *)buf;

		printf(
			"struct sigma_firmware sf {\n"
			"	unsigned char magic[7] = { "
		);
		dump(sf->magic, 7);
		printf("};\n");
		printf(
			"	uint8_t version = %i;\n"
			"	uint32_t crc = 0x%08x;\n"
			"};\n",
			sf->version, le32toh(sf->crc));

		j = 0;
		sa = (void *)(buf + SIZEOF_sf);
		while (bytes > 0) {
			bytes -= SIZEOF_sf;

			printf(
				"struct sigma_action sa%i = {\n"
				"	uint8_t instr  = %#x /* %u (%s) */;\n"
				"	uint8_t len_hi = %#x /* %u */;\n"
				"	uint16_t len   = %#x /* %u */;\n"
				"	uint16_t addr  = %#x /* %u */;\n",
				j,
				sa->instr, sa->instr, decode_inst(sa->instr),
				sa->len_hi, sa->len_hi,
				le16toh(sa->len), le16toh(sa->len),
				be16toh(sa->addr), be16toh(sa->addr));

			if (sa->instr == 3)
				sa->len = 0;

			printf("	unsigned char payload[%u]", LEN_sa_payload(sa));
			if (LEN_sa_payload(sa)) {
				printf(" = {\n\t\t");
				dumpx(sa->payload, LEN_sa_payload(sa));
				printf("\n");
				printf("	};\n");
			} else
				printf(";\n");
			printf("};\n");

			sa = (void *)sa + SIZEOF_sa(sa);
			++j;
		}
	}

	return 0;
}
