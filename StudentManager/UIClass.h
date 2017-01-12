#include "ltframe/include/InterFace.h"
#include <string>
#include "list.h"
using namespace std;

namespace UIClassSpace {

	class UIClass : public I_LTFRAME_IWebView
	{
	public:
		UIClass();
		~UIClass(void);
		virtual LRESULT HandleUserMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LPCWSTR GetWindowClassName();
		void DragWindow();
		void QuitApp();
		void MinWindow();
		void MaxWindow();
		void ResWindow();
		void PushList(LTExecState*);
		void PushList2(LTExecState*);
		virtual void OnFinalMessage() {
			delete this;
		};
	private:
		LTFrameView* ltskinview;
		wchar_t* applicationpath;
		RECT CenterWindow(int w, int h);

	};
}