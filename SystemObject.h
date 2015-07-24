#ifndef SYSTEM_OBJECT_H
#define SYSTEM_OBJECT_H

#include "npapi.h"
#include "npruntime.h"
#include "npfunctions.h"

#ifdef __cplusplus
extern "C" {
#endif

extern NPNetscapeFuncs* browser;

typedef struct SystemObject {
    NPObject header;
} SystemObject;

NPClass *getSystemClass(void);

#ifdef __cplusplus
}
#endif
#endif

