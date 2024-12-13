#include "lab4.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tbl_sz 16
#define pg_tbl_sz 256
#define f_sz 256
#define m_sz 256 * f_sz

// TLB Entry Structure
typedef struct {
    int page_number;
    int frame_number;
} TLBEntry;

// Global Variables
TLBEntry tlb[tbl_sz];
int page_table[pg_tbl_sz];
signed char physical_memory[m_sz];
int tlb_index = 0;
int next_frame = 0;

// Function Prototypes
void initialize();
int tlb_lookup(int page_number);
void tlb_update(int page_number, int frame_number);
int page_table_lookup(int page_number);
int handle_page_fault(int page_number);
int translate_logical_address(int logical_address, int* physical_address);
signed char read_from_physical_memory(int physical_address);

// Function Implementations

void initialize() {
    memset(tlb, -1, sizeof(tlb));
    memset(page_table, -1, sizeof(page_table));
    memset(physical_memory, 0, sizeof(physical_memory));
}

int tlb_lookup(int page_number) {
    for (int i = 0; i < tbl_sz; i++) {
        if (tlb[i].page_number == page_number) {
            return tlb[i].frame_number;
        }
    }
    return -1;
}

void tlb_update(int page_number, int frame_number) {
    tlb[tlb_index].page_number = page_number;
    tlb[tlb_index].frame_number = frame_number;
    tlb_index = (tlb_index + 1) % tbl_sz;
}

int page_table_lookup(int page_number) {
    return page_table[page_number];
}

int handle_page_fault(int page_number) {
    int frame_number = next_frame;
    next_frame++;

    if (next_frame >= 256) {
        fprintf(stderr, "Error: memory\n");
        exit(1);
    }

    page_table[page_number] = frame_number;
    return frame_number;
}

int translate_logical_address(int logical_address, int* physical_address) {
    int page_number = (logical_address >> 8) & 0xFF;
    int offset = logical_address & 0xFF;

    int frame_number = tlb_lookup(page_number);

    if (frame_number == -1) {
        frame_number = page_table_lookup(page_number);
        
        if (frame_number == -1) {
            frame_number = handle_page_fault(page_number);
        }
        
        tlb_update(page_number, frame_number);
    }

    *physical_address = (frame_number << 8) | offset;
    return frame_number;
}

signed char read_from_physical_memory(int physical_address) {
    return physical_memory[physical_address];
}
