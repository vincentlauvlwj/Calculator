#pragma once

#include "resource.h"

#define ID_READ			0
#define ID_WRITE		1

#define ID_BACKSPACE	2
#define ID_CLEAR		3

#define ID_LEFTBRACKET	4
#define ID_RIGHTBRACKET	5
#define ID_PLUS			6
#define ID_SUB			7
#define ID_MULTI		8
#define ID_DIVIDE		9
#define ID_MOD			10
#define ID_POWER		11
#define ID_SUBMIT		12

#define ID_ZERO			13
#define ID_ONE			14
#define ID_TWO			15
#define ID_THREE		16
#define ID_FOUR			17
#define ID_FIVE			18
#define ID_SIX			19
#define ID_SEVEN		20
#define ID_EIGHT		21
#define ID_NINE			22

#define ID_POINT		23
#define ID_DBLZERO		24

#define ID_EDIT			25

ATOM				MyRegisterClass(HINSTANCE hInstance);

BOOL				InitInstance(HINSTANCE, int);

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	EditProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

VOID				CreateComponents(HWND parent);

VOID				MoveComponents(HWND parent);
