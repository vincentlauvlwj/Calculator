/*
 * ��ͷ�ļ�ʹ�ú궨��ʵ��ջ�ļ򵥹��ܣ�һ���̶����ֲ���C����û�з��͵�ȱ��
 * ʹ����Ӧ��ʮ�ֽ�����ʹ�ã��ڵ��ö�Ӧ�Ĳ���"����"ʱ��ȷ��������Ͳ�������������
 *
 * ���ߣ�������
 */

#ifndef STACK_H
#define STACK_H
#include<stdlib.h>

#define MAX_STACK_SIZE 100		//Ĭ�ϳ�ʼ����ջ����
#define INCREMENT_SIZE 10		//ջ��������������

typedef struct tagSTACK{
	void *base;					//ջ��ָ��
	void *top;					//ջ��ָ��
	int size;					//ջ����
} STACK;

/*
 * ջ�ĳ�ʼ��
 * ����:
 *  type: ջ�����ݵ���������
 *  stack: �ȴ���ʼ����ջ
 * ����ֵ:
 *  ��
 */	
#define InitStack(type, stack)												\
	{																		\
		stack.top = stack.base = malloc (sizeof(type) * MAX_STACK_SIZE);	\
		stack.size = MAX_STACK_SIZE;											\
	}

/*
 * ջ������
 * ����:
 *  type: ջ�����ݵ���������
 *  stack: �ȴ����ٵ�ջ
 * ����ֵ:
 *  ��
 */
#define DestroyStack(type, stack)						\
	{													\
		free((type*)stack.base);						\
	}

/*
 * ����ѹջ
 * ����:
 *  type: ջ�����ݵ���������
 *  stack: �ȴ�������ջ
 *	elem: �ȴ���ջ������Ԫ��
 * ����ֵ:
 *  ��
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
 * ���ݵ�ջ
 * ����:
 *  type: ջ�����ݵ���������
 *  stack: �ȴ�������ջ
 *	elem: ���յ�ջ������Ԫ��
 * ����ֵ:
 *  ��
 */
#define PopStack(type, stack, elem)						\
	{													\
		stack.top = (type*)stack.top - 1;				\
		elem = *((type*)stack.top);						\
	}

/*
 * ��ȡջ��Ԫ��
 *  type: ջ�����ݵ���������
 *  stack: �ȴ�������ջ
 *	elem: ����ջ��������Ԫ��
 * ����ֵ:
 *  ��
 */
#define GetStackTop(type, stack, elem)					\
	{													\
		elem = *((type*)stack.top - 1);					\
	}

/*
 * �жϲ���ջΪ��
 * ����:
 *  stack: �ȴ�������ջ
 * ����ֵ:
 *  ��ջΪ�գ�����1; ��ջ��Ϊ�գ�����0.
 */
#define StackEmpty(stack) (stack.top == stack.base)

/*
 * ���ջ	
 * ����:
 *  stack: �ȴ�������ջ
 * ����ֵ:
 *  ��
 */
#define ClearStack(stack) (stack.top = stack.base)

#endif