/*
 * This file is for generate the firmware for sigma
 * author: Bob.liu@analog.com
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

#include "gen_firmware.h"
#include "crc32.h"

static u8 sigma_prog[MAX_LEN];
static u8 sigma_param[MAX_LEN];

static u32 sigma_program_size;
static u32 sigma_param_size;

int get_param() {
	int fd;
	fd = open("parameter.bin",O_RDONLY);
	if( fd < 0) {
		printf("open parameter.bin error\n");
		return -1;
	}
	sigma_param_size = read(fd, sigma_param, MAX_LEN);
	close(fd);
	return 0;
}

int get_prog() {
	int fd;
	fd = open("program.bin",O_RDONLY);
	if( fd < 0) {
		printf("open program.bin error\n");
		return -1;
	}
	sigma_program_size = read(fd, sigma_prog, MAX_LEN);
	close(fd);
	return 0;
}

/* Got the parameter.bin and program.bin by "Save as Raw Data"->
 * "Adress+Data" in SigmaStudio.
 */

int main(int argc, char **argv)
{
	int fd;
	struct sigma_firmware_header head;
	struct sigma_action *sa_param;
	struct sigma_action *sa_program;
	u32 param_size;
	u32 program_size;
	u32 crc;

	if(get_prog() < 0)
		return -1;
	if(get_param() < 0)
		return -1;

	param_size = sizeof(struct sigma_action) + sigma_param_size + (sigma_param_size % 2);
	program_size = sizeof(struct sigma_action) + sigma_program_size + (sigma_program_size % 2);

	//
	sa_param = (struct sigma_action *)malloc(param_size);
	if(!sa_param) {
		printf("malloc param payload error\n");
		return -1;
	}
	sa_param->len =  htole16(sigma_param_size & 0xffff);
	sa_param->len_hi = (sigma_param_size >> 16) & 0xf;
  	sa_param->instr = SIGMA_ACTION_WRITEXBYTES;
	/* I2C transfer starts from MSB */
	sa_param->addr = htobe16((sigma_param[0] << 8) | sigma_param[1]);
	memcpy(sa_param->payload, &sigma_param[2], sigma_param_size - 2);

	//
	sa_program = (struct sigma_action *)malloc(program_size);
	if(!sa_program) {
		printf("malloc prog payload error\n");
		free(sa_param);
		return -1;
	}
	sa_program->len = htole16(sigma_program_size & 0xffff);
	sa_program->len_hi = (sigma_program_size >> 16) & 0x0f;
  	sa_program->instr = SIGMA_ACTION_WRITEXBYTES;
	/* I2C transfer starts from MSB */
	sa_program->addr = htobe16((sigma_prog[0] << 8) | sigma_prog[1]);
	memcpy(sa_program->payload, &sigma_prog[2], sigma_program_size - 2);

	crc = crc32(sa_program, program_size, 0);
	crc = crc32(sa_param, param_size, crc);

	/* init head */
	memcpy(head.magic, SIGMA_MAGIC, sizeof(SIGMA_MAGIC));
	head.version = 1;
	head.crc = htole32(crc);

	/* write to file */
	fd = open(OUTPUT_FILE, O_CREAT|O_RDWR|O_TRUNC, S_IRWXU);
	if(fd < 0) {
		printf("create file error\n");
		return -1;
	}

	write(fd, &head, sizeof(head));
	write(fd, sa_program, program_size);
	write(fd, sa_param, param_size);

	close(fd);
	free(sa_program);
	free(sa_param);
	return 0;
}
