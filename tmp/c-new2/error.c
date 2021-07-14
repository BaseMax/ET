/**
 The One Programming Language

 File: error.c
  _        _
 / \ |\ | |_    Max Base
 \_/ | \| |_    Copyright 2021; One Language Contributors

 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lexer.h"
#include "token.h"
#include "array.h"
#include "ast.h"
#include "parser.h"

#include "error.h"

extern Parser parser;

void error(ErrorType type, const char* format, ...)
{
	debug("error");

	va_list args;
	va_start(args, format);
	// TODO: Show error type with another custom color!
	fprintf(stderr, "%s: ", error_name(type));
	vfprintf(stderr, format, args);
	va_end(args);

	Token* current_token = (*parser.tokens);
	fprintf(stderr, " at %s:%d:%d", parser.path == NULL ? "REPL" : parser.path, current_token->loc.line, current_token->loc.column);
	fputs("\n", stderr);

	if (type != ERROR_WARNING)
	{
		exit(1);
	}
}

char* error_name(ErrorType type)
{
	debug("error_name");

	switch (type)
	{
		case ERROR_TREE:
			return "TREE";
		case ERROR_WARNING:
			return "WARNING";
		case ERROR_PANIC:
			return "ERROR";
		case ERROR_TOKEN:
			return "TOKEN ERROR";
		case ERROR_PARSER:
			return "PARSER ERROR";
		default:
			return "UNKNOWN ERROR";
	}
}
