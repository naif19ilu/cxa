#define CXA_HEADER_ONLY
#include "cxa.h"

#include <stdio.h>

int main (int argc, char **argv)
{
	struct CxaFlag flag[] =
	{
		{
			"document",
			"file to work with",
			NULL,
			'd',
			CXA_FLAG_NEEDS_ARG_YES,
			CXA_FLAG_NOT_SEEN
		},
		{
			"verbose",
			"display operations as they happen",
			NULL,
			'v',
			CXA_FLAG_NEEDS_ARG_NON,
			CXA_FLAG_NOT_SEEN
		},
		{
			"usage",
			"print program's usage or flag's usage",
			NULL,
			'u',
			CXA_FLAG_NEEDS_ARG_MAY,
			CXA_FLAG_NOT_SEEN
		},
		CXA_FINAL_FLAG
	};

	cxa_parse(flag, (unsigned int) argc, argv);

	return 0;
}
