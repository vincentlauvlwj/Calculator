/*
 * 此文件实现表达式求值的功能
 */

#include "stdafx.h"
#include "evaluate.h"

// 把Calculator.c中的操作符栈和操作数栈的作用范围扩展到此文件
extern STACK stackOptr;
extern STACK stackOpnd;

// 使用二维数组存放的运算符优先级表格
TCHAR priority[9][9] = {
	L'<', L'=', L'<', L'<', L'<', L'<', L'<', L'<', L'E', 
	L'E', L'>', L'>', L'>', L'>', L'>', L'>', L'>', L'>', 
	L'<', L'>', L'>', L'>', L'<', L'<', L'<', L'<', L'>', 
	L'<', L'>', L'>', L'>', L'<', L'<', L'<', L'<', L'>', 
	L'<', L'>', L'>', L'>', L'>', L'>', L'>', L'<', L'>', 
	L'<', L'>', L'>', L'>', L'>', L'>', L'>', L'<', L'>', 
	L'<', L'>', L'>', L'>', L'>', L'>', L'>', L'<', L'>', 
	L'<', L'>', L'>', L'>', L'>', L'>', L'>', L'<', L'>', 
	L'<', L'E', L'<', L'<', L'<', L'<', L'<', L'<', L'='
};

/*
 * 获得运算符在优先级表格中的索引
 * 参数:
 *  optr: 操作符
 * 返回值:
 *  索引
 */
int GetIndex(TCHAR optr) 
{
	switch(optr) {
		case L'(': return 0;
		case L')': return 1;
		case L'+': return 2;
		case L'-': return 3;
		case L'*': return 4;
		case L'/': return 5;
		case L'%': return 6;
		case L'^': return 7;
		case L'=': return 8;
		default : return -1;
	}
}

/*
 * 读取表达式时，用循环语句依次将字符串中的字符调用这个函数解析
 * 将读取到的连续数字字符解析为变量压入数字栈，将读取到的符号字符压入字符栈
 * 参数:
 *  c: 待解析的字符
 * 返回值:
 *  若操作正常则返回TRUE，不合法则返回非FALSE
 * 作者:
 *  李昕
 */
BOOL AnalyzeChar(TCHAR c) 
{
    static double opnd;
	static double tail = 0.1;
	static BOOL fDot = TRUE;		// 判断是否为整数部分，1代表读取整数部分，0代表读取小数部分
	static BOOL	fRBracket = FALSE;	// 判断前一个读取的字符是否为右括号
	static BOOL	fLBracket = FALSE;	// 判断前一个读取的字符是否为做括号
	static BOOL fOptr = FALSE;		// 判断前一个读取的字符是否为运算符
	int		i;						// 读取到的单个数字

    switch(c) {
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
		i = (int) (c - 48);
		if(fDot){
			// 如果在小数点前，则用原来的数乘以10再加上新读取的数即可
			opnd = opnd * 10 + i;
		}else{
			// 如果在小数点后，则直接加上当前读取的数与尾数的乘积，尾数再乘以0.1
			opnd = opnd + i * tail;
			tail *= 0.1;
		}
		fOptr = FALSE;
		fLBracket = FALSE;
		fRBracket = FALSE;
		return TRUE;

	case L'.':
		// 如果在小数点后再次读取到了小数点，则跳出switch分支语句，返回FALSE
		if(! fDot) break;
		fDot = FALSE;
		fLBracket = FALSE;
		fRBracket = FALSE;
		fOptr = FALSE;
		return TRUE;

	case L' ':
	case L'\r': return TRUE;	// 忽略空格与回车

	case L'\n': 
		// 相当于 '='
		return AnalyzeChar(L'='); 
    	
	case L'-':
		// 如果减号前面是左括号，则插入一个零，实现负号的功能
		if(fLBracket) {
			if(! AnalyzeChar('0')) break;
		}
	case L'+':
	case L'*':
	case L'/':
	case L'^':
	case L'%':
	case L'=':
		// 如果操作符前面还是一个操作符，则连续出现了两个操作符，报错
		if(fOptr) break;
		// 只有在操作符前面不是右括号的时候才把读取到的操作数压栈，避免压入多余的0
		if(! fRBracket) PushStack(double, stackOpnd, opnd);
		// 压入操作数后opnd和tail恢复默认值，准备下次读取
		opnd = 0.0;
		tail = 0.1;
		fDot = TRUE;
		fRBracket = FALSE;
		fLBracket = FALSE;
		// 如果读取到=,则fOptr应恢复默认值，否则置为TRUE
		if(c == L'=') {
			fOptr = FALSE;
		} else {
			fOptr = TRUE;
		}
		return AnalyzeOptr(c);
	case L'(':
		fLBracket = TRUE;
		return AnalyzeOptr(c);
	case L')':
		PushStack(double, stackOpnd, opnd);
		fRBracket = TRUE;
		return AnalyzeOptr(c);
    }

	// 返回FALSE前要把所有标志都恢复默认值，避免读取下一个表达式时出错
	opnd = 0.0;
	tail = 0.1;
	fDot = TRUE;
	fLBracket = FALSE;
	fRBracket = FALSE;
	fOptr = FALSE;
	return FALSE;
}

