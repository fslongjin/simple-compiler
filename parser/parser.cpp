//
// Created by longjin on 22-7-29.
//

#include "parser.h"

namespace parser
{
	/**
	 * 获取下一个token
	 * @return 下一个token
	 */
	lex::Token Parser::get_next_token()
	{
		this->current_token = lexer->get_token();
		return this->current_token;
	}

	std::unique_ptr<lex::ExprAST> Parser::log_error(const char* str)
	{
		fprintf(stderr, "[ERROR] %s\n", str);
		return nullptr;
	}
	std::unique_ptr<lex::PrototypeAST> Parser::log_error_prototype(const char* str)
	{
		log_error(str);
		return nullptr;
	}

	/**
	 * 解析数字表达式
	 * @param numVal 数字值
	 * @return AST结点
	 */
	std::unique_ptr<lex::ExprAST> Parser::parse_number_expr()
	{
		auto res = std::make_unique<lex::NumberExprAST>(current_token.get_numVal());
		get_next_token();    // 消费掉当前表达式的token
		return std::move(res);
	}

	/**
	 * 解析括号表达式
	 * @return AST结点
	 */
	std::unique_ptr<lex::ExprAST> Parser::parse_paren_expr()
	{
		// 该方法是递归下降解析器的一种典型方法
		// 括号并不会构造AST结点，因为括号的用途是引导parser理解层次结构，并去构造AST

		// 吃掉左括号'('
		get_next_token();
		// 解析中间的表达式
		auto val = parse_expression();
		// 若没有结果，则返回。
		if (!val)
			return nullptr;
		if (current_token.get_token() != ')')
			return log_error("expected ')'");

		// 吃掉右括号
		get_next_token();
		return val;
	}
	std::unique_ptr<lex::ExprAST> Parser::parse_identifier_expr()
	{
		std::string identifier = this->current_token.get_str();

		// 消费掉当前表达式
		get_next_token();
		// 下一个表达式不是左括号，则当前表达式为变量。
		if (current_token.get_token() != '(')
			return std::make_unique<lex::VariableExprAST>(identifier);

		// 表明这是一个函数调用
		get_next_token();    // 消费掉左括号

		std::vector<std::unique_ptr<lex::ExprAST>> args;

		if (current_token.get_token() != ')')    // 存在参数
		{
			while (true)
			{
				// 解析括号内的参数
				if (auto arg = parse_expression())
					args.emplace_back(std::move(arg));
				else return nullptr;

				// 括号解析完毕
				if (current_token.get_token() == ')')
					break;
				if (current_token.get_token() != ',')
					return log_error("Expected ')' or ',' in argument list.");
				get_next_token();
			}
		}

		// 消费掉当前的')'/
		get_next_token();

		// 构造函数调用的ast结点
		return std::make_unique<lex::CallExprAST>(identifier, std::move(args));
	}

	/**
	 * 解析主表达式
	 * 包括：identifierExpr
	 * numberExpr
	 * parenExpr
	 * @return 对应的表达式的AST结点
	 */
	std::unique_ptr<lex::ExprAST> Parser::parse_primary()
	{
		switch (current_token.get_token())
		{
		default:
			return log_error("Unknown token when expecting an expression");
		case lex::tok_identifier:
			return parse_identifier_expr();
		case lex::tok_number:
			return parse_number_expr();
		case '(':
			return parse_paren_expr();
		}
	}

} // parser