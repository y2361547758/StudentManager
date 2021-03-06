#include "stdafx.h"
#include "UIClass.h"
#include "Shellapi.h"
#include <list>
using namespace std;
namespace UIClassSpace {

	UIClass* UIClassC = 0;
	DEFINEFUNCTION DragWindowFun(LTExecState* es)
	{
		UIClassC->DragWindow();
		return JsUndefined();
	}

	DEFINEFUNCTION QuitAppFun(LTExecState* es)
	{
		UIClassC->QuitApp();
		return JsUndefined();
	}
	DEFINEFUNCTION MinWindowFun(LTExecState* es)
	{
		UIClassC->MinWindow();
		return JsUndefined();
	}
	DEFINEFUNCTION MaxWindowFun(LTExecState* es)
	{
		UIClassC->MaxWindow();
		return JsUndefined();
	}
	DEFINEFUNCTION ResWindowFun(LTExecState* es)
	{
		UIClassC->ResWindow();
		return JsUndefined();
	}
	DEFINEFUNCTION GetListFun(LTExecState* es)
	{
		wchar_t* w = read();
		LTEncodedJSValue s = String2JsValue(es, w);
		free(w);
		return s;
	}
	DEFINEFUNCTION GetList2Fun(LTExecState* es)
	{
		list<STU>* Whole;
		list<STU>::iterator p;
		int i = 0;
		Whole = (list<STU>*) filescanf();
		LTEncodedJSValue arr = CreateObject(es);
		for (p = (*Whole).begin(); p != (*Whole).end(); ++p,++i) {
			wchar_t no[32], birth[11];
			LTEncodedJSValue value = CreateObject(es);
			char it[11];
			wsprintf(no, L"%d", p->number);
			wsprintf(birth, L"%d-%02d-%02d", p->year,p->month,p->day);
			sprintf(it, "%d", i);
			CreateJsObject(es, value, "no", String2JsValue(es, no));
			CreateJsObject(es, value, "name", String2JsValue(es, p->name));
			CreateJsObject(es, value, "sex", String2JsValue(es, p->sex));
			CreateJsObject(es, value, "birth", String2JsValue(es, birth));
			CreateJsObject(es, value, "class", Int2JsValue(p->classnumber));
			CreateJsObject(es, value, "math", Int2JsValue(p->math));
			CreateJsObject(es, value, "eng", Int2JsValue(p->English));
			CreateJsObject(es, value, "com", Int2JsValue(p->computer));
			CreateJsObject(es, value, "mil", Int2JsValue(p->military));
			CreateJsObject(es, value, "pe", Int2JsValue(p->PE));

			CreateJsObject(es, arr, it, value);
		}
		(*Whole).clear();
		return arr;
	}
	DEFINEFUNCTION PushListFun(LTExecState* es)
	{
		UIClassC->PushList(es);
		return JsUndefined();
	}
	DEFINEFUNCTION PushList2Fun(LTExecState* es)
	{
		UIClassC->PushList2(es);
		return JsUndefined();
	}
	RECT UIClass::CenterWindow(int w, int h)
	{
		int scrWidth, scrHeight;
		RECT rect;
		scrWidth = GetSystemMetrics(SM_CXSCREEN);
		scrHeight = GetSystemMetrics(SM_CYSCREEN);
		rect.left = (scrWidth - w) / 2;
		rect.top = (scrHeight - h) / 2;
		return rect;
	}

