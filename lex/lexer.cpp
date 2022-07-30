//
// Created by longjin on 22-7-29.
//

#include "lexer.h"

namespace lex
{

	Token Lexer::get_token()
	{
		int last_char = getchar();
		std::string identifier_str;
		// 跳过空格
		while (std::isspace(last_char))
			last_char = getchar();

		// 第一个字符为字母
		if (std::isalpha(last_char))
		{
			identifier_str = std::to_string(last_char);
			// 读取整个单词
			while (std::isalnum((last_char = getchar())))
				identifier_str += std::to_string(last_char);

			if (identifier_str == "def")
				return Token(tok_def, 0);
			if (identifier_str == "extern")
				return Token(tok_extern, 0);

			return Token(tok_identifier, 0);
		}

		// 第一个为数字
		if (std::isdigit(last_char) || last_char == '.')
		{
			std::string num_str;
			do
			{
				num_str += std::to_string(last_char);
				last_char = getchar();
			} while (isdigit(last_char) || last_char == '.');

			numVal = strtod(num_str.c_str(), nullptr);
			return Token(tok_number, numVal);
		}

		// 忽略注释
		if (last_char == '#')
		{
			do
			{
				last_char = getchar();
			} while (last_char != EOF && last_char != '\n' && last_char != '\r');

			if (last_char != EOF)
				return get_token();
		}

		// 检查是否到达了文件的结尾
		if (last_char == EOF)
			return Token(tok_eof, 0);

		// 其他情况的话，不符合我们的解析条件，直接返回
		return Token(last_char, 0);
	}
	bool Token::operator==(const Token& p) const
	{
		return this->get_numVal() == p.get_numVal() && this->get_token() == p.get_token();
	}
	bool Token::operator!=(const Token& p) const
	{
		return !((*this) == p);
	}
	void Token::set_str(const std::string& s)
	{
		this->str = s;
	}
	std::string Token::get_str() const
	{
		return this->str;
	}
} // lex