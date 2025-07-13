#include "cxa.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>

/* Project name, for better error reporting */
static char *Project;

/* Stores the index where the flag with certain shortname
 * can be found within 'flags' array
 */
static char QuickShortNameAccess[26 * 2 + 10];

static void check_names (struct CxaFlag*);
static char get_quick_access_for (const char);

void cxa_execute (const int argc, char **argv, struct CxaFlag *flags, const char *projectName)
{
	Project = (char*) projectName;
	check_names(flags);
}

void cxa_print_usage (const char*, const struct CxaFlag*)
{
}

static void check_names (struct CxaFlag *flags)
{
	for (unsigned char i = 0; i < 62; i++)
	{
		QuickShortNameAccess[i] = -1;
	}
	for (unsigned char i = 0; flags[i].longname; i++)
	{
		const char shortname = flags[i].shortname;
		const char key       = get_quick_access_for(shortname);
		
		assert(key != -1 && "PROGRAMMER: INVALID SHORTNAME");
		assert(QuickShortNameAccess[key] == -1 && "PROGRAMMER: REPEATED SHORTNAMES");

		QuickShortNameAccess[key] = i;
		const char *longname = flags[i].longname;

		for (unsigned char j = i + 1; flags[j].longname; j++)
		{
			assert(strcmp(longname, flags[j].longname) != 0 && "PROGRAMMER: REPEATED LONGNAMES");
		}
	}
}

static char get_quick_access_for (const char a)
{
	if (isdigit(a)) { return a - '0'; }
	if (islower(a)) { return a - 'a' + 10; }
	if (isupper(a)) { return a - 'A' + 35; }

	return -1;
}
