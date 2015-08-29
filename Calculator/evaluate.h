#ifndef _EVALUATE_H_
#define _EVALUATE_H_
#include <windows.h>
#include "stack.h"

BOOL AnalyzeChar(TCHAR c);

BOOL AnalyzeOptr(TCHAR optr);

BOOL Calculate(double *result, double opnd1, double opnd2, TCHAR optr);

int GetIndex(TCHAR optr);

#endif