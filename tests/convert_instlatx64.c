/* This program can be used to convert CPUID Dump from http://instlatx64.atw.hu/
 * To build: gcc convert_instlatx64.c -o convert_instlatx64
 * To run: ./convert_instlatx64 <input file (from instlatx64)> <output file (without extension)>
 * Then, you can use the create_test.py script to create the test file */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define LINE_LEN     53
#define FILENAME_LEN 128
#define CMD_LEN      256
#define EXT_CPUID    0x80000000

int main(int argc, char *argv[])
{
	uint32_t addr, prev_addr, eax, ebx, ecx, edx;
	char line[LINE_LEN], raw_filename[FILENAME_LEN], report_filename[FILENAME_LEN], cmd[CMD_LEN];
	FILE *fin = NULL, *fout = NULL;

	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s <input file (from instlatx64)> <output file (without extension)>\n", argv[0]);
		return 1;
	}

	const char *input_filename  = argv[1];
	const char *output_filename = argv[2];
	snprintf(raw_filename,    FILENAME_LEN, "%s.raw",        output_filename);
	snprintf(report_filename, FILENAME_LEN, "%s_report.txt", output_filename);

	/* Open files */
	if((fin  = fopen(input_filename, "r")) == NULL)
	{
		perror("Failed to open input file");
		return 1;
	}
	if((fout = fopen(raw_filename, "w")) == NULL)
	{
		perror("Failed to open output file");
		return 1;
	}

	/* Parse and convert CPUID dump */
	prev_addr = -1;
	while(fgets(line, LINE_LEN, fin) != NULL)
	{
		sscanf(line, "CPUID %x: %x-%x-%x-%x", &addr, &eax, &ebx, &ecx, &edx);
		if(((addr < EXT_CPUID) && (addr >= 32)) || ((addr >= EXT_CPUID) && (addr - EXT_CPUID >= 32)) || (addr == prev_addr))
			continue;

		if(addr < EXT_CPUID)
			fprintf(fout, "basic_cpuid[%d]", addr);
		else
			fprintf(fout, "ext_cpuid[%d]", addr - EXT_CPUID);
		fprintf(fout, "=%08x %08x %08x %08x\n", eax, ebx, ecx, edx);
		prev_addr = addr;
	}

	/* Invoke cpuid_tool */
	fclose(fout);
	snprintf(cmd, CMD_LEN, "cpuid_tool --load=%s --report --outfile=%s", raw_filename, report_filename);
	system(cmd);

	/* Invoke create_test */
	snprintf(cmd, CMD_LEN, "./create_test.py %s %s > %s.test", raw_filename, report_filename, output_filename);
	if((argc > 3) && !strcmp(argv[3], "--create"))
	{
		system(cmd);
		remove(raw_filename);
		remove(report_filename);
	}
	else
	{
		printf("Done. Use the following command to create the test file:\n");
		printf("%s\n", cmd);
	}

	return 0;
}
