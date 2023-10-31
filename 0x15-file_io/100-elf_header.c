/*
** 100-elf_header.c
**
** Display the information contained in the ELF header at the start of an ELF file.
**
** Usage: elf_header elf_filename
**
** Format: the same as readelf -h (version 2.26.1)
**
** If the file is not an ELF file, or on error, exit with status code 98 and display a comprehensive error message to stderr.
**
** You are allowed to use lseek once and read a maximum of two times at runtime.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <elf.h>

static void print_elf_header(const Elf64_Ehdr *ehdr)
{
	printf("Magic:   %08x\n", ehdr->e_ident[EI_MAG0] |
	                       (ehdr->e_ident[EI_MAG1] << 8) |
	                       (ehdr->e_ident[EI_MAG2] << 16) |
	                       (ehdr->e_ident[EI_MAG3] << 24));
	printf("Class:    %d\n", ehdr->e_ident[EI_CLASS]);
	printf("Data:     %d\n", ehdr->e_ident[EI_DATA]);
	printf("Version:  %d\n", ehdr->e_ident[EI_VERSION]);
	printf("OS/ABI:    %d\n", ehdr->e_ident[EI_OSABI]);
	printf("ABI Version: %d\n", ehdr->e_ident[EI_ABIVERSION]);
	printf("Type:      %d\n", ehdr->e_type);
	printf("Entry point address: %016lx\n", ehdr->e_entry);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	Elf64_Ehdr ehdr;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: elf_header elf_filename\n");
		return EXIT_FAILURE;
	}

	fp = fopen(argv[1], "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
		return EXIT_FAILURE;
	}

	/* Seek to the ELF header. */
	lseek(fileno(fp), 0, SEEK_SET);

	/* Read the ELF header. */
	if (fread(&ehdr, sizeof(ehdr), 1, fp) != 1)
	{
		fprintf(stderr, "Error reading ELF header: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	fclose(fp);

	/* Check if the file is an ELF file. */
	if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0)
	{
		fprintf(stderr, "File '%s' is not an ELF file\n", argv[1]);
		return EXIT_FAILURE;
	}

	print_elf_header(&ehdr);

	return EXIT_SUCCESS;
}

