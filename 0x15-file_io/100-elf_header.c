#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

#define BUF_SIZE 128

/**
 * error_exit - Prints an error message to stderr and exits with status code 98
 * @msg: The error message to be printed
 */
void error_exit(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(98);
}

/**
 * print_elf_header - Prints the information contained in the ELF header
 * @header: Pointer to the ELF header structure
 */
void print_elf_header(Elf64_Ehdr *header)
{
    printf("Magic: ");
    for (int i = 0; i < EI_NIDENT; i++)
        printf("%02x ", header->e_ident[i]);
    printf("\n");

    printf("Class: %s\n", (header->e_ident[EI_CLASS] == ELFCLASS32) ? "ELF32" : "ELF64");

    printf("Data: %s\n", (header->e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "2's complement, big endian");

    printf("Version: %d\n", header->e_ident[EI_VERSION]);

    printf("OS/ABI: %s\n", (header->e_ident[EI_OSABI] == ELFOSABI_SYSV) ? "UNIX - System V" : "UNIX - Other");

    printf("ABI Version: %d\n", header->e_ident[EI_ABIVERSION]);

    printf("Type: %s\n", (header->e_type == ET_EXEC) ? "EXEC (Executable file)" : "Other");

    printf("Entry point address: 0x%lx\n", header->e_entry);
}

/**
 * main - Entry point of the program
 * @argc: The number of command-line arguments
 * @argv: An array of command-line argument strings
 * Return: 0 on success, 98 on error
 */
int main(int argc, char *argv[])
{
    int fd;
    Elf64_Ehdr header;

    if (argc != 2)
        error_exit("Usage: elf_header elf_filename");

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        error_exit("Error: Cannot open file");

    if (read(fd, &header, sizeof(header)) != sizeof(header))
        error_exit("Error: Cannot read ELF header");

    if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 ||
        header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3)
        error_exit("Error: Not an ELF file");

    print_elf_header(&header);

    close(fd);
    return 0;
}
