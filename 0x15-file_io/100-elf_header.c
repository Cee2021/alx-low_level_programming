#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void close_elf(int elf)
{
    close(elf);
}

void print_magic()
{
    /* Function implementation ... */
}

void print_class()
{
    /* Function implementation ... */
}

void print_data()
{
    /* Function implementation ... */
}

void print_version()
{
    /* Function implementation ... */
}

void print_abi()
{
    /* Function implementation ... */
}

void print_osabi()
{
    /* Function implementation ... */
}

void print_type(unsigned int e_type)
{
    /* Function implementation ... */
    printf("Type: %u\n", e_type);
}

void print_entry(unsigned long int e_entry)
{
    /* Function implementation ... */
    printf("Entry: %lu\n", e_entry);
}

/* Declare the dprintf function */
int dprintf(int fd, const char *format, ...);

/**
 * Checks if a file is an ELF file.
 * @param e_ident A pointer to an array containing the ELF magic numbers.
 *
 * Description: If the file is not an ELF file - exit code 98.
 */
void check_elf(unsigned char *e_ident)
{
    int index;
    for (index = 0; index < 4; index++)
    {
        if (e_ident[index] != 127 &&
            e_ident[index] != 'E' &&
            e_ident[index] != 'L' &&
            e_ident[index] != 'F')
        {
            dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
            exit(98);
        }
    }
}

/* Function definitions... */

int main(int __attribute__((__unused__)) argc, char *argv[])
{
    Elf64_Ehdr *header;
    int o, r;

    o = open(argv[1], O_RDONLY);
    if (o == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
        exit(98);
    }

    header = malloc(sizeof(Elf64_Ehdr));
    if (header == NULL)
    {
        close_elf(o);
        dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
        exit(98);
    }

    r = read(o, header, sizeof(Elf64_Ehdr));
    if (r == -1)
    {
        free(header);
        close_elf(o);
        dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", argv[1]);
        exit(98);
    }

    check_elf(header->e_ident);
    printf("ELF Header:\n");
    print_magic();
    print_class();
    print_data();
    print_version();
    print_osabi();
    print_abi();
    print_type(header->e_type);
    print_entry(header->e_entry);

    free(header);
    close_elf(o);
    return 0;
}

