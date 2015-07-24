#include "PluginObject.h"
#include <stdio.h>


#define PLUGIN_NAME		"System Plugin"
#define PLUGIN_DESCRIPTION	"Execute Native Binary Plugin version 1.0"
#define PLUGIN_MIME		"application/x-system::Execute Native Binary Plugin from Peking University"

NPNetscapeFuncs* browser;


NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16 mode, int16 argc,
	char* argn[], char* argv[], NPSavedData* saved)
{
	if (browser->version >= 14)
		instance->pdata = browser->createobject(instance, getPluginClass());
	return NPERR_NO_ERROR;
}

NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
	return NPERR_NO_ERROR;
}

NPError NPP_SetWindow(NPP instance, NPWindow* window)
{
	return NPERR_NO_ERROR;
}

NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream,
	NPBool seekable, uint16* stype)
{
	*stype = NP_ASFILEONLY;
	return NPERR_NO_ERROR;
}

NPError NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	return NPERR_NO_ERROR;
}

int32 NPP_WriteReady(NPP instance, NPStream* stream)
{
	return 0;
}

int32 NPP_Write(NPP instance, NPStream* stream, int32 offset, int32 len,
	void* buffer)
{
	return 0;
}

void NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
}

void NPP_Print(NPP instance, NPPrint* platformPrint)
{
}

int16 NPP_HandleEvent(NPP instance, void* event)
{
	return 0;
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason,
	void* notifyData)
{
}

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
	switch (variable) {
	case NPPVpluginNameString:
		*((char **)value) = PLUGIN_NAME;
		return NPERR_NO_ERROR;

	case NPPVpluginDescriptionString:
		*((char **)value) = PLUGIN_DESCRIPTION;
		return NPERR_NO_ERROR;

	case NPPVpluginNeedsXEmbed:
		*((NPBool *)value) = TRUE;
		return NPERR_NO_ERROR;

	case NPPVpluginScriptableNPObject:
		browser->retainobject((NPObject*)instance->pdata);
		*((void **)value) = instance->pdata;
		return NPERR_NO_ERROR;

	default:
		return NPERR_GENERIC_ERROR;
	}
	return NPERR_GENERIC_ERROR;
}

NPError NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
	return NPERR_GENERIC_ERROR;
}

NPError NP_GetValue(void* future, NPPVariable variable, void *value)
{
	return NPP_GetValue(future, variable, value);
}

NPError NP_GetEntryPoints(NPPluginFuncs* pluginFuncs)
{
	pluginFuncs->version		= 11;
	pluginFuncs->size		= sizeof(pluginFuncs);
	pluginFuncs->newp		= NPP_New;
	pluginFuncs->destroy		= NPP_Destroy;
	pluginFuncs->setwindow		= NPP_SetWindow;
	pluginFuncs->newstream		= NPP_NewStream;
	pluginFuncs->destroystream	= NPP_DestroyStream;
	pluginFuncs->asfile		= NPP_StreamAsFile;
	pluginFuncs->writeready		= NPP_WriteReady;
	pluginFuncs->write		= (NPP_WriteProcPtr)NPP_Write;
	pluginFuncs->print		= NPP_Print;
	pluginFuncs->event		= NPP_HandleEvent;
	pluginFuncs->urlnotify		= NPP_URLNotify;
	pluginFuncs->getvalue		= NPP_GetValue;
	pluginFuncs->setvalue		= NPP_SetValue;
	
	return NPERR_NO_ERROR;
}

NPError NP_Initialize(NPNetscapeFuncs* browserFuncs, NPPluginFuncs* pluginFuncs)
{
	browser = browserFuncs;
	NP_GetEntryPoints(pluginFuncs);
	return NPERR_NO_ERROR;
}

char *NP_GetMIMEDescription(void)
{
	return (char *)PLUGIN_MIME;
}

void NP_Shutdown(void)
{
}

#if 0
#pragma export on
NPError NP_Initialize(NPNetscapeFuncs* browserFuncs, NPPluginFuncs* pluginFuncs);
NPError NP_GetEntryPoints(NPPluginFuncs *pluginFuncs);
void NP_Shutdown(void);
#pragma export off
#endif
