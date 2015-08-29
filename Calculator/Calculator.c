/*
* ���ļ�����������ڵ��ʵ�ִ��ڻ��ĺ���
* ����: ���Ŀ�
*/
#include "stdafx.h"
#include "Calculator.h"

#define MAX_LOADSTRING 100						// �ַ�������󳤶�
#define cxWindow 240							// ���ڿ��
#define cyWindow 341							// ���ڸ߶�
#define cx (4 * cxClient / 26)					// С��ť���
#define cy (6 * cyClient / 64)					// С��ť�߶�
#define xSpace (cxClient / 26)					// ��ťˮƽ���
#define ySpace (cyClient / 64)					// ��ť��ֱ���

HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
HFONT hFont;									// �Զ�������ľ��
int cxClient, cyClient;							// ������ʾ����Ŀ�Ⱥ͸߶�
WNDPROC OldEditProc;							// �༭���Ĭ�ϴ��ڴ�����

STACK stackOptr;								// ������ջ
STACK stackOpnd;								// ������ջ

												// ���水ť��Ϣ������
struct {
	int id;
	TCHAR *szCaption;
	int x, y, width, height;
} buttons[] = {
	ID_READ,			TEXT("Read"),	0, 0, 0, 0,
	ID_WRITE,			TEXT("Write"),	0, 0, 0, 0,
	ID_BACKSPACE,		TEXT("��"),		0, 0, 0, 0,
	ID_CLEAR,			TEXT("C"),		0, 0, 0, 0,
	ID_LEFTBRACKET,		TEXT("("),		0, 0, 0, 0,
	ID_RIGHTBRACKET,	TEXT(")"),		0, 0, 0, 0,
	ID_PLUS,			TEXT("+"),		0, 0, 0, 0,
	ID_SUB,				TEXT("-"),		0, 0, 0, 0,
	ID_MULTI,			TEXT("*"),		0, 0, 0, 0,
	ID_DIVIDE,			TEXT("/"),		0, 0, 0, 0,
	ID_MOD,				TEXT("%"),		0, 0, 0, 0,
	ID_POWER,			TEXT("^"),		0, 0, 0, 0,
	ID_SUBMIT,			TEXT("="),		0, 0, 0, 0,
	ID_ZERO,			TEXT("0"),		0, 0, 0, 0,
	ID_ONE,				TEXT("1"),		0, 0, 0, 0,
	ID_TWO,				TEXT("2"),		0, 0, 0, 0,
	ID_THREE,			TEXT("3"),		0, 0, 0, 0,
	ID_FOUR,			TEXT("4"),		0, 0, 0, 0,
	ID_FIVE,			TEXT("5"),		0, 0, 0, 0,
	ID_SIX,				TEXT("6"),		0, 0, 0, 0,
	ID_SEVEN,			TEXT("7"),		0, 0, 0, 0,
	ID_EIGHT,			TEXT("8"),		0, 0, 0, 0,
	ID_NINE,			TEXT("9"),		0, 0, 0, 0,
	ID_POINT,			TEXT("."),		0, 0, 0, 0,
	ID_DBLZERO,			TEXT("00"),		0, 0, 0, 0
};

// ������ڵ�
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CALCULATOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALCULATOR));

	// ����Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

// ע�ᴰ�����
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCULATOR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(217, 228, 241));
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_CALCULATOR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

