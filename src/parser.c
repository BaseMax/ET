//
// Created by max on 6/9/21.
//

#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "lexer.h"
#include "token.h"
#include "array.h"
#include "one.h"
#include "ast.h"
#include "tree.h"

#include "parser.h"

// Global variable(s)
Parser parser;
Array tokens;

// macro
#define PARSER_HAS(want_type) ((*parser.tokens)->type == want_type)

#define PARSER_CURRENT \
	(*parser.tokens)

#define PARSER_CURRENT_LOG                              \
	debug_parser(                                       \
		 "Current Token is %s (%d)%s%s",                \
		 token_name((*parser.tokens)->type),            \
		 (*parser.tokens)->type,                        \
		 ((*parser.tokens)->value != NULL) ? ": " : "", \
		 ((*parser.tokens)->value != NULL) ? (*parser.tokens)->value : "");

void parser_init()
{
	debug_parser("parser_init");

	parser.package = NULL;
	parser.tokens = NULL;
	parser.tokens_count = 0;
	array_init(&tokens);
}

void parser_push(Token* t)
{
	array_push(&tokens, t);
}

void parser_scan()
{
	debug_parser("parser_scan");

	Token* t;
	for (;;)
	{
		t = lexer_scan();
		parser_push(t);
		debug_parser("parser_scan: print_token %s", token_name(t->type));
		if (t->type == TOKEN_ERROR)
		{
			printf("Error: %s\n", t->value);
			break;
		}
		else if (t->type == TOKEN_EOF)
			break;
	}
}

void parser_start()
{
	debug_parser("parser_start");

	parser.tokens = (Token**)tokens.data;
	parser.tokens_count = tokens.count;

	parser_tokens_log();

	AstRoot* root = parser_check();

	tree_show(stdout, root);

	ast_free(&root);
}

void parser_tokens_log()
{
	debug_parser("parser_tokens_log");

	for (size_t i = 0; i < parser.tokens_count; i++)
	{
		info_parser("Token[%zu] is %s (%s)", i, token_name(parser.tokens[i]->type), parser.tokens[i]->value == NULL ? "Null" : parser.tokens[i]->value);
	}
}

AstRoot* parser_check()
{
	debug_parser("parser_check");

	Token* t;
	AstRoot* root = malloc(sizeof(AstRoot));
	AstGlobalStatenent* stmt;
	AstFunctions fns;

	//	Array vars;
	//	Array structs;
	//	Array enums;
	//	Array types;

	array_init(&fns);
	//	array_init(&vars);
	//	array_init(&structs);
	//	array_init(&enums);
	//	array_init(&types);

	while ((*parser.tokens)->type != TOKEN_EOF)
	{
		t = *parser.tokens;
		debug_parser("parser_check: print_token %d <-> %s", t->type, token_name(t->type));
		if (t->type == TOKEN_ERROR)
		{
			debug_parser("parser_check: print_token_error %s", t->value);
			error_token(t->value);
			break;
		}

		stmt = parser_parse();
		if (stmt != NULL)
		{
			if (stmt->type == AST_GLOBAL_STATEMENT_FN)
			{
				array_push(&fns, (void*)stmt->fn);
			}
			//			else if (stmt->type == AST_GLOBAL_STATEMENT_VAR)
			//			{
			//				array_push(&vars, stmt->var);
			//			}
			//			else if (stmt->type == AST_GLOBAL_STATEMENT_TYPE)
			//			{
			//				array_push(&vars, stmt->type);
			//			}
			//			else if (stmt->type == AST_GLOBAL_STATEMENT_STRUCT)
			//			{
			//				array_push(&structs, stmt->struct);
			//			}
			//			else if (stmt->type == AST_GLOBAL_STATEMENT_ENUM)
			//			{
			//				array_push(&enums, stmt->enum);
			//			}
		}

		// parser_next();
		// parser.tokens++;
	}

	root->package = (char*)parser.package;
	root->functions = fns;

	//	printf("==>%d\n", fns.count);
	//	printf("==>%d\n", root->functions.count);

	//	root->vars = &vars;
	//	root->types = &types;
	//	root->strucs = &structs;
	//	root->enums = &enums;

	//	array_free(&fns);
	//	array_free(&vars);
	//	array_free(&types);
	//	array_free(&structs);
	//	array_free(&enums);

	return root;
}

