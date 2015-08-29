/*
 * 此头文件使用宏定义实现栈的简单功能，一定程度上弥补了C语言没有泛型的缺陷
 * 使用者应当十分谨慎地使用，在调用对应的操作"函数"时明确操作对象和操作的数据类型
 *
 * 作者：李宇涛
 */

#ifndef STACK_H
#define STACK_H
#include<stdlib.h>

#define MAX_STACK_SIZE 100		//默认初始化的栈容量
#define INCREMENT_SIZE 10		//栈容量的扩充增量

typedef struct tagSTACK{
	void *base;					//栈底指针
	void *top;					//栈顶指针
	int size;					//栈容量
} STACK;

/*
 * 栈的初始化
 * 参数:
 *  type: 栈内数据的数据类型
 *  stack: 等待初始化的栈
 * 返回值:
 *  无
 */	
#define InitStack(type, stack)												\
	{																		\
		stack.top = stack.base = malloc (sizeof(type) * MAX_STACK_SIZE);	\
		stack.size = MAX_STACK_SIZE;											\
	}

/*
 * 栈的销毁
 * 参数:
 *  type: 栈内数据的数据类型
 *  stack: 等待销毁的栈
 * 返回值:
 *  无
 */
#define DestroyStack(type, stack)						\
	{													\
		free((type*)stack.base);						\
	}

/*
 * 数据压栈
 * 参数:
 *  type: 栈内数据的数据类型
 *  stack: 等待操作的栈
 *	elem: 等待进栈的数据元素
 * 返回值:
 *  无
 */
#define PushStack(type, stack, elem)\
	{													\
		type *in = (type*)stack.top;					\
		if((type*)stack.top - (type*)stack.base >= stack.size){\
			stack.base = realloc ( stack.base, sizeof(type) * (stack.size + INCREMENT_SIZE) );\
			stack.top = (type*)stack.base + stack.size;	\
			stack.size += INCREMENT_SIZE;				\
		}												\
		*in = elem;										\
		stack.top = (type*)stack.top + 1;				\
	}

/*
 * 数据弹栈
 * 参数:
 *  type: 栈内数据的数据类型
 *  stack: 等待操作的栈
 *	elem: 接收弹栈的数据元素
 * 返回值:
 *  无
 */
#define PopStack(type, stack, elem)						\
	{													\
		stack.top = (type*)stack.top - 1;				\
		elem = *((type*)stack.top);						\
	}

/*
 * 获取栈顶元素
 *  type: 栈内数据的数据类型
 *  stack: 等待操作的栈
 *	elem: 接收栈顶的数据元素
 * 返回值:
 *  无
 */
#define GetStackTop(type, stack, elem)					\
	{													\
		elem = *((type*)stack.top - 1);					\
	}

/*
 * 判断操作栈为空
 * 参数:
 *  stack: 等待操作的栈
 * 返回值:
 *  若栈为空，返回1; 若栈不为空，返回0.
 */
#define StackEmpty(stack) (stack.top == stack.base)

/*
 * 清空栈	
 * 参数:
 *  stack: 等待操作的栈
 * 返回值:
 *  无
 */
#define ClearStack(stack) (stack.top = stack.base)

#endif