/**
 The One Programming Language

 File: error.h
  _        _
 / \ |\ | |_    Max Base
 \_/ | \| |_    Copyright 2021

 **/

#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Position;

typedef enum
{
	ERROR,
	WARNING,
	NOTE,
} ErrorLevel;

typedef struct
{
	char *message;
	Position position;
	ErrorLevel level;
} Error;

typedef struct
{
	int error_count;
	int error_capacity;
	Error *errors;
} ErrorsContainer;

void ErrorsInit(ErrorsContainer *);

void ErrorsPrint(FILE *, ErrorsContainer *);

void ErrorsFree(ErrorsContainer *);

void ErrorsAdd(ErrorsContainer *, char *);

void error(char *);

#endif