	UIClass::UIClass()
	{
		HMODULE hModule = ::GetModuleHandle(0);
		applicationpath = (wchar_t*)malloc((MAX_PATH + 1) * sizeof(wchar_t));
		GetModuleFileName(NULL, applicationpath, MAX_PATH);
		(wcsrchr(applicationpath, '\\'))[1] = 0;								//找执行文件目录
		int ww = 700, wh = 550;													//定义窗口大小
		RECT rect = CenterWindow(ww, wh);										//居中定位
		OleInitialize(0);
		ltskinview = new LTFrameView(this, L"LTFrame", NULL, WS_POPUP | WS_VISIBLE, rect.left, rect.top, ww, wh, 0);
		ltskinview->SetAllowKeyDownAutoScroll(false);							//允许/禁止鼠标拖动滚动
		UIClassC = this;
		//绑定JS函数到C函数-->
		ltskinview->BindUserFunction("DragWindow", DragWindowFun, 0);
		ltskinview->BindUserFunction("QuitApp", QuitAppFun, 0);
		ltskinview->BindUserFunction("MinWindow", MinWindowFun, 0);
		ltskinview->BindUserFunction("MaxWindow", MaxWindowFun, 0);
		ltskinview->BindUserFunction("ResWindow", ResWindowFun, 0);
		ltskinview->BindUserFunction("getList", GetListFun, 0);
		ltskinview->BindUserFunction("getList2", GetList2Fun, 0);
		ltskinview->BindUserFunction("pushList", PushListFun, 0);
		ltskinview->BindUserFunction("pushList2", PushList2Fun, 2);
		//定义JS到C++函数--<
		SetWindowText(ltskinview->windowHandle(), L"LTFrame");					//定义窗口名
		wstring path = wstring(applicationpath) + L"./html/Index.html";		//定义文件路径
		ltskinview->loadFile(path.c_str());										//加载HTML
		ltskinview->EnableDragFrameChangeSize(true);							//允许/禁止改变窗口大小
		ltskinview->MessageLoop();
	}


	UIClass::~UIClass(void)
	{
		OleUninitialize();
		free(applicationpath);
		delete ltskinview;
	}
	void UIClass::DragWindow()
	{
		ReleaseCapture();
		POINT DownPoint;
		GetCursorPos(&DownPoint);
		SendMessage(ltskinview->windowHandle(), WM_NCLBUTTONDOWN, HTCAPTION, MAKEWPARAM(DownPoint.x, DownPoint.y));
	}
	void UIClass::QuitApp()
	{
		ltskinview->QuitApp();
		//ltskinview->CloseWindow();
	}
	void UIClass::MinWindow()
	{
		::ShowWindow(ltskinview->windowHandle(), SW_MINIMIZE);
	}
	void UIClass::MaxWindow()
	{
		::ShowWindow(ltskinview->windowHandle(), SW_MAXIMIZE);
	}
	void UIClass::ResWindow()
	{
		::ShowWindow(ltskinview->windowHandle(), SW_RESTORE);
	}
	void UIClass::PushList(LTExecState* es)
	{
		char f[65535];
		JsValue2String(es, GetJsParameter(es, 0), f);
		write(f);
		return;
	}
	void UIClass::PushList2(LTExecState* es)
	{
		list<STU> Whole;
		LTEncodedJSValue txt = GetJsParameter(es, 0);
		LTEncodedJSValue l = GetJsParameter(es, 1);
		LTExecState exec = ltskinview->GetGlobalExecState();
		int i,len = JsValue2Int(es, l);
		for (i = 0; i < len; ++i) {
			STU student;
			char it[11],tmp[20];
			sprintf(it, "%d", i);
			LTEncodedJSValue row = GetJsObjectAttribute(exec, txt, it);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "no"), tmp);
			student.number = atoi(tmp);
			JsValue2StringW(exec, GetJsObjectAttribute(exec, row, "name"), student.name);
			JsValue2StringW(exec, GetJsObjectAttribute(exec, row, "sex"), student.sex);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "birth"), tmp);
			sscanf(tmp, "%d-%d-%d", &student.year, &student.month, &student.day);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "class"), tmp);
			student.classnumber = atoi(tmp);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "math"), tmp);
			student.math = atoi(tmp);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "eng"), tmp);
			student.English = atoi(tmp);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "com"), tmp);
			student.computer = atoi(tmp);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "mil"), tmp);
			student.military = atoi(tmp);
			JsValue2String(exec, GetJsObjectAttribute(exec, row, "pe"), tmp);
			student.PE = atoi(tmp);
			Whole.push_front(student);
		}
		filechange(&Whole);
		Whole.clear();
		return;
	}
	LRESULT UIClass::HandleUserMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{

		}
		return ltskinview->HandleMessage(hwnd, uMsg, wParam, lParam);
	}
	LPCWSTR UIClass::GetWindowClassName()
	{
		return L"INDEX";
	}
}