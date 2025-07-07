/*
 * Cxa - Command line argument parser for C
 * Jul 6, 2025
 * cxa's defintions and source code if wanted
 */
#ifndef CXA_H
#define CXA_H

#ifdef __cplusplus
extern "C" {
#endif

#define CXA_FINAL_FLAG {NULL, NULL, NULL, 0, 0, 0}

enum CxaFlagSeen
{
	CXA_NOT_SEEN = 0,
	CXA_WAS_SEEN = 1,
};

enum CxaFlagArgument
{
	CXA_FLAG_ARGUMENT_NON = 0,
	CXA_FLAG_ARGUMENT_MAY = 1,
	CXA_FLAG_ARGUMENT_YES = 2
};

struct CxaFlag
{
	char *longname;
	char *argument;
	char *description;
	char shortname;
	enum CxaFlagSeen seen;
	enum CxaFlagArgument needs;
};

void cxa_parse_arguments (struct CxaFlag*, const int, char**);

#ifdef __cplusplus
}
#endif
#endif

#ifdef CXA_HEADER_ONLY

/* TODO */

#endif