void parser_next()
{
	debug_parser("parser_next");

	parser.tokens_index++;
	parser.tokens++;
}

void parser_prev()
{
	debug_parser("parser_prev");

	parser.tokens_index--;
	parser.tokens--;
}

AstGlobalStatenent* parser_parse()
{
	debug_parser("parser_parse");

	AstGlobalStatenent* stmt = malloc(sizeof(AstGlobalStatenent));
	AstFunction* fn;

	size_t i = parser.tokens_index;

	if (parser.tokens_index == 0 && PARSER_CURRENT->type != TOKEN_PACKAGE && parser.package == NULL)
	{
		info_parser("User not defined a package name, so we set it to '%s' as default.", ONE_PACKAGE_DEFAULT);
		parser.package = ONE_PACKAGE_DEFAULT;
	}

	if (PARSER_CURRENT->type == TOKEN_PACKAGE)
	{
		parser_parse_package();
		return NULL;
	}
	else if (PARSER_CURRENT->type == TOKEN_FN)
	{
		fn = parser_parse_fn();
		stmt->type = AST_GLOBAL_STATEMENT_FN;
		stmt->fn = fn;
		return stmt;
	}

	if (PARSER_CURRENT->type == TOKEN_EOF)
		return NULL;
	else if (parser.tokens_index == i)
	{
		printf("==>%s\n", PARSER_CURRENT->value);
		error_parser("Cannot parse this kind of statement, we face to %s token!", token_name(PARSER_CURRENT->type));
	}

	return stmt;
}

AstFunction* parser_parse_fn()
{
	debug_parser("parser_parse_fn");

	AstFunction* fn; // = malloc(sizeof(AstFunction));
	AstBlock* block;
	Token* ident;

	AstArguments args;
	array_init(&args);

	parser_expect(TOKEN_FN);
	ident = PARSER_CURRENT;
	parser_expect(TOKEN_VALUE_IDENTIFIER);

	info_parser("Define function = %s", ident->value);
	block = parser_parse_block();

	fn = ast_make_function((char*)ident->value, args, block);

	//	fn->arguments = args;
	// 	array_free(&args);
	//	fn->name = (char*)ident->value;
	//	fn->block = block;

	return fn;
}

AstBlock* parser_parse_block()
{
	debug_parser("parser_parse_block");

	parser_expect(TOKEN_OPERATOR_BRACKET_CURLY_LEFT);
	AstBlock* block = parser_parse_statements();
	parser_expect(TOKEN_OPERATOR_BRACKET_CURLY_RIGHT);
	return block;
}

AstBlock* parser_parse_statements()
{
	debug_parser("parser_parse_statements");

	AstStatement* stmt;
	AstStatements stmts;
	array_init(&stmts);

	while (PARSER_CURRENT->type != TOKEN_OPERATOR_BRACKET_CURLY_RIGHT)
	{
		stmt = parser_parse_statement();
		//		printf("STMT TYPE ===>%s\n", ast_statement_name(stmt->type));
		array_push(&stmts, (void*)stmt);
	}

	AstBlock* block = ast_make_block(stmts);
	//	array_free(&stmts); // TODO

	return block;
}

AstStatement* parser_parse_statement()
{
	debug_parser("parser_parse_statement");

	AstStatement* stmt;

	info_parser("statement type = %s", token_name(PARSER_CURRENT->type));

	if (PARSER_CURRENT->type == TOKEN_PRINT || PARSER_CURRENT->type == TOKEN_PRINTNL || PARSER_CURRENT->type == TOKEN_PRINTDB || PARSER_CURRENT->type == TOKEN_PRINTDBNL)
	{
		stmt = parser_parse_statement_prints();
		return stmt;
	}
	else if (PARSER_CURRENT->type == TOKEN_IF)
	{
	}
	else if (PARSER_CURRENT->type == TOKEN_WHILE)
	{
	}
	else if (PARSER_CURRENT->type == TOKEN_DO)
	{
	}
	else if (PARSER_CURRENT->type == TOKEN_MATCH)
	{
	}
	else if (PARSER_CURRENT->type == TOKEN_SWITCH)
	{
	}
	else
	{
		error_parser("We face to %s as a unknown type of token at print statement!", token_name(PARSER_CURRENT->type));
		return NULL;
	}
	return NULL;
}

AstStatement* parser_parse_statement_prints()
{
	debug_parser("parser_parse_statement_prints");

	if (PARSER_CURRENT->type == TOKEN_PRINT)
	{
		return parser_parse_statement_print();
	}
	else if (PARSER_CURRENT->type == TOKEN_PRINTNL)
	{
		return parser_parse_statement_printnl();
	}
	else if (PARSER_CURRENT->type == TOKEN_PRINTDB)
	{
		return parser_parse_statement_printdb();
	}
	else if (PARSER_CURRENT->type == TOKEN_PRINTDBNL)
	{
		return parser_parse_statement_printdbnl();
	}
	else
	{
		error_parser("We expect a print statement but we face to a %s token!", token_name(PARSER_CURRENT->type));
		return NULL;
	}
}

AstStatement* parser_parse_statement_print()
{
	debug_parser("parser_parse_statement_print");

	AstStatement* stmt = ast_make_statement(AST_STATEMENT_PRINT);

	parser_expect(TOKEN_PRINT);

	//	printf("====::::: current token is %s\n", token_name(PARSER_CURRENT->type));

	stmt->expressions = parser_parse_expressions();

	return stmt;
}

AstStatement* parser_parse_statement_printnl()
{
	debug_parser("parser_parse_statement_printnl");

	AstStatement* stmt = ast_make_statement(AST_STATEMENT_PRINTNL);

	parser_expect(TOKEN_PRINTNL);
	stmt->expressions = parser_parse_expressions();

	return stmt;
}

AstStatement* parser_parse_statement_printdb()
{
	debug_parser("parser_parse_statement_printdb");

	AstStatement* stmt = ast_make_statement(AST_STATEMENT_PRINTDB);

	parser_expect(TOKEN_PRINTDB);
	stmt->expressions = parser_parse_expressions();

	return stmt;
}

AstStatement* parser_parse_statement_printdbnl()
{
	debug_parser("parser_parse_statement_printdbnl");

	AstStatement* stmt = ast_make_statement(AST_STATEMENT_PRINTDBNL);

	parser_expect(TOKEN_PRINTDBNL);
	stmt->expressions = parser_parse_expressions();

	return stmt;
}

AstExpressions parser_parse_expressions()
{
	debug_parser("parser_parse_expressions");

	AstExpression* expr;
	AstExpressions exprs;
	array_init(&exprs);

	bool is_first = true;

	do {
		if (is_first == false) parser_expect(TOKEN_OPERATOR_COMMA);
		//		printf("1/at exprs parser ====::::: current token is %s - %s\n", token_name(PARSER_CURRENT->type), PARSER_CURRENT->value);
		expr = parser_parse_expression();
		//		printf("2/at exprs parser ====::::: current token is %s\n", token_name(PARSER_CURRENT->type));
		array_push(&exprs, expr);
		if (is_first == true) is_first = false;
	} while (PARSER_CURRENT->type == TOKEN_OPERATOR_COMMA);

	//	printf("SOOOO at bottom of exprs parser ====::::: current token is %s\n", token_name(PARSER_CURRENT->type));

	//	for (;;)
	//	{
	//		expr = parser_parse_expression();
	//		array_push(&exprs, (void*)expr);
	//
	//		printf("::::: current token is %s\n", token_name(PARSER_CURRENT->type));
	//
	//		if (parser_has(TOKEN_OPERATOR_COMMA))
	//		{
	//			parser_expect(TOKEN_OPERATOR_COMMA);
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}

	return exprs;
}

