//
// Created by longjin on 22-7-29.
//

#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
namespace lex
{
	// 词法分析器返回的token中，若为关键字，则返回小于0的值，否则返回值大于0
	enum token_enum
	{
		tok_eof = -1,

		// commands
		tok_def = -2,
		tok_extern = -3,

		// primary
		tok_identifier = -4,
		tok_number = -5
	};

	/**
	 * lexer分析的token的结构体
	 */
	class Token
	{
		int token;
		double numVal;
		std::string str;

	 public:
		Token() = default;
		Token(int token, double numVal)
			: token(token), numVal(numVal)
		{
		}

		void set_token(int tok)
		{
			this->token = tok;
		}
		int get_token() const
		{
			return token;
		}

		void set_numVal(double val)
		{
			this->numVal = val;
		}
		double get_numVal() const
		{
			return this->numVal;
		}

		void set_str(const std::string& s);

		std::string get_str() const;

		bool operator==(const Token& p) const;
		bool operator!=(const Token& p) const;
	};

	/**
	 * 词法分析器
	 */
	class Lexer
	{
	 private:
		std::string identifier;
		double numVal = 0;

	 public:
		explicit Lexer() = default;
		// 获取一个token
		Token get_token();
		std::string get_identifier()const;
	};

} // lex

#endif //_LEXER_H_
