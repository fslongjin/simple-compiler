//
// Created by longjin on 22-7-29.
//

#ifndef _PARSER_H_
#define _PARSER_H_

#include <lex/lexer.h>
#include "ast.h"
#include <memory>
#include <utility>
#include <map>

namespace parser
{
	// 存放有每个二元操作符的优先级的map
	extern std::map<char, int> binOpPrecedence;

	class Parser
	{
	 private:
		// 对应的词法解析器的指针
		std::shared_ptr<lex::Lexer> lexer;
		lex::Token current_token;

		/**
		 * 获取下一个token
		 * @return 下一个token
		 */
		lex::Token get_next_token();

		/**
		 * 错误日志输出函数
		 * @param str 错误日志信息
		 * @return nullptr
		 */
		std::unique_ptr<lex::ExprAST> log_error(const char* str);
		std::unique_ptr<lex::PrototypeAST> log_error_prototype(const char* str);

		/**
		 * 解析数字表达式
		 * @param numVal 数字值
		 * @return AST结点
		 */
		std::unique_ptr<lex::ExprAST> parse_number_expr();

		/**
		 * 解析括号表达式
		 * @return AST结点
		 */
		std::unique_ptr<lex::ExprAST> parse_paren_expr();

		/**
		 * 解析标志符表达式
		 * @return
		 */
		std::unique_ptr<lex::ExprAST> parse_identifier_expr();

		/**
		 * 解析主表达式
		 * 包括：identifierExpr
		 * numberExpr
		 * parenExpr
		 * @return 对应的表达式的AST结点
		 */
		std::unique_ptr<lex::ExprAST> parse_primary();

		/**
		 * 获取当前token的二元运算符优先级
		 * @return 优先级。若不存在，则返回-1
		 */
		int get_token_precedence();

		/**
		 * 解析表达式
		 * @return primary binary operator rhs
		 */
		std::unique_ptr<lex::ExprAST> parse_expression();

		/**
		 * 解析主表达式的右侧
		 * @param expr_pre 函数将要处理的运算符的最小优先级
		 * @param lhs 二元运算符左侧的ast结点
		 * @return
		 */
		std::unique_ptr<lex::ExprAST> parse_binOp_rhs(int expr_pre, std::unique_ptr<lex::ExprAST> lhs);

		/**
		 * 解析函数原型
		 * @return 函数原型的AST
		 */
		std::unique_ptr<lex::PrototypeAST> parse_prototype();

		/**
		 * 解析函数定义
		 * 函数定义其实就是一个原型+表达式的形式
		 * @return 函数定义的AST
		 */
		std::unique_ptr<lex::FunctionAST> parse_definition();

		/**
		 * 解析函数的extern声明
		 * @return extern声明的函数的AST
		 */
		std::unique_ptr<lex::PrototypeAST> parse_extern();

		/**
		 * 解析用户定义的任何顶级表达式
		 * @return 顶级表达式的匿名函数AST
		 */
		std::unique_ptr<lex::FunctionAST>parse_top_level_expr();
	 public:
		explicit Parser(std::shared_ptr<lex::Lexer> lexer)
			: lexer(std::move(lexer))
		{
		}
	};

} // parser

#endif //_PARSER_H_