AstExpression* parser_parse_expression_primary()
{
	AstExpression* expr = malloc(sizeof(AstExpression));

	//	printf("1/at expr parser====::::: current token is %s - %s\n", token_name(PARSER_CURRENT->type), PARSER_CURRENT->value);

	if (PARSER_CURRENT->type == TOKEN_VALUE_STRING)
	{
		expr->operator= AST_TYPE_STRING;
		expr->vstring = (char*)PARSER_CURRENT->value;
		info_parser("parser_parse_expression: string value is '%s'", expr->vstring);

		parser_next();
	}
	else if (PARSER_CURRENT->type == TOKEN_VALUE_NUMBER)
	{
		expr->operator= AST_TYPE_I32;
		expr->vstring = (char*)PARSER_CURRENT->value;
		info_parser("parser_parse_expression: number value is '%s'", expr->vstring);

		parser_next();
	}
	else
	{
		error_parser("Bad token as expression, we face to a %s token!", token_name(PARSER_CURRENT->type));
	}
	return expr;
}

ParseRule rules[] = {
/* Compiling Expressions rules < Calls and Functions infix-left-paren
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
*/
//> Calls and Functions infix-left-paren
	 [TOKEN_OPERATOR_BRACKET_ROUND_LEFT]    = {grouping, call,   PREC_CALL},
//< Calls and Functions infix-left-paren
	 [TOKEN_OPERATOR_BRACKET_ROUND_RIGHT]   = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_OPERATOR_BRACKET_SQUARE_LEFT]    = {NULL,     NULL,   PREC_NONE}, // [big]
	 [TOKEN_OPERATOR_BRACKET_SQUARE_RIGHT]   = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_OPERATOR_COMMA]         = {NULL,     NULL,   PREC_NONE},
/* Compiling Expressions rules < Classes and Instances table-dot
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
*/
//> Classes and Instances table-dot
	 [TOKEN_OPERATOR_DOT]           = {NULL,     dot,    PREC_CALL},
//< Classes and Instances table-dot
	 [TOKEN_OPERATOR_MINUS]         = {unary,    binary, PREC_TERM},
	 [TOKEN_OPERATOR_PLUS]          = {NULL,     binary, PREC_TERM},
	 [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_OPERATOR_SLASH]         = {NULL,     binary, PREC_FACTOR},
	 [TOKEN_OPERATOR_STAR]          = {NULL,     binary, PREC_FACTOR},
/* Compiling Expressions rules < Types of Values table-not
  [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-not
	 [TOKEN_OPERATOR_B]          = {unary,    NULL,   PREC_NONE},
//< Types of Values table-not
/* Compiling Expressions rules < Types of Values table-equal
  [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-equal
	 [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
//< Types of Values table-equal
	 [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
/* Compiling Expressions rules < Types of Values table-comparisons
  [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-comparisons
	 [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY},
	 [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
	 [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
	 [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
	 [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
//< Types of Values table-comparisons
/* Compiling Expressions rules < Global Variables table-identifier
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
*/
//> Global Variables table-identifier
	 [TOKEN_IDENTIFIER]    = {variable, NULL,   PREC_NONE},
//< Global Variables table-identifier
/* Compiling Expressions rules < Strings table-string
  [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
*/
//> Strings table-string
	 [TOKEN_STRING]        = {string,   NULL,   PREC_NONE},
