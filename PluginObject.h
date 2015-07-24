#ifndef SYSTEM_PLUGIN_OBJECT_H
#define SYSTEM_PLUGIN_OBJECT_H

#include "SystemObject.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PluginObject {
	NPObject header;
	NPP npp;
	SystemObject *systemObject;
} PluginObject;

NPClass *getPluginClass(void);

#ifdef __cplusplus
}
#endif
#endif

