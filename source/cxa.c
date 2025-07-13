#include "cxa.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

/* Refers to the name of the major project which is making use of the
 * library, set on 'cxa_execute' function
 */
static char *Project     = "unnamed";

/* Refers to the last flag seen as long as the flag needs an argument
 * even if it is optional
 */
static struct CxaFlag *LastSeen = NULL;

/* Stores the index where the flag with certain shortname
 * can be found within 'flags' array
 */
static short QuickShortNameAccess[26 * 2 + 10];

static void error_undefined_shortname (const char name, const char *group)
{
	const char *const fmt =
	"cxa:%s:\x1b[31merror:\x1b[0m undefined flag provided\n"
	"   '-%c' cannot be recognized as a program's argument\n"
	"   found in '%s' group\n";
	fprintf(stderr, fmt, Project, name, group);
	exit(EXIT_FAILURE);
}

static void error_multi_taker_in_group (const char first, const char second, const char *group)
{
	const char *const fmt =
	"cxa:%s:\x1b[31merror:\x1b[0m more than one flag takes argument in this group\n"
	"   both '-%c' and '-%c' take argument (found in '%s' group)\n"
	"   fix: ... -%c <arg> -%c <arg>\n";
	fprintf(stderr, fmt, Project, first, second, group, first, second);
	exit(EXIT_FAILURE);
}


static void check_names (struct CxaFlag*);
static short get_quick_access_for (const char);

static void handle_short_flag (struct CxaFlag*, const char*, const size_t);

void cxa_execute (const unsigned char argc, char **argv, struct CxaFlag *flags, const char *projectName)
{
	Project = (char*) projectName;
	check_names(flags);

	bool endOfArgs = false;

	for (unsigned char i = 1; i < argc; i++)
	{
		const char *this = argv[i];
		const size_t len = strlen(this);

		if (endOfArgs)
		{
		}
		else if (len == 2 && *this == '-' && this[1] == '-')
		{
			endOfArgs = true;
			continue;
		}
		else if (len >  2 && *this == '-' && this[1] == '-')
		{
		}
		else if (len == 1 && *this == '-')
		{
		}
		else if (len >  1 && *this == '-')
		{
			handle_short_flag(flags, this, len);
		}
		else
		{
		}
	}
}

void cxa_print_usage (const char *desc, const struct CxaFlag *flags)
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
		const short key      = get_quick_access_for(shortname);
		
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

static short get_quick_access_for (const char a)
{
	if (isdigit(a)) { return a - '0'; }
	if (islower(a)) { return a - 'a' + 10; }
	if (isupper(a)) { return a - 'A' + 36; }

	return -1;
}

static void handle_short_flag (struct CxaFlag *flags, const char *given, const size_t len)
{
	for (size_t i = 1; i < len; i++)
	{
		const char name = given[i];
		const short key = get_quick_access_for(name);

		if (key == -1 || QuickShortNameAccess[key] == -1)
		{
			error_undefined_shortname(name, given);
		}

		struct CxaFlag *flag = &flags[QuickShortNameAccess[key]];
		flag->seen = CXA_FLAG_WAS_SEEN;

		if (flag->has != CXA_FLAG_TAKER_NON && LastSeen)
		{
			error_multi_taker_in_group(LastSeen->shortname, name, given);
		}
		if (flag->has != CXA_FLAG_TAKER_NON)
		{
			LastSeen = flag;
		}
	}
}
