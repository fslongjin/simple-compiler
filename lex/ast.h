//
// Created by longjin on 22-7-29.
//

#ifndef SIMPLE_COMPILER_AST_H
#define SIMPLE_COMPILER_AST_H

#include <string>
#include <memory>
#include <vector>

namespace lex
{

	// 表达式AST的基类
	// 这是所有表达式结点的基类
	class ExprAST
	{
	 public:
		virtual ~ExprAST()
		{
		}
	};

	// 数字表达式的抽象语法树结构
	class NumberExprAST : public ExprAST
	{
	 private:
		double val;
	 public:
		explicit NumberExprAST(double val)
			: val(val)
		{
		}
	};

	// 表示一个变量的表达式类
	class VariableExprAST : public ExprAST
	{
	 private:
		std::string name;
	 public:
		explicit VariableExprAST(const std::string& name)
			: name(name)
		{
		}
	};

	// 二元运算符的ast结点
	class BinaryExprAST : public ExprAST
	{
	 private:
		// 操作数
		char opcode;
		// 指向左子节点和右子节点的指针
		std::unique_ptr<ExprAST> left_son, right_son;
	 public:
		/**
		 * 二元运算符AST结点的构造函数
		 * @param op 操作码
		 * @param left 左子节点的指针
		 * @param right 右子节点的指针
		 */
		explicit BinaryExprAST(char op, std::unique_ptr<ExprAST> left, std::unique_ptr<ExprAST> right)
			: opcode(op), left_son(std::move(left)), right_son(std::move(right))
		{
		}
	};

	// 函数调用的抽象语法树结点
	class CallExprAST : public ExprAST
	{
	 private:
		std::string callee;    // 被调用者
		std::vector<std::unique_ptr<ExprAST>> args;    // 函数调用参数列表
	 public:
		/**
		 * 函数调用AST的构造函数
		 * @param callee 被调用者
		 * @param args 函数参数
		 */
		CallExprAST(const std::string& callee, std::vector<std::unique_ptr<ExprAST>> args)
			:
			callee(callee), args(std::move(args))
		{

		}
	};

	// 函数原型的AST
	class PrototypeAST
	{
	 private:
		std::string func_name;    // 函数名
		std::vector<std::string> args;    // 参数名列表
	 public:
		/**
		 * 函数原型AST的构造函数
		 * @param name 函数名
		 * @param args 参数名列表
		 */
		PrototypeAST(const std::string& name, std::vector<std::string> args)
			: func_name(name), args(std::move(args))
		{
		}

	};

	// 函数实现的AST，表示定义函数本体
	class FunctionAST
	{
	 private:
		std::unique_ptr<PrototypeAST> prototype;    // 指向函数原型的指针
		std::unique_ptr<ExprAST> func_body;    // 指向函数体的表达式ast的指针
	 public:
		/**
		 * 函数体的AST结点的构造函数
		 * @param prototype 指向的原型的指针
		 * @param body 指向函数体内的表达式的指针
		 */
		FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<ExprAST> body)
			: prototype(std::move(prototype)), func_body(std::move(body))
		{
		}
	};

} // lex

#endif //SIMPLE_COMPILER_AST_H
