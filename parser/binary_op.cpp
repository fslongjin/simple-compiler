//
// Created by longjin on 22-7-30.
//
#include "parser.h"

namespace parser
{
	// 存放有每个二元操作符的优先级的map
	std::map<char, int> binOpPrecedence = {
		{ '<', 10 },
		{ '+', 20 },
		{ '-', 20 },
		{ '*', 40 },
	};

	/**
	 * 获取当前token的二元运算符优先级
	 * @return 优先级。若不存在，则返回-1
	 */
	int Parser::get_token_precedence()
	{
		if (!isascii(this->current_token.get_token()))
			return -1;

		// 确保当前操作符是一个二元运算符
		if (!binOpPrecedence.count((char)current_token.get_token()))
			return -1;
		return binOpPrecedence[(char)current_token.get_token()];
	}
	/**
	 * 解析表达式
	 * 表达式格式：[二元运算符， 主表达式]
	 * @return primary binary operator rhs
	 */
	std::unique_ptr<lex::ExprAST> Parser::parse_expression()
	{
		// 线解析
		auto lhs = parse_primary();
		if (!lhs)
			return nullptr;
		return parse_binOp_rhs(0, std::move(lhs));
	}

	/**
	 * 解析主表达式的右侧
	 * @param expr_pre 函数将要处理的运算符的最小优先级
	 * @param lhs 二元运算符左侧的ast结点
	 * @return
	 */
	std::unique_ptr<lex::ExprAST> Parser::parse_binOp_rhs(int expr_pre, std::unique_ptr<lex::ExprAST> lhs)
	{
		while (true)
		{
			int tok_pre = get_token_precedence();

			// 如果不是二元运算符/或者优先级较小，直接结束
			if (tok_pre < expr_pre)
				return lhs;

			int binOp = current_token.get_token();
			get_next_token();    // 消费掉当前的token

			// 解析二元操作符右侧，把它们当成主表达式来解析
			auto rhs = parse_primary();

			if (!rhs)
				return nullptr;

			// 这个时候，左右结点都被解析完了

			// 判断如何把他们连接在一起
			int next_pre = get_token_precedence();    // 右边结点的右侧的操作符的优先级
			if (tok_pre < next_pre)
			{
				// 如果右边的那个操作符的优先级更高，那么rhs就会被右边那个操作符当成它的lhs
				rhs = parse_binOp_rhs(tok_pre + 1, std::move(rhs));
				if(!rhs)
					return nullptr;
			}

			// 将lhs和rhs接在一起, 创建二元运算符AST
			lhs = std::make_unique<lex::BinaryExprAST>(binOp, std::move(lhs), std::move(rhs));
		}
	}

}// parser
