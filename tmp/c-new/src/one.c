/**
 The One Programming Language

 File: one.c
  _        _
 / \ |\ | |_    Max Base
 \_/ | \| |_    Copyright 2021; One Language Contributors

 **/

#include "token.h"
#include "arg.h"
#include "compile.h"
#include "error.h"

int main(int argc, char **argv, char **env)
{
	int ret = EXIT_SUCCESS;

	ErrorsContainer errors;
	ErrorsInit(&errors);

	Args args;
	ArgsInit(&args);
	ArgsParse(argc, argv, env, &args, &errors);

	if (args.help)
	{
		ArgsHelp(stderr, &args);
	}
	else if (args.version)
	{
		ArgsVersion(stderr, &args);
	}
	else if (args.input_files_count > 0)
	{
		ret = compile(&args, &errors);
	}
	else
	{
		ArgsHelp(stderr, &args);
	}

	ErrorsPrints(stderr, &errors);

	ArgsFree(&args);
	ErrorsFree(&errors);

	return ret;
}
