#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SystemObject.h"


const char *prog = "system";
static bool identifiersInitialized = false;

#define SYSTEM_VERSION				"0.6"

#define SYSTEM_PROPERTY_VERSION			0
#define SYSTEM_NUM_PROPERTIES			1

static NPIdentifier systemPropertyIdentifiers[SYSTEM_NUM_PROPERTIES];
static const NPUTF8 *systemPropertyNames[SYSTEM_NUM_PROPERTIES] = {
	"version",
};

#define SYSTEM_METHOD_EXEC			0
#define SYSTEM_NUM_METHODS			1

static NPIdentifier systemMethodIdentifiers[SYSTEM_NUM_METHODS];
static const NPUTF8 *systemMethodNames[SYSTEM_NUM_METHODS] = {
	"exec",
};

static NPObject *systemAllocate(NPP npp, NPClass *theClass)
{
	SystemObject *newInstance = (SystemObject *)malloc(sizeof(SystemObject));

	if (!identifiersInitialized) {

		browser->getstringidentifiers(systemPropertyNames,
			SYSTEM_NUM_PROPERTIES, systemPropertyIdentifiers);

		browser->getstringidentifiers(systemMethodNames,
			SYSTEM_NUM_METHODS, systemMethodIdentifiers);

		identifiersInitialized = true;
	}

	return &newInstance->header;
}

static void systemDeallocate(NPObject *obj) 
{
	free(obj);
}

static void systemInvalidate(NPObject *obj)
{
}

static bool systemHasMethod(NPObject *obj, NPIdentifier name)
{
	int i;
	for (i = 0; i < SYSTEM_NUM_METHODS; i++)
		if (name == systemMethodIdentifiers[i])
			return true;
	return false;
}

static bool do_exec(const NPVariant cmd, NPVariant *result);
static bool systemInvoke(NPObject *obj, NPIdentifier name, const NPVariant *args,
	uint32_t argCount, NPVariant *variant)
{

	if (name == systemMethodIdentifiers[SYSTEM_METHOD_EXEC]) {
		if (argCount != 1) {
			fprintf(stderr, "%s: bad arguments", prog);
			return false;
		}
		return do_exec(args[0], variant);
	}

	return false;
}

static bool systemInvokeDefault(NPObject *obj, const NPVariant *args,
	uint32_t argCount, NPVariant *result)
{
	return false;
}

static bool systemHasProperty(NPObject *obj, NPIdentifier name)
{
	int i;
	for (i = 0; i < SYSTEM_NUM_PROPERTIES; i++)
		if (name == systemPropertyIdentifiers[i])
			return true;
	return false;
}

static bool systemGetProperty(NPObject *obj, NPIdentifier name, NPVariant *variant)
{
	if (name == systemPropertyIdentifiers[SYSTEM_PROPERTY_VERSION]) {
		STRINGZ_TO_NPVARIANT(strdup(SYSTEM_VERSION), *variant);
		return true;
	}
	return false;
}

static bool systemSetProperty(NPObject *obj, NPIdentifier name, 
	const NPVariant *variant)
{
	return false;
}

static NPClass systemClass = { 
	NP_CLASS_STRUCT_VERSION,
	systemAllocate, 
	systemDeallocate, 
	systemInvalidate,
	systemHasMethod,
	systemInvoke,
	systemInvokeDefault,
	systemHasProperty,
	systemGetProperty,
	systemSetProperty,
};

NPClass *getSystemClass(void)
{
	return &systemClass;
}



static bool do_exec(const NPVariant cmd, NPVariant *result)
{
	bool ret = false;
	const char *cmd_str;
	char *out_str;
	FILE *fp = NULL;
	char buf[1024 * 1024];

	/* set default return value */
	NULL_TO_NPVARIANT(*result);
		
	/* init algor */
	if (!NPVARIANT_IS_STRING(cmd)) {
		fprintf(stderr, "%s (%s %d): invalid argument type\n", prog, __FILE__, __LINE__);
		goto end;
	}	
	if (!(cmd_str = NPVARIANT_TO_STRING(cmd).UTF8Characters)) {
		fprintf(stderr, "%s (%s %d): invalid argument value\n", prog, __FILE__, __LINE__);
		goto end;
	}
	if (strlen(cmd_str) <= 0) {
		fprintf(stderr, "%s (%s %d): invalid argument value\n", prog, __FILE__, __LINE__);
		goto end;
	}

	if (!(fp = popen(cmd_str, "r"))) {
		fprintf(stderr, "%s (%s %d): execute `%s' failed\n", prog, __FILE__, __LINE__, cmd_str);
		goto end;
	}

	buf[0] = 0;
	fread(buf, 1, sizeof(buf), fp);
	
	if (!(out_str = strdup(buf))) {
		fprintf(stderr, "%s (%s %d): inner error\n", prog, __FILE__, __LINE__);
		goto end;
	}

	/* set output */
	STRINGZ_TO_NPVARIANT(out_str, *result);

	ret = true;

end:	
	if (fp) pclose(fp);
	return ret;		
}