/*
* ��ʼ��ʾ�����
* ����:
*  hInstance: ��ǰ����ʵ��
*  nCmdShow: ���ڳ�ʼ״̬
* ����ֵ:
*  ��ʼ���ɹ��򷵻�TRUE
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, 0, cxWindow, cyWindow, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

/*
* �����������
*  ������
*   parent: ����ĸ����ڣ������������ڵľ��
*  ����ֵ��
*   ��
*/
VOID CreateComponents(HWND parent)
{
	int i;
	for (i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
		CreateWindow(TEXT("button"), buttons[i].szCaption,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, parent, (HMENU)buttons[i].id, hInst, NULL);
	CreateWindow(TEXT("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 0, 0, 0, parent, (HMENU)ID_EDIT, hInst, NULL);
}

/*
* ���ð�ť��λ����Ϣ
* ������
*  id: �����õİ�ť��ID
*  x: ��ť���Ͻǵ�ˮƽ����
*  y: ��ť���ϽǵĴ�ֱ����
*  width: ��ť���
*  height: ��ť�߶�
* ����ֵ��
*  ��
*/
VOID SetButtonRect(int id, int x, int y, int width, int height)
{
	buttons[id].x = x;
	buttons[id].y = y;
	buttons[id].width = width;
	buttons[id].height = height;
}

/*
* ���������λ�úʹ�С
*  ������
*   parent: ����ĸ����ڣ������������ڵľ��
*  ����ֵ��
*   ��
*/
VOID MoveComponents(HWND parent)
{
	int i;
	SetButtonRect(ID_READ, xSpace, 3 * cy + 4 * ySpace, 2 * cx + 7 * xSpace / 2, cy);
	SetButtonRect(ID_WRITE, 2 * cx + 11 * xSpace / 2, 3 * cy + 4 * ySpace, 2 * cx + 7 * xSpace / 2, cy);
	SetButtonRect(ID_BACKSPACE, xSpace, 4 * cy + 5 * ySpace, 2 * cx + xSpace, cy);
	SetButtonRect(ID_CLEAR, 2 * cx + 3 * xSpace, 4 * cy + 5 * ySpace, cx, cy);
	SetButtonRect(ID_LEFTBRACKET, 3 * cx + 4 * xSpace, 4 * cy + 5 * ySpace, cx, cy);
	SetButtonRect(ID_RIGHTBRACKET, 4 * cx + 5 * xSpace, 4 * cy + 5 * ySpace, cx, cy);
	SetButtonRect(ID_SEVEN, xSpace, 5 * cy + 6 * ySpace, cx, cy);
	SetButtonRect(ID_EIGHT, cx + 2 * xSpace, 5 * cy + 6 * ySpace, cx, cy);
	SetButtonRect(ID_NINE, 2 * cx + 3 * xSpace, 5 * cy + 6 * ySpace, cx, cy);
	SetButtonRect(ID_DIVIDE, 3 * cx + 4 * xSpace, 5 * cy + 6 * ySpace, cx, cy);
	SetButtonRect(ID_POWER, 4 * cx + 5 * xSpace, 5 * cy + 6 * ySpace, cx, cy);
	SetButtonRect(ID_FOUR, xSpace, 6 * cy + 7 * ySpace, cx, cy);
	SetButtonRect(ID_FIVE, cx + 2 * xSpace, 6 * cy + 7 * ySpace, cx, cy);
	SetButtonRect(ID_SIX, 2 * cx + 3 * xSpace, 6 * cy + 7 * ySpace, cx, cy);
	SetButtonRect(ID_MULTI, 3 * cx + 4 * xSpace, 6 * cy + 7 * ySpace, cx, cy);
	SetButtonRect(ID_MOD, 4 * cx + 5 * xSpace, 6 * cy + 7 * ySpace, cx, cy);
	SetButtonRect(ID_ONE, xSpace, 7 * cy + 8 * ySpace, cx, cy);
	SetButtonRect(ID_TWO, cx + 2 * xSpace, 7 * cy + 8 * ySpace, cx, cy);
	SetButtonRect(ID_THREE, 2 * cx + 3 * xSpace, 7 * cy + 8 * ySpace, cx, cy);
	SetButtonRect(ID_SUB, 3 * cx + 4 * xSpace, 7 * cy + 8 * ySpace, cx, cy);
	SetButtonRect(ID_SUBMIT, 4 * cx + 5 * xSpace, 7 * cy + 8 * ySpace, cx, 2 * cy + ySpace);
	SetButtonRect(ID_ZERO, xSpace, 8 * cy + 9 * ySpace, cx, cy);
	SetButtonRect(ID_DBLZERO, cx + 2 * xSpace, 8 * cy + 9 * ySpace, cx, cy);
	SetButtonRect(ID_POINT, 2 * cx + 3 * xSpace, 8 * cy + 9 * ySpace, cx, cy);
	SetButtonRect(ID_PLUS, 3 * cx + 4 * xSpace, 8 * cy + 9 * ySpace, cx, cy);

	for (i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
		MoveWindow(GetDlgItem(parent, buttons[i].id),
			buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, TRUE);
	MoveWindow(GetDlgItem(parent, ID_EDIT),
		xSpace, ySpace, 5 * cx + 4 * xSpace, 3 * cy + 2 * ySpace, TRUE);
}

// �����ڵ���Ϣ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, iFileLength, iLength, i;
	HDC hdc;
	long lfHeight;
	HANDLE hFile;
	PBYTE pBuffer;
	DWORD dwBytes;
	PTSTR pstrBuffer;

	switch (message) {
	case WM_CREATE:
		CreateComponents(hWnd);
		hdc = GetDC(NULL);
		lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(NULL, hdc);
		hFont = CreateFont(lfHeight, 0, 0, 0, FW_BOLD, TRUE, 0, 0, 0, 0, 0, 0, 0, TEXT("Times New Roman"));
		for (i = 0; i <= sizeof(buttons) / sizeof(buttons[0]); i++)
			SendMessage(GetDlgItem(hWnd, i), WM_SETFONT, (WPARAM)hFont, TRUE);
		OldEditProc = (WNDPROC)SetWindowLong(GetDlgItem(hWnd, ID_EDIT), GWL_WNDPROC, (LONG)EditProc);

		// ��ʼ��������ջ�Ͳ�����ջ������ '=' ѹ�������ջ
		InitStack(TCHAR, stackOptr);
		InitStack(double, stackOpnd);
		PushStack(TCHAR, stackOptr, L'=');
		break;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		MoveComponents(hWnd);
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		if (wmId >= ID_LEFTBRACKET && wmId <= ID_POINT) {
			SendMessage(GetDlgItem(hWnd, ID_EDIT), WM_CHAR, buttons[wmId].szCaption[0], NULL);
			break;
		}
		switch (wmId) {
		case ID_DBLZERO:
			SendMessage(GetDlgItem(hWnd, ID_EDIT), WM_CHAR, buttons[wmId].szCaption[0], NULL);
			SendMessage(GetDlgItem(hWnd, ID_EDIT), WM_CHAR, buttons[wmId].szCaption[0], NULL);
			break;
		case ID_CLEAR:
			SetWindowText(GetDlgItem(hWnd, ID_EDIT), NULL);
			break;
		case ID_BACKSPACE:
			SendMessage(GetDlgItem(hWnd, ID_EDIT), WM_CHAR, VK_BACK, 0);
			break;
		case ID_READ:
		case ID_OPEN:
			// ��input.txt�ж�ȡ���ʽ������
			/*����: �뿡��*/
			SendMessage(hWnd, WM_COMMAND, ID_CLEAR, 0);
			if (INVALID_HANDLE_VALUE == (hFile = CreateFile(TEXT("input.txt"), GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL))) {
				MessageBox(hWnd, TEXT("An error occured while opening file \"input.txt\""), szTitle, MB_ICONERROR);
				break;
			}
			iFileLength = GetFileSize(hFile, NULL);
			pBuffer = (PBYTE)malloc(iFileLength + 1);
			ReadFile(hFile, pBuffer, iFileLength, &dwBytes, NULL);
			pBuffer[iFileLength] = '\0';
			for (i = 0; i < iFileLength; i++) {
				SendMessage(GetDlgItem(hWnd, ID_EDIT), WM_CHAR, pBuffer[i], 0);
				if (pBuffer[i] == '=') {
					for (; pBuffer[i] != '\n'; i++);
				}
			}
			MessageBox(hWnd, TEXT("Calculated successfully!"), szTitle, MB_ICONINFORMATION);
			CloseHandle(hFile);
			free(pBuffer);
			break;
		case ID_WRITE:
		case ID_SAVE:
			// ������������output.txt��
			if (PathFileExists(TEXT("output.txt")))
				if (MessageBox(hWnd, TEXT("\"output.txt\" already exists. Do you want to overwrite it?"),
					szTitle, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL) break;
			if (INVALID_HANDLE_VALUE == (hFile = CreateFile(TEXT("output.txt"), GENERIC_WRITE,
				0, NULL, CREATE_ALWAYS, 0, NULL))) {
				MessageBox(hWnd, TEXT("An error occured while opening file \"output.txt\""), szTitle, MB_ICONERROR);
				break;
			}
			iLength = GetWindowTextLength(GetDlgItem(hWnd, ID_EDIT));
			pstrBuffer = (PTSTR)malloc((iLength + 1) * sizeof(TCHAR));
			GetWindowText(GetDlgItem(hWnd, ID_EDIT), pstrBuffer, iLength + 1);
			WriteFile(hFile, pstrBuffer, iLength * sizeof(TCHAR), &dwBytes, NULL);
			MessageBox(hWnd, TEXT("Written successfully!"), szTitle, MB_ICONINFORMATION);
			CloseHandle(hFile);
			free(pstrBuffer);
			break;
		case ID_EDIT:
			if (wmEvent == EN_ERRSPACE || wmEvent == EN_MAXTEXT)
				MessageBox(hWnd, TEXT("Edit control out of space."),
					szTitle, MB_OK | MB_ICONSTOP);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_SETFOCUS:
		SetFocus(GetDlgItem(hWnd, ID_EDIT));
		break;

	case WM_DESTROY:
		DeleteObject(
			(HBRUSH)SetClassLong(hWnd, GCL_HBRBACKGROUND,
				(LONG)GetStockObject(WHITE_BRUSH)));
		DeleteObject(hFont);
		// �����˳�ʱ���ٲ�����ջ�Ͳ�����ջ
		DestroyStack(TCHAR, stackOptr);
		DestroyStack(double, stackOpnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
* ���ʽ�����﷨����
*  ������
*   hWnd: �༭��ľ��
*  ����ֵ��
*   ��
*/
VOID SyntaxError(HWND hWnd)
{
	TCHAR szBuffer[MAX_LOADSTRING];
	wsprintf(szBuffer, TEXT("\r\n%s\r\n"), TEXT("Syntax Error!"));
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)szBuffer);
	MessageBox(GetParent(hWnd), TEXT("Syntax Error!"), szTitle, MB_ICONERROR);
	ClearStack(stackOpnd);
	ClearStack(stackOptr);
	PushStack(TCHAR, stackOptr, L'=');
}

// �༭�����Ϣ������
LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR szBuffer[MAX_LOADSTRING];
	int iCount, iLength, i;
	double result;

	switch (message) {
	case WM_CHAR:
		switch (wParam) {
		case L'\r':
			return 0;
		case L'\n':
		case L'=':
			// ���յ� '=' ��ʼ������������ʽ
			iCount = SendMessage(hWnd, EM_GETLINECOUNT, 0, 0);
			iLength = GetWindowTextLength(hWnd);
			SendMessage(hWnd, EM_SETSEL, iLength, iLength);
			SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)TEXT("="));
			szBuffer[0] = MAX_LOADSTRING;
			SendMessage(hWnd, EM_GETLINE, iCount - 1, (LPARAM)szBuffer);

			for (i = 0; szBuffer[i - 1] != L'='; i++) {
				if (!AnalyzeChar(szBuffer[i])) {
					SyntaxError(hWnd);
					return 0;
				}
			}
			if (StackEmpty(stackOpnd)) {
				SyntaxError(hWnd);
				return 0;
			}
			PopStack(double, stackOpnd, result);
			swprintf(szBuffer, MAX_LOADSTRING, TEXT("%g\r\n"), result);
			SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)szBuffer);
			return 0;
		}
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
			return SendMessage(hWnd, WM_CHAR, L'=', 0);
	}
	return  CallWindowProc(OldEditProc, hWnd, message, wParam, lParam);
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
