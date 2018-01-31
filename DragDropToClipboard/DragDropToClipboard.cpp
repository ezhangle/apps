// DragDropToClipboard.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include "DragDropToClipboard.h"

// Forward declarations of functions included in this code module:
INT_PTR CALLBACK DropFilesDlg(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DROP_AREA), NULL, DropFilesDlg);

    return 0;
}

// Message handler for dialog.
INT_PTR CALLBACK DropFilesDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		DragAcceptFiles(hDlg, TRUE);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
	case WM_DROPFILES:
		{
			HDROP hDropInfo = reinterpret_cast<HDROP>(wParam);
			UINT nCount = DragQueryFile(hDropInfo, -1, NULL, 0);
			std::u16string files;
			char16_t pathBuffer[MAX_PATH + 1];
			for (size_t i = 0; i < nCount; ++i)
			{
				UINT pathLength = DragQueryFile(hDropInfo, i, reinterpret_cast<LPWSTR>(pathBuffer), sizeof(pathBuffer));
				if (!files.empty())
					files += u"\r\n";
				files += pathBuffer;
			}
			DragFinish(hDropInfo);
			auto len = (files.size() + 1) * sizeof(files[0]);
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
			memcpy(GlobalLock(hMem), files.c_str(), len);
			GlobalUnlock(hMem);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hMem);
			CloseClipboard();
		}
		break;
    }
    return (INT_PTR)FALSE;
}