/*
 * 解析操作符
 * 参数:
 *  optr: 待解析的操作符
 * 返回值:
 *  若操作正常则返回TRUE，不合法则返回非FALSE
 * 作者:
 *  程慧涛
 */
BOOL AnalyzeOptr(TCHAR optr) {
	TCHAR top;
	double opnd1, opnd2, result;

    GetStackTop(TCHAR, stackOptr, top) ;
    switch(priority[GetIndex(top)][GetIndex(optr)]) {
    case L'<':
        PushStack(TCHAR, stackOptr, optr);
        return TRUE;
    case L'=':
		if(optr != L'=') {
			if(StackEmpty(stackOptr)) return FALSE;
			PopStack(TCHAR, stackOptr, top);
		}
        return TRUE;
    case L'>':
		if(StackEmpty(stackOptr)) return FALSE;
		PopStack(TCHAR, stackOptr, top);

		if(StackEmpty(stackOpnd)) return FALSE;
		PopStack(double, stackOpnd, opnd2);

		if(StackEmpty(stackOpnd)) return FALSE;
		PopStack(double, stackOpnd, opnd1);

		if(! Calculate(&result, opnd1, opnd2, top)) return FALSE;
		PushStack(double, stackOpnd, result);
		return AnalyzeOptr(optr);
    case L'E': // 穿透
    default:
        return FALSE;
    }
}

/*
 * 计算每一步操作符的计算结果 
 * 参数：
 *   *result :指向result变量的指针
 *   opnd1:二元操作符的左操作数
 *   opnd2：二元操作符的右操作数
 *   optr:要进行的操作符
 * 返回值：
 *   若计算出结果，返回TRUE；否则，返回FALSE
 * 作者:
 *   程慧涛
 */
BOOL Calculate(double *result, double opnd1, double opnd2, TCHAR optr)
{
	int i;
    switch(optr) {
	case L'+':
		*result = opnd1 + opnd2;
		return TRUE;
	case L'-':
		*result = opnd1 - opnd2;
		return TRUE;
	case L'*':
		*result = opnd1 * opnd2;
		return TRUE;
	case L'/':
		// 防止除零操作
		if(opnd2 == 0.0)
			return FALSE;
		*result = opnd1 / opnd2;
		return TRUE;
	case L'^':
		*result = 1;
		if(opnd2 > 0) {
			for(i = 0;i < (int) opnd2;i ++) *result *= opnd1;
		} else {
			opnd2 = -opnd2;
			for(i = 0;i < (int) opnd2;i ++) *result *= opnd1;
			*result = 1 / *result;
		}
		return TRUE;
	case L'%':
		if((int) opnd2 == 0) return FALSE;
		if((int) opnd1 != opnd1 || (int) opnd2 != opnd2) return FALSE;
		*result = (int) opnd1 % (int) opnd2;
		return TRUE;
	default:
		// 报错
		return FALSE;
    }
}