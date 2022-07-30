//
// Created by longjin on 22-7-29.
//

#ifndef _PARSER_H_
#define _PARSER_H_

#include <lex/lexer.h>
#include "ast.h"
#include <memory>
#include <utility>

namespace parser
{

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

	 public:
		explicit Parser(std::shared_ptr<lex::Lexer> lexer)
			: lexer(std::move(lexer))
		{
		}
	};

} // parser

#endif //_PARSER_H_
