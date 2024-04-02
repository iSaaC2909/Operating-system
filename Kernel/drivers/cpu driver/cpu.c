#include <stddef.h>

// Driver interface for the CPU driver\
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef struct {
    void (*init)(void);
    void (*interrupt)(void);
    void (*io)(uint8_t* data, size_t size);
    void (*cleanup)(void);
} cpu_driver;

// Driver implementation
static cpu_driver cpu_driver = {
    .init = cpu_init,
    .interrupt = cpu_interrupt,
    .io = cpu_io,
    .cleanup = cpu_cleanup,
};

//error handling
#include <stddef.h>

// ... your existing code ...

// Error handling
void error_handler(const char *message) {
    // Print an informative error message
    puts(message);

    // If available, provide additional system-specific error information
    #ifdef __GNUC__
       puts(__func__); // Print the function name where the error occurred
    #endif

    // Attempt cleanup before exiting
    cleanup();
    exit(1);
}



void enable_paging() {
    uint32_t cr0 = read_cr0();
    cr0 = 0x8000000;
    write_cr0(cr0);

    uint32_t page_directory[1024];
    memset(page_directory, 0, 1024 * sizeof(uint32_t));

    uint32_t page_table[1024];
    memset(page_table, 0, 1024 * sizeof(uint32_t));

    uint32_t physical_address = 0x00000000;
    uint32_t virtual_address = 0x00000000;

    page_directory[PT_INDEX(virtual_address)] =
        ((uint32_t)page_table | PG_PRESENT | PG_WRITABLE);

    page_table[PT_INDEX(physical_address)] =
        (physical_address & PTE_MASK);

    // Enable PAE
    uint32_t cr4;
    cr4 = read_cr4();
    cr4 |= 0x00000020; // Set PAE flag
    write_cr4(cr4);

    // Set page directory
    write_cr3((uint32_t)page_directory);

    // Enable paging
    uint32_t cr0;
    cr0 = read_cr0();
    cr0 |= 0x80000000; // Set PG flag
    write_cr0(cr0);

     if (read_cr0() != 0x8000000) {
       error_handler("Failed to set CR0 register for paging");
   }

   if (read_cr4() != 0x00000020) {
       error_handler("Failed to enable PAE");
   }
}
