/*
 * ���ļ�ʵ�ֱ��ʽ��ֵ�Ĺ���
 */

#include "stdafx.h"
#include "evaluate.h"

// ��Calculator.c�еĲ�����ջ�Ͳ�����ջ�����÷�Χ��չ�����ļ�
extern STACK stackOptr;
extern STACK stackOpnd;

// ʹ�ö�ά�����ŵ���������ȼ����
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
 * �������������ȼ�����е�����
 * ����:
 *  optr: ������
 * ����ֵ:
 *  ����
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
 * ��ȡ���ʽʱ����ѭ��������ν��ַ����е��ַ����������������
 * ����ȡ�������������ַ�����Ϊ����ѹ������ջ������ȡ���ķ����ַ�ѹ���ַ�ջ
 * ����:
 *  c: ���������ַ�
 * ����ֵ:
 *  �����������򷵻�TRUE�����Ϸ��򷵻ط�FALSE
 * ����:
 *  ���
 */
BOOL AnalyzeChar(TCHAR c) 
{
    static double opnd;
	static double tail = 0.1;
	static BOOL fDot = TRUE;		// �ж��Ƿ�Ϊ�������֣�1�����ȡ�������֣�0�����ȡС������
	static BOOL	fRBracket = FALSE;	// �ж�ǰһ����ȡ���ַ��Ƿ�Ϊ������
	static BOOL	fLBracket = FALSE;	// �ж�ǰһ����ȡ���ַ��Ƿ�Ϊ������
	static BOOL fOptr = FALSE;		// �ж�ǰһ����ȡ���ַ��Ƿ�Ϊ�����
	int		i;						// ��ȡ���ĵ�������

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
			// �����С����ǰ������ԭ����������10�ټ����¶�ȡ��������
			opnd = opnd * 10 + i;
		}else{
			// �����С�������ֱ�Ӽ��ϵ�ǰ��ȡ������β���ĳ˻���β���ٳ���0.1
			opnd = opnd + i * tail;
			tail *= 0.1;
		}
		fOptr = FALSE;
		fLBracket = FALSE;
		fRBracket = FALSE;
		return TRUE;

	case L'.':
		// �����С������ٴζ�ȡ����С���㣬������switch��֧��䣬����FALSE
		if(! fDot) break;
		fDot = FALSE;
		fLBracket = FALSE;
		fRBracket = FALSE;
		fOptr = FALSE;
		return TRUE;

	case L' ':
	case L'\r': return TRUE;	// ���Կո���س�

	case L'\n': 
		// �൱�� '='
		return AnalyzeChar(L'='); 
    	
	case L'-':
		// �������ǰ���������ţ������һ���㣬ʵ�ָ��ŵĹ���
		if(fLBracket) {
			if(! AnalyzeChar('0')) break;
		}
	case L'+':
	case L'*':
	case L'/':
	case L'^':
	case L'%':
	case L'=':
		// ���������ǰ�滹��һ��������������������������������������
		if(fOptr) break;
		// ֻ���ڲ�����ǰ�治�������ŵ�ʱ��ŰѶ�ȡ���Ĳ�����ѹջ������ѹ������0
		if(! fRBracket) PushStack(double, stackOpnd, opnd);
		// ѹ���������opnd��tail�ָ�Ĭ��ֵ��׼���´ζ�ȡ
		opnd = 0.0;
		tail = 0.1;
		fDot = TRUE;
		fRBracket = FALSE;
		fLBracket = FALSE;
		// �����ȡ��=,��fOptrӦ�ָ�Ĭ��ֵ��������ΪTRUE
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

	// ����FALSEǰҪ�����б�־���ָ�Ĭ��ֵ�������ȡ��һ�����ʽʱ����
	opnd = 0.0;
	tail = 0.1;
	fDot = TRUE;
	fLBracket = FALSE;
	fRBracket = FALSE;
	fOptr = FALSE;
	return FALSE;
}

/*
 * ����������
 * ����:
 *  optr: �������Ĳ�����
 * ����ֵ:
 *  �����������򷵻�TRUE�����Ϸ��򷵻ط�FALSE
 * ����:
 *  �̻���
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
    case L'E': // ��͸
    default:
        return FALSE;
    }
}

/*
 * ����ÿһ���������ļ����� 
 * ������
 *   *result :ָ��result������ָ��
 *   opnd1:��Ԫ���������������
 *   opnd2����Ԫ���������Ҳ�����
 *   optr:Ҫ���еĲ�����
 * ����ֵ��
 *   ����������������TRUE�����򣬷���FALSE
 * ����:
 *   �̻���
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
		// ��ֹ�������
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
		// ����
		return FALSE;
    }
}