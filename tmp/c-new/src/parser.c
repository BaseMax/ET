/**
 The One Programming Language

 File: parser.c
  _        _
 / \ |\ | |_    Max Base
 \_/ | \| |_    Copyright 2021; One Language Contributors

 **/

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "vm.h"
#include "tree.h"
#include "error.h"
#include "parser.h"

Parser *parserInit(Lexer *lex, ErrorsContainer *errors)
{
	Parser *pars = (Parser*) malloc(sizeof(Parser));
	pars->lex = lex;
	return pars;
}

void parserNextToken(Parser *pars, ErrorsContainer *errors)
{
	pars->lex->tokens++; // go back to the prev token
}

void parserPrevToken(Parser *pars, ErrorsContainer *errors)
{
	pars->lex->tokens--; // go back to the prev token
}

bool tokenIsPrimaryType(Parser *pars, ErrorsContainer *errors)
{
	Token *t = *pars->lex->tokens;

	if (t->type == TOKEN_TYPE_I8)
		return true;
	else if (t->type == TOKEN_TYPE_I16)
		return true;
	else if (t->type == TOKEN_TYPE_I32)
		return true;
	else if (t->type == TOKEN_TYPE_I64)
		return true;
	else if (t->type == TOKEN_TYPE_I128)
		return true;

	else if (t->type == TOKEN_TYPE_U8)
		return true;
	else if (t->type == TOKEN_TYPE_U16)
		return true;
	else if (t->type == TOKEN_TYPE_U32)
		return true;
	else if (t->type == TOKEN_TYPE_U64)
		return true;
	else if (t->type == TOKEN_TYPE_U128)
		return true;

	else if (t->type == TOKEN_TYPE_CHAR)
		return true;
	else if (t->type == TOKEN_TYPE_STRING)
		return true;
	else if (t->type == TOKEN_TYPE_BOOL)
		return true;

	else if (t->type == TOKEN_TYPE_ANY)
		return true;

	return false;
}

bool tokenIsUserType(Parser *pars, ErrorsContainer *errors)
{
	Token *t = *pars->lex->tokens;

	if (t->type == TOKEN_VALUE_IDENTIFIER)
	{
		if (strcmp(t->vstring, "point") == 0)
			return true;

		// TODO: Need to check from a vector-like!
		return false;
	}
	return false;
}

bool parseDatatype(Parser *pars, ErrorsContainer *errors)
{
	if (tokenIsPrimaryType(pars, errors) == true || tokenIsUserType(pars, errors) == true)
	{ // if current token is a primitive data-type
		// TODO: check data-type array. e;g `i32 []`

		parserNextToken(pars, errors); // go to next token

		printf("---------- parseDatatype\n");
		return true; // yes it's a data-type token series!
	}
	return false; // is not!
}

AstStatements *parseArguments(Parser *pars, ErrorsContainer *errors)
{
	AstType *type;
	AstArgument *arg;
	Array *args = (Array*) malloc(sizeof(Array));

	arrayInit(args);

	if ((*pars->lex->tokens)->type == TOKEN_BRACKET_OPEN)
	{
		parserNextToken(pars, errors); // go to next token
		while ((*pars->lex->tokens)->type != TOKEN_BRACKET_CLOSE)
		{ // loop iterate before see a `)` token.
			if (parseDatatype(pars, errors) == true)
			{
				char *name = (*pars->lex->tokens)->vstring; // get value of current IDENTIFIER token.
				parserExceptToken(pars, TOKEN_VALUE_IDENTIFIER, errors);
				parserNextToken(pars, errors); // go to next token

				bool hasArray = false;
				type = astType(AST_TYPE_I8, hasArray);

				arg = astArgument(name, type);
				arrayPush(args, arg);
			}
			else
			{
				// TODO: ErrorAppend(...)
				return NULL;
			}
		}
		parserExceptTokenGo(pars, TOKEN_BRACKET_CLOSE, errors);
	}
	return args; // it's optional, so we return a empty array list!
}

bool parserHasToken(Parser *pars, TokenType want, ErrorsContainer *errors)
{
	Token *t = *pars->lex->tokens;
	if (t->type == want)
		return true;
	return false;
}

bool parserExceptToken(Parser *pars, TokenType want, ErrorsContainer *errors)
{
	Token *t = *pars->lex->tokens;
	if (t->type != want)
	{
		// TODO: ErrorAppend(...)
		printf("Error: we except %s, but we see %s\n", tokenName(want), tokenName(t->type));
		return false; // not valid
	}
	return true; // valid
}

bool parserExceptTokenGo(Parser *pars, TokenType want, ErrorsContainer *errors)
{
	int res = parserExceptToken(pars, want, errors); // call except function
	if (res == 1)
		pars->lex->tokens++;
	// Why we not have and need a `else`, because we already handled errors at parent function.
	return res;
}

AstExpression *parseExpressionPrimitive(Parser *pars, ErrorsContainer *errors)
{
	AstExpression *expr;
	printf("---------- parseExpressionPrimitive\n");
	switch ((*pars->lex->tokens)->type)
	{
		case TOKEN_VALUE_IDENTIFIER:
		case TOKEN_VALUE_STRING:
		case TOKEN_VALUE_NUMBER:
		case TOKEN_VALUE_BOOL:
		{
			printf("[TEST direct]==>%s\n", tokenName((*pars->lex->tokens)->type));
			Token *t = (*pars->lex->tokens);
			printf("---> value of current direct value is %d\n", t->vint);
			printf("---> value of current direct value is %s\n", t->vstring);
			parserNextToken(pars, errors);
			expr = astExpression(TOKEN_OPERATOR_DIRECT, 0, 0, t->vstring, false, NULL, NULL);
			return expr;
			break;
		}
		default:
			// TODO: ErrorAppend(...)
			printf("Error: bad value as expression!\n");
			parserNextToken(pars, errors);
			exit(1);
			break;
	}

	//	if (parserHasToken(pars, TOKEN_VALUE_IDENTIFIER, errors) == true)
	//		parserNextToken(pars, errors);
	//	else if (parserHasToken(pars, TOKEN_VALUE_STRING, errors) == true)
	//		parserNextToken(pars, errors);
	//	else if (parserHasToken(pars, TOKEN_VALUE_NUMBER, errors) == true)
	//		parserNextToken(pars, errors);
	//	else if (parserHasToken(pars, TOKEN_VALUE_BOOL, errors) == true)
	//		parserNextToken(pars, errors);
	//	else
	//	{
	//		// TODO: ErrorAppend(...)
	//		printf("Error: bad value as expression!\n");
	//		parserNextToken(pars, errors);
	//		exit(1);
	//	}
}

AstExpression *parseExpression_1(Parser *pars, ErrorsContainer *errors)
{
	// TODO: Adding ++
	// TODO: Adding --
	// TODO: Adding []
	// TODO: Adding ()
	// TODO: Adding .
	// TODO: Adding ->

	AstExpression *expr;

	if (parserHasToken(pars, TOKEN_BRACKET_OPEN, errors) == true) // current token is (
	{
		parserNextToken(pars, errors); // SKIP ( TOKEN
		expr = parseExpression(pars, errors);
		parserExceptTokenGo(pars, TOKEN_BRACKET_CLOSE, errors); // we except ) and skip it
	}
	else
	{
		expr = parseExpressionPrimitive(pars, errors);
	}
	return expr;
}

AstExpression *parseExpression_2(Parser *pars, ErrorsContainer *errors)
{
	// TODO: type casting...

	AstExpression *expr;
	if (parserHasToken(pars, TOKEN_OPERATOR_PLUS, errors) == true) // current token is +
	{
		parserNextToken(pars, errors); // SKIP + TOKEN
		expr = parseExpression_1(pars, errors); // parser expr-value after + operator
		expr = astExpression(TOKEN_OPERATOR_PLUS, 10, 0, "", false, expr, NULL);
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_MINUS, errors) == true) // current token is -
	{
		parserNextToken(pars, errors); // SKIP - TOKEN
		expr = parseExpression_1(pars, errors); // parser expr-value after - operator
		expr = astExpression(TOKEN_OPERATOR_MINUS, 10, 0, "", false, expr, NULL);
	}
	else
	{
		expr = parseExpression_1(pars, errors); // parser expr-value
	}
	return expr;
}

AstExpression *parseExpression_3(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_3\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_2(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_MUL, errors) == true) // current token is *
	{
		parserNextToken(pars, errors); // SKIP * TOKEN
		expr = astExpression(TOKEN_OPERATOR_MUL, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_DIV, errors) == true) // current token is /
	{
		parserNextToken(pars, errors); // SKIP / TOKEN
		expr = astExpression(TOKEN_OPERATOR_DIV, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_DIVINT, errors) == true) // current token is //
	{
		parserNextToken(pars, errors); // SKIP // TOKEN
		expr = astExpression(TOKEN_OPERATOR_DIVINT, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_REM, errors) == true) // current token is %
	{
		parserNextToken(pars, errors); // SKIP % TOKEN
		expr = astExpression(TOKEN_OPERATOR_REM, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_4(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_4\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_3(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_PLUS, errors) == true) // current token is +
	{
		parserNextToken(pars, errors); // SKIP + TOKEN
		expr = astExpression(TOKEN_OPERATOR_PLUS, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_MINUS, errors) == true) // current token is -
	{
		parserNextToken(pars, errors); // SKIP - TOKEN
		expr = astExpression(TOKEN_OPERATOR_MINUS, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_5(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_5\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_4(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_SHIFT_LEFT, errors) == true) // current token is >>
	{
		parserNextToken(pars, errors); // SKIP >> TOKEN
		expr = astExpression(TOKEN_OPERATOR_SHIFT_LEFT, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_SHIFT_RIGHT, errors) == true) // current token is <<
	{
		parserNextToken(pars, errors); // SKIP << TOKEN
		expr = astExpression(TOKEN_OPERATOR_SHIFT_RIGHT, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_6(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_6\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_5(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_GE, errors) == true) // current token is >=
	{
		parserNextToken(pars, errors); // SKIP >= TOKEN
		expr = astExpression(TOKEN_OPERATOR_GE, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_G, errors) == true) // current token is >
	{
		parserNextToken(pars, errors); // SKIP > TOKEN
		expr = astExpression(TOKEN_OPERATOR_G, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_L, errors) == true) // current token is <
	{
		parserNextToken(pars, errors); // SKIP < TOKEN
		expr = astExpression(TOKEN_OPERATOR_L, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_LE, errors) == true) // current token is <=
	{
		parserNextToken(pars, errors); // SKIP <= TOKEN
		expr = astExpression(TOKEN_OPERATOR_LE, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_7(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_7\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_6(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_EQUALEQUAL, errors) == true) // current token is ==
	{
		parserNextToken(pars, errors); // SKIP == TOKEN
		expr = astExpression(TOKEN_OPERATOR_EQUALEQUAL, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_EQUALEQUALEQUAL, errors) == true) // current token is ===
	{
		parserNextToken(pars, errors); // SKIP === TOKEN
		expr = astExpression(TOKEN_OPERATOR_EQUALEQUALEQUAL, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_NOTEQUALEQUAL, errors) == true) // current token is !==
	{
		parserNextToken(pars, errors); // SKIP !== TOKEN
		expr = astExpression(TOKEN_OPERATOR_NOTEQUALEQUAL, 10, 0, "", false, expr, parseExpression(pars, errors));
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_NOTEQUAL, errors) == true) // current token is !=
	{
		parserNextToken(pars, errors); // SKIP != TOKEN
		expr = astExpression(TOKEN_OPERATOR_NOTEQUAL, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_8(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_8\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_7(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_BIT_AND, errors) == true) // current token is &
	{
		parserNextToken(pars, errors); // SKIP & TOKEN
		expr = astExpression(TOKEN_OPERATOR_BIT_AND, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_9(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_9\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_8(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_BIT_XOR, errors) == true) // current token is ^
	{
		parserNextToken(pars, errors); // SKIP ^ TOKEN
		expr = astExpression(TOKEN_OPERATOR_BIT_XOR, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_10(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_10\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_9(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_BIT_OR, errors) == true) // current token is |
	{
		parserNextToken(pars, errors); // SKIP | TOKEN
		expr = astExpression(TOKEN_OPERATOR_BIT_OR, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_11(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_11\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_10(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_AND, errors) == true) // current token is &&
	{
		parserNextToken(pars, errors); // SKIP && TOKEN
		expr = astExpression(TOKEN_OPERATOR_AND, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_12(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_12\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_11(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_OR, errors) == true) // current token is ||
	{
		parserNextToken(pars, errors); // SKIP || TOKEN
		expr = astExpression(TOKEN_OPERATOR_OR, 10, 0, "", false, expr, parseExpression(pars, errors));
	}

	return expr;
}

AstExpression *parseExpression_13(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_13\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_12(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_QUESTION, errors) == true) // current token is ?
	{
		parserNextToken(pars, errors); // SKIP ? TOKEN
		AstExpression *expr_true = parseExpression(pars, errors);
		parserExceptTokenGo(pars, TOKEN_OPERATOR_COLON, errors); // we except : token and skip it...
		AstExpression *expr_false = parseExpression(pars, errors);
		expr = astExpression3(TOKEN_OPERATOR_IF, 10, 0, "", false, expr, expr_true, expr_false);
	}
	else if (parserHasToken(pars, TOKEN_OPERATOR_QUESTIONQUESTION, errors) == true) // current token is ??
	{
		parserNextToken(pars, errors); // SKIP ?? TOKEN
		AstExpression *expr_false = parseExpression(pars, errors);
		expr = astExpression(TOKEN_OPERATOR_IFIF, 10, 0, "", false, expr, expr_false);
	}

	return expr;
}

AstExpression *parseExpression_14(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression_14\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	AstExpression *expr = parseExpression_13(pars, errors);

	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));
	if (parserHasToken(pars, TOKEN_OPERATOR_EQUAL, errors) == true) // current token is =
	{
		parserNextToken(pars, errors); // SKIP = TOKEN
		AstExpression *right = parseExpression(pars, errors);
		expr = astExpression(TOKEN_OPERATOR_EQUAL, 10, 0, "", false, expr, right);
	}

	return expr;
}

AstExpression *parseExpression(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseExpression\n");
	printf("[TEST]==>%s\n", tokenName((*pars->lex->tokens)->type));

	return parseExpression_14(pars, errors);
}

AstExpressions *parseExpressions(Parser *pars, ErrorsContainer *errors)
{
	AstExpression *expr;
	Array *exprs = (Array*) malloc(sizeof(Array));
	arrayInit(exprs);

	printf("---------- parseExpressions\n");

	expr = parseExpression(pars, errors);
	arrayPush(exprs, expr);

	while (parserHasToken(pars, TOKEN_OPERATOR_VIRGOOL, errors) == true)
	{
		parserExceptTokenGo(pars, TOKEN_OPERATOR_VIRGOOL, errors); // skip virgool and go to next token!
		expr = parseExpression(pars, errors);
		arrayPush(exprs, expr);
	}
	return exprs;
}

AstStatement *parseStatementPrint(Parser *pars, ErrorsContainer *errors)
{
	AstStatement *stmt = astStatement(AST_STATEMENT_PRINT);
	printf("---------- parseStatementPrint\n");

	parserExceptTokenGo(pars, TOKEN_PRINT, errors);
	stmt->expressions = parseExpressions(pars, errors);
	return stmt;
}

AstStatement *parseStatementPrintNl(Parser *pars, ErrorsContainer *errors)
{
	AstStatement *stmt = astStatement(AST_STATEMENT_PRINTNL);
	printf("---------- parseStatementPrintNl\n");

	parserExceptTokenGo(pars, TOKEN_PRINTNL, errors);
	stmt->expressions = parseExpressions(pars, errors);
	return stmt;
}

AstStatement *parseStatementPrintErr(Parser *pars, ErrorsContainer *errors)
{
	AstStatement *stmt = astStatement(AST_STATEMENT_PRINTDB);
	printf("---------- parseStatementPrintErr\n");

	parserExceptTokenGo(pars, TOKEN_PRINTDB, errors);
	stmt->expressions = parseExpressions(pars, errors);
	return stmt;
}

AstStatement *parseStatementPrintErrNl(Parser *pars, ErrorsContainer *errors)
{
	AstStatement *stmt = astStatement(AST_STATEMENT_PRINTDBNL);
	printf("---------- parseStatementPrintErrNl\n");

	parserExceptTokenGo(pars, TOKEN_PRINTNLDB, errors);
	stmt->expressions = parseExpressions(pars, errors);
	return stmt;
}

AstStatement *parseStatementReturn(Parser *pars, ErrorsContainer *errors)
{
	AstStatement *stmt = astStatement(AST_STATEMENT_RETURN);
	printf("---------- parseStatementReturn\n");

	parserExceptTokenGo(pars, TOKEN_RETURN, errors);
	stmt->expressions = parseExpressions(pars, errors);
	return stmt;
}

AstStatement *parseStatement(Parser *pars, ErrorsContainer *errors)
{
	if ((*pars->lex->tokens)->type == TOKEN_PRINT)
	{
		return parseStatementPrint(pars, errors);
	}
	else if ((*pars->lex->tokens)->type == TOKEN_PRINTNL)
	{
		return parseStatementPrintNl(pars, errors);
	}
	else if ((*pars->lex->tokens)->type == TOKEN_PRINTDB)
	{
		return parseStatementPrintErr(pars, errors);
	}
	else if ((*pars->lex->tokens)->type == TOKEN_PRINTNLDB)
	{
		return parseStatementPrintErrNl(pars, errors);
	}
	else if ((*pars->lex->tokens)->type == TOKEN_RETURN)
	{
		return parseStatementReturn(pars, errors);
	}
	else
	{
		// TODO: ErrorAppend(...)
		printf("Error: bad stmt!\n");
		printf("==>%s\n", tokenName((*pars->lex->tokens)->type));
		//		exit(1);
		return NULL;
		parserNextToken(pars, errors);
	}
}

AstBlock *parseBlock(Parser *pars, ErrorsContainer *errors)
{
	AstBlock *block;
	Array *stmts = (Array*) malloc(sizeof(Array));
	arrayInit(stmts);

	parserExceptTokenGo(pars, TOKEN_SECTION_OPEN, errors);
	printf("==== start stmt loop\n");

	AstStatement *stmt;
	while ((*pars->lex->tokens)->type != TOKEN_SECTION_CLOSE)
	{
		stmt = parseStatement(pars, errors);
		printf("==== end current stmt\n");
		arrayPush(stmts, stmt);
	}
	printf("==== end stmt loop\n");
	parserExceptTokenGo(pars, TOKEN_SECTION_CLOSE, errors);

	block = astBlock(stmts);
	return block;
}

AstFunction *parseFunction(Parser *pars, ErrorsContainer *errors)
{
	printf("---------- parseFunction\n");

	if (parseDatatype(pars, errors) == true)
	{
		char *name = (*pars->lex->tokens)->vstring; // get value of current IDENTIFIER token
		parserExceptTokenGo(pars, TOKEN_VALUE_IDENTIFIER, errors); // check if current token is a user identifier

		//		printf("==>%s\n", tokenName((*pars->lex->tokens)->type));
		AstArguments *args = parseArguments(pars, errors);
		printf("==== end args\n");
		AstBlock *block = parseBlock(pars, errors);
		printf("==== end block\n");

		AstFunction *func = astFunction(name, args, block);
		return func;
	}
	else
	{
		// TODO: ErrorAppend(...)
		return NULL;
	}
}

void check(Parser *pars)
{
	//	printf("==>%s\n", tokenName((*pars->lex->tokens)->type));
	pars->lex->tokens++;
	//	printf("==>%s\n", tokenName((*pars->lex->tokens)->type));
}

void parsePackage(Parser *pars, char **package, ErrorsContainer *errors)
{
	printf("---------- parsePackage\n");

	parserExceptTokenGo(pars, TOKEN_PACKAGE, errors);

	if (parserHasToken(pars, TOKEN_VALUE_IDENTIFIER, errors) == true)
	{
		*package = (*pars->lex->tokens)->vstring;
		printf("==>%s\n", *package);
		parserNextToken(pars, errors);
	}
	else if (parserHasToken(pars, TOKEN_VALUE_STRING, errors) == true)
	{
		*package = (*pars->lex->tokens)->vstring;
		printf("==>%s\n", *package);
		parserNextToken(pars, errors);
	}
	else
	{
		printf("Error: bad value for package name!\n");
		// TODO: AppendError(...)
		exit(1);
	}
}

int parserCheck(Parser *pars, ErrorsContainer *errors)
{
	bool hasPackageName = false;
	AstFunction *func;
	AstRoot *root = (AstRoot*) malloc(sizeof(AstRoot));
	vmInit(root);
	arrayInit(root->functions);

	printf("=============== Parser ===============\n");

	Token *t;
	while (*pars->lex->tokens != NULL)
	{
		t = *pars->lex->tokens;
		printf("[TOKEN] %s\n", tokenName(t->type));
		if (t->type == TOKEN_EOF)
			break;
		else if (t->type == TOKEN_UNKNOWM)
		{
			printf("Break...............\n");
			break;
		}

		if (parserHasToken(pars, TOKEN_PACKAGE, errors) == true)
		{
			if (hasPackageName == true)
			{
				// TODO: AppendError(...), It's not allowerd to define package name twice at a source file!
				printf("Error: cannot twice define package name in a single file!");
				exit(1);
				// parserNextToken(pars, errors);
			}
			else
			{
				hasPackageName = true;
				parsePackage(pars, &root->package, errors);
				printf("==>%s\n", root->package);
			}
		}
		else if (parseDatatype(pars, errors) == true) // if current token is a primitive data-type
		{
			if ((*pars->lex->tokens)->type == TOKEN_VALUE_IDENTIFIER)
			{ // check if current token is a user identifier
				parserNextToken(pars, errors); // go to next token
				if ((*pars->lex->tokens)->type == TOKEN_BRACKET_OPEN || (*pars->lex->tokens)->type == TOKEN_SECTION_OPEN)
				{
					parserPrevToken(pars, errors); // go back to user-identifier name (function name)
					parserPrevToken(pars, errors); // go back to data-type
					func = parseFunction(pars, errors);
					printf("==== end func\n");
					arrayPush(root->functions, func);
				}
			}
		}
		else
		{
			printf("Error: bad token in the parser stage!\n");
			// TODO: ErrorAPpend(...)
			exit(1);
		}
	}

	treeLog(root);

	vmLog(root);
	vmFree(root);
	return EXIT_SUCCESS;
}

void parserFree(Parser *pars)
{
}
