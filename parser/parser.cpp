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
		std::string identifier = this->lexer->get_identifier();

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
	/**
	 * 解析函数原型
	 * @return 函数原型的AST
	 */
	std::unique_ptr<lex::PrototypeAST> Parser::parse_prototype()
	{
		if (current_token.get_token() != lex::tok_identifier)
			return log_error_prototype("Expected function name in prototype.");
		// 获取函数名
		std::string func_name = this->lexer->get_identifier();
		get_next_token();

		if (current_token.get_token() != '(')
			return log_error_prototype("Expected '(' in prototype.");

		// 读取参数名
		std::vector<std::string> arg_names;
		while (get_next_token().get_token() == lex::tok_identifier)
			arg_names.emplace_back(this->lexer->get_identifier());
		if (current_token.get_token() != ')')
			return log_error_prototype("Expected ')' in prototype.");

		// 成功解析函数原型
		get_next_token();    // 消费掉')'
		return std::make_unique<lex::PrototypeAST>(func_name, std::move(arg_names));
	}

	/**
	 * 解析函数定义
	 * 函数定义其实就是一个原型+表达式的形式
	 * @return 函数定义的AST
	 */
	std::unique_ptr<lex::FunctionAST> Parser::parse_definition()
	{
		get_next_token();    // 消费掉 'def' 关键字
		auto proto = parse_prototype();
		if (!proto) return nullptr;

		// 构建函数定义的AST（原型+表达式）拼接而成
		if (auto expr = parse_expression())
			return std::make_unique<lex::FunctionAST>(std::move(proto), std::move(expr));
		return nullptr;
	}

	/**
	 * 解析函数的extern声明
	 * @return extern声明的函数的AST
	 */
	std::unique_ptr<lex::PrototypeAST> Parser::parse_extern()
	{
		get_next_token();    // 消费掉‘extern’关键字
		return parse_prototype();
	}

	/**
	 * 解析用户定义的任何顶级表达式
	 * @return 顶级表达式的匿名函数AST
	 */
	std::unique_ptr<lex::FunctionAST> Parser::parse_top_level_expr()
	{
		// 解析顶级表达式
		if (auto expr = parse_expression())
		{
			// 为顶级表达式构建一个匿名的函数原型
			auto prototype = std::make_unique<lex::PrototypeAST>("", std::vector<std::string>());
			// 构建AST
			return std::make_unique<lex::FunctionAST>(std::move(prototype), std::move(expr));
		}
		return nullptr;
	}

} // parser