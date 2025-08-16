// bns.h
#ifndef BNS_H
#define BNS_H

#include <stdint.h>

#define BNS_MAGIC 0x424E535F  // "BNS_"

typedef struct {
    uint32_t magic;  // Identifier
    uint32_t size;   // Size of program code in bytes
} bns_header_t;

#endif
