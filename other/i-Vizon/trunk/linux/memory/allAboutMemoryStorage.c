#include <stdio.h>
#include <string.h>

/**
 *      |----------------------------------------
 *      |High Address: cmd line args & env vars
 *      |----------------------------------------
 *      | STACK (LIFO) : auto variables and some register variables
 *      |---------------------------------------
 *      |                   |
 *      |                   |
 *      |                  \ /
 *      |                   -
 *      |
 *      |                   -
 *      |                  / \
 *      |                   |
 *      |                   |
 *      |----------------------------------------
 *      | HEAP : malloc, calloc, ralloc, free : brk/sbr/mmap
 *      |           aka: shared memory
 *      |----------------------------------------
 *      | Uninitialized data              initialized to zero by exec
 *      | (bss) “block started by symbol” global, static variables
 *      |                                 that are initialized to zero or
 *      |                                 do not have explicit initialization in source code.
 *      |----------------------------------------
 *      | initialized data : has global and non zero initialized static variables
 *      |   (data)           const, extern in virtual memory space
 *      |                    Ronly(const) & RW
 *      |------------------------------------->read from program file by exec
 *      |
 *      |Low address:   text : Ronly,  assembly instruction
 *      |
 *      |----------------------------------------
 */
static int  IV1_global_static_integer;           // uninitialized bss
static int  IV2_global_static_integer_initialized = 10;/* initialized global variable stored in DS*/
char        IV3_global_string[] = "Mageswaran"; // initialized data RW
static char IV4_global_static_string[] = "Mageswaran"; // initialized data RW
char*       IV5_global_string_ptr = "Mageswaran";      // initialized data RW
int         IV6_global_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // initialized data RW
int         IV7_global_integer;                  /* Uninitialized variable stored in bss*/
const char* IV8_global_const_char = "Mageswara"; // initialized data Ronly

/// @TIPS: Global variable names are stored in output binary except static variables
/// @TIPS: stack frame : stores all function call details and data with bare minimum
///        of return address
int main(int argv, char* argc)
{
    int     IV_local_integer;
    static  IV_local_static;    /* Uninitialized static variable stored in bss */
    int     IV_local_integer_initialized = 1; // data segment
    static int IV_local_static_initialized = 5; /* Initialized static variable stored in DS*/
    char*   IV_local_sting_ptr = "Mageswaran";
    const char*   IV_local_const_string_ptr = "Mageswaran";

    printf("Time to run some tools and check the memory layout\n");
    /**
     * size allAboutMemoryStorage
     * readelf -a allAboutMemoryStorage
     */
}