//< Strings table-string
	 [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
/* Compiling Expressions rules < Jumping Back and Forth table-and
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
*/
//> Jumping Back and Forth table-and
	 [TOKEN_AND]           = {NULL,     and_,   PREC_AND},
//< Jumping Back and Forth table-and
	 [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
/* Compiling Expressions rules < Types of Values table-false
  [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-false
	 [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
//< Types of Values table-false
	 [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
/* Compiling Expressions rules < Types of Values table-nil
  [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-nil
	 [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
//< Types of Values table-nil
/* Compiling Expressions rules < Jumping Back and Forth table-or
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
*/
//> Jumping Back and Forth table-or
	 [TOKEN_OR]            = {NULL,     or_,    PREC_OR},
//< Jumping Back and Forth table-or
	 [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
/* Compiling Expressions rules < Superclasses table-super
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
*/
//> Superclasses table-super
	 [TOKEN_SUPER]         = {super_,   NULL,   PREC_NONE},
//< Superclasses table-super
/* Compiling Expressions rules < Methods and Initializers table-this
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
*/
//> Methods and Initializers table-this
	 [TOKEN_THIS]          = {this_,    NULL,   PREC_NONE},
//< Methods and Initializers table-this
/* Compiling Expressions rules < Types of Values table-true
  [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
*/
//> Types of Values table-true
	 [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
//< Types of Values table-true
	 [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
	 [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule* getRule(TokenType type) {
	return &rules[type];
}

AstExpression* parser_parse_expression_precedence(Precedence precedence)
{
	advance();
	ParseFn prefixRule = getRule(parser.previous.type)->prefix;
	if (prefixRule == NULL)
	{
		error("Expect expression.");
		return;
	}

	bool canAssign = precedence <= PREC_ASSIGNMENT;
	prefixRule(canAssign);

	while (precedence <= getRule(parser.current.type)->precedence)
	{
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;

		infixRule(canAssign);
	}

	if (canAssign && match(TOKEN_EQUAL))
	{
		error("Invalid assignment target.");
	}
}

AstExpression* parser_parse_expression()
{
	debug_parser("parser_parse_expression");
	return parser_parse_expression_precedence();
	//	return parser_parse_expression_primary();
}

void parser_parse_package()
{
	debug_parser("parser_parse_package");

	Token* t;
	if (parser.tokens_index == 0)
	{
		parser_expect(TOKEN_PACKAGE);
		if (PARSER_HAS(TOKEN_VALUE_IDENTIFIER))
		{
			PARSER_CURRENT_LOG
			t = PARSER_CURRENT;
			parser_expect(TOKEN_VALUE_IDENTIFIER);
		}
		else if (PARSER_HAS(TOKEN_VALUE_STRING))
		{
			PARSER_CURRENT_LOG
			t = PARSER_CURRENT;
			parser_expect(TOKEN_VALUE_STRING);
		}
		else
		{
			error_parser("You cannot set package name as %s token, you have to write this in two general forms:\n\t1- Identifier: `package yourname`\n\t2- String: `package \"your-name\"`", token_name(PARSER_CURRENT->type));
			return;
		}
	}
	else
	{
		error_parser("You are unable to define package name at middle or end of the file and you have to set package name at top!");
		return;
	}

	info_parser("SET PACKAGE = \"%s\"", t->value);
	parser.package = (char*)t->value;
}

bool parser_expect(TokenType expected)
{
	debug_parser("parser_expect");

	if (PARSER_CURRENT->type == expected)
	{
		parser_next();
		return true;
	}
	else
	{
		error_parser("We expected %s but we find %s", token_name(expected), token_name(PARSER_CURRENT->type));
	}
	return false;
}

bool parser_has(TokenType expected)
{
	debug_parser("parser_has");

	if (PARSER_CURRENT->type == expected)
	{
		return true;
	}
	return false;
}

void parser_free()
{
	debug_parser("parser_free");

	// TODO
}
