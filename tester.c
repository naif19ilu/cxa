#include "cxa.h"
#include <stdio.h>

int main (int argc, char **argv)
{
	char *doc, *info;

	struct CxaFlag flags[] =
	{

		CXA_SET_STR("doc",  "document",    &doc,  CXA_FLAG_TAKER_YES, 'd'),
		CXA_SET_STR("inf",  "information", &info, CXA_FLAG_TAKER_MAY, 'i'),
		CXA_SET_CHR("help", "help",        NULL,  CXA_FLAG_TAKER_NON, 'h'),
		CXA_SET_END
	};

	struct Cxa *final = cxa_execute((unsigned char) argc, argv, flags, "test");
	printf("doc : <arg: %s> <seen: %d>\n", doc ,   flags[0].meta & CXA_FLAG_SEEN_MASK);
	printf("info: <arg: %s> <seen: %d>\n", info,   flags[1].meta & CXA_FLAG_SEEN_MASK);
	printf("help: <arg: %s> <seen: %d>\n", "none", flags[2].meta & CXA_FLAG_SEEN_MASK);

	printf("\npositional args:\n");
	for (unsigned long i = 0; i < final->len; i++)
	{
		printf("  pos: %s\n", final->positional[i]);
	}

	cxa_print_usage("tiny desc here", flags);
	cxa_clean(final);
	return 0;
}
