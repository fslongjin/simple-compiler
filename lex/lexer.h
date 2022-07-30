//
// Created by longjin on 22-7-29.
//

#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
namespace lex
{
	// 词法分析器返回的token中，若为关键字，则返回小于0的值，否则返回值大于0
	enum Token
	{
		tok_eof = -1,

		// commands
		tok_def = -2,
		tok_extern = -3,

		// primary
		tok_identifier = -4,
		tok_number = -5
	};

	class lexer
	{
	 private:
		std::string identifier;
		double numVal;

		int get_token();
	};

} // lex

#endif //_LEXER_H_
