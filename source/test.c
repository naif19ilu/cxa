/*
 * Cxa - C executable arguments parser
 * Jul 7, 2025
 * Tester file
 */
#define CXA_HEADER_ONLY
#include "cxa.h"

#include <stdio.h>

int main (int argc, char **argv)
{
	struct CxaFlag flags[] = {
		{ "document",     "specifies the input document",                 NULL,       'd', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "verbose",      "enables verbose output",                       NULL,       'v', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "usage",        "prints usage or flag-specific help",           NULL,       'u', CXA_FLAG_NEEDS_ARG_MAY, CXA_FLAG_NOT_SEEN },
		{ "interactive",  "runs program in interactive mode",             NULL,       'i', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "output",       "defines output file path",                     NULL,       'o', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "force",        "forces overwrite of output file",              NULL,       'f', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "color",        "sets output color mode (auto, always, never)", "auto",     'c', CXA_FLAG_NEEDS_ARG_MAY, CXA_FLAG_NOT_SEEN },
		{ "log",          "writes log to specified file",                 "log.txt",  'l', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "threads",      "sets number of worker threads",                "4",        't', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "dry-run",      "shows what would be done without doing it",    NULL,       'n', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "filter",       "applies a filter expression to input",         NULL,       'x', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "config",       "loads configuration from file",                NULL,       'C', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "quiet",        "suppresses all output",                        NULL,       'q', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "summary",      "shows a summary at the end",                   NULL,       's', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		{ "max-lines",    "limits number of lines processed",             "1000",     'M', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "min-size",     "minimum size filter for files",                "10K",      'm', CXA_FLAG_NEEDS_ARG_MAY, CXA_FLAG_NOT_SEEN },
		{ "retry",        "sets retry count on failure",                  "3",        'r', CXA_FLAG_NEEDS_ARG_YES, CXA_FLAG_NOT_SEEN },
		{ "timeout",      "sets timeout in seconds",                      "30",       'T', CXA_FLAG_NEEDS_ARG_MAY, CXA_FLAG_NOT_SEEN },
		{ "seed",         "random seed for reproducibility",              "random",   'S', CXA_FLAG_NEEDS_ARG_MAY, CXA_FLAG_NOT_SEEN },
		{ "license",      "prints license info and exits",                NULL,       'L', CXA_FLAG_NEEDS_ARG_NON, CXA_FLAG_NOT_SEEN },
		CXA_FINAL_FLAG
	};

	cxa_parse("tester", flags, (unsigned int) argc, argv, 16);
	cxa_document(flags, "powerful CLI argument parser");
	cxa_debug(flags);
	return 0;
}
