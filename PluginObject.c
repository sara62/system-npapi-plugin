#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PluginObject.h"

static bool identifiersInitialized = false;

#define PLUGIN_PROPERTY_SYSTEM			0
#define PLUGIN_NUM_PROPERTIES			1

static NPIdentifier pluginPropertyIdentifiers[PLUGIN_NUM_PROPERTIES];
static const NPUTF8 *pluginPropertyNames[PLUGIN_NUM_PROPERTIES] = {
	"system",
};

#define PLUGIN_METHOD_GETTOKEN			0
#define PLUGIN_NUM_METHODS			1

static NPIdentifier pluginMethodIdentifiers[PLUGIN_NUM_METHODS];
static const NPUTF8 *pluginMethodNames[PLUGIN_NUM_METHODS] = {
	"getToken"
};

static void initializeIdentifiers(void)
{
	browser->getstringidentifiers(pluginPropertyNames,
		PLUGIN_NUM_PROPERTIES, pluginPropertyIdentifiers);
	browser->getstringidentifiers(pluginMethodNames,
		PLUGIN_NUM_METHODS, pluginMethodIdentifiers);
}

bool pluginHasProperty(NPObject *obj, NPIdentifier name)
{
	int i;
	for (i = 0; i < PLUGIN_NUM_PROPERTIES; i++)
		if (name == pluginPropertyIdentifiers[i])
			return true;
	return false;
}

bool pluginHasMethod(NPObject *obj, NPIdentifier name)
{
	int i;
	for (i = 0; i < PLUGIN_NUM_METHODS; i++)
		if (name == pluginMethodIdentifiers[i])
			return true;
	return false;
}

bool pluginGetProperty(NPObject *obj, NPIdentifier name, NPVariant *variant)
{
	PluginObject *plugin = (PluginObject *)obj;

	if (name == pluginPropertyIdentifiers[PLUGIN_PROPERTY_SYSTEM]) {
		NPObject *resultObj = &plugin->systemObject->header;
		browser->retainobject(resultObj);
		OBJECT_TO_NPVARIANT(resultObj, *variant);
		return true;
	}

	return false;
}

bool pluginSetProperty(NPObject *obj, NPIdentifier name, const NPVariant *variant)
{
	return false;
}

bool pluginInvoke(NPObject *obj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}

bool pluginInvokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}

void pluginInvalidate(NPObject *obj)
{
	// Release any remaining references to JavaScript objects.
}

NPObject *pluginAllocate(NPP npp, NPClass *theClass)
{
	PluginObject *newInstance = malloc(sizeof(PluginObject));
	
	if (!identifiersInitialized) {
		identifiersInitialized = true;
		initializeIdentifiers();
	}
	newInstance->systemObject = 
		(SystemObject *)browser->createobject(npp, getSystemClass());
	newInstance->npp = npp;

	return &newInstance->header;
}

void pluginDeallocate(NPObject *obj) 
{
	free(obj);
}

static NPClass pluginClass = { 
	NP_CLASS_STRUCT_VERSION,
	pluginAllocate, 
	pluginDeallocate, 
	pluginInvalidate,
	pluginHasMethod,
	pluginInvoke,
	pluginInvokeDefault,
	pluginHasProperty,
	pluginGetProperty,
	pluginSetProperty,
};
 
NPClass *getPluginClass(void)
{
	return &pluginClass;
}
