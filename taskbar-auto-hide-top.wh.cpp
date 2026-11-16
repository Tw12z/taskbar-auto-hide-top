// ==WindhawkMod==
// @id              taskbar-auto-hide-top
// @name            Taskbar auto-hide when maximized (top taskbar support)
// @description     Auto-hides the taskbar when a window is maximized, also for a taskbar placed at the top, with a click-through area beside dock-style taskbars
// @version         1.4.0
// @author          Tw12z (based on m417z's taskbar-auto-hide-when-maximized)
// @github          https://github.com/Tw12z
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -ldwmapi -lgdi32 -lole32 -loleaut32 -lversion
// ==/WindhawkMod==

// Source code is published under The GNU General Public License v3.0.
//
// This mod is a fork of "Taskbar auto-hide when maximized" by m417z:
// https://github.com/ramensoftware/windhawk-mods
// The auto-hide logic (which windows hide the taskbar) is the original one.
// Added in this version: support for a taskbar placed at the top (or any
// non-bottom edge), and the dock-area / click-through feature.

// ==WindhawkModReadme==
/*
# Taskbar auto-hide when maximized (top taskbar support)

Hides the taskbar while a window is maximized, and shows it again when no
window needs the screen space. Works with the taskbar at the **top** of the
screen, and lets you click the empty space beside a **dock-style** taskbar
(for example the close button of a maximized window).

Based on the *Taskbar auto-hide when maximized* mod by m417z.

## Why a separate mod?

Windows 11 turns auto-hide off for every taskbar edge except the bottom one
("This setting is only available when the taskbar is at the bottom"). The
original mod relies on that auto-hide, so with the taskbar at the top it did
nothing. This version switches that check off, so the regular auto-hide logic
works for a top taskbar as well.

It also fixes a second problem of a top taskbar: when it slides in, the
transparent space on both sides of a dock-style taskbar covers the window
controls (minimize / maximize / close) of the maximized window, and just
touching the top edge of the screen next to the dock reveals the taskbar.

## Quick start

1. **Disable the original** *Taskbar auto-hide when maximized* mod. Both mods
   hook the same functions and must not run at the same time.
2. Install and enable this mod.
3. Maximize a window. The taskbar should slide away. Move the mouse to the top
   edge, over the dock, to bring it back.

For an ordinary full-length taskbar, set **Reveal / click area** to
*Full length (no clipping)*.

For a left/right taskbar, reveal areas run vertically: left/right padding and
margins mean top/bottom, and centered width means height. Stored setting names
are unchanged. This mod does not move the taskbar.

## Settings

### Mode

* **Auto-hide when a window is maximized or intersects the taskbar**
* **Auto-hide only when a window is maximized** (default)
* **Never auto-hide**

### Reveal / click area

The area of the taskbar window that reacts to the mouse. Everything outside
of it is "click-through": it doesn't cover windows below it, and hovering
there doesn't reveal the taskbar. All values are in **physical pixels** (at
150% scaling, 100 pixels are 66 device-independent pixels).

| Option | What it does |
| --- | --- |
| **Follow the taskbar buttons** (default) | The area is the span from the first to the last taskbar button (Start, apps, tray icons), plus the padding below. It follows the dock when apps are opened/closed. |
| **Fixed length, centered** | A centered width for top/bottom taskbars, or height for left/right taskbars. |
| **Fixed start/end margins** | Click-through space at the left/right or top/bottom ends of the taskbar. |
| **Full length** | No clipping, the original behavior. |

#### How to tune it for your setup

* **Follow the taskbar buttons**: use the start/end padding settings. The padding
  has to be at least as large as the part of your dock that sticks out of the
  button span. 100 is a good start. If you use a mod that magnifies icons (for
  example *Taskbar dock animation*) and the magnified icons are cut off,
  increase it. To reveal the taskbar from a smaller part of the screen edge,
  lower it (20-40).
* **Fixed length, centered**: measure your dock along its long axis and enter
  a slightly larger length. The area stays centered.
* **Fixed start/end margins**: for a top taskbar, the
  window controls take about 140-150 pixels at 100% scaling (3 buttons of 46
  pixels). Set the *right* margin a bit above that (and the *left* margin to
  whatever is not part of your dock). Don't make the margins so large that the
  two overlap; then the whole area is ignored and the mod keeps the full length.

Enable **Debug logging** in Windhawk to see the computed area:
`Clipping taskbar 00010A2E to x=514..1418`.

### Other options

* **Reveal delay** (default 400 ms): the mouse has to rest on the screen edge
  this long before the auto-hidden taskbar slides in, like the macOS Dock. A
  quick pass over the edge, for example to click the close button of a window
  that is snapped next to a top taskbar, doesn't reveal it. Set it to 0 for
  Windows' default (immediate) behavior.
* **Apply only to foreground window**: only the active window can hide the
  taskbar.
* **Excluded programs**: windows of these programs never hide the taskbar.
  Entries can be process names, paths or application IDs, for example
  `mspaint.exe`, `C:\Windows\System32\notepad.exe` or
  `Microsoft.WindowsCalculator_8wekyb3d8bbwe!App`.
* **Primary monitor only**: don't touch the taskbars of other monitors.
* **Customize the old taskbar on Windows 11**: only for ExplorerPatcher or a
  similar tool that brings the old taskbar back.

## Tips and troubleshooting

* **The taskbar doesn't hide after I clicked it.** Windows never auto-hides the
  active taskbar. Click into another window (for example the maximized one)
  and it hides again.
* **It doesn't hide at all.** Check that the original mod is disabled, then
  toggle this mod off and on. In the log you should see
  `Auto-hide gate hooked` once at startup. If you see
  `Auto-hide gate not found`, your Windows build uses a different internal
  layout; the mod then behaves like the original (bottom taskbar only). Please
  report your Windows version (`winver`) in an issue.
* **The taskbar opens when I touch the corner of the screen.** The area is too
  wide for your setup. Lower the padding, or use fixed margins.
* **The dock is cut off at the sides.** The area is too narrow. Increase the
  padding or the active area length.
* **After the mod was enabled the taskbar is still always visible.** Maximized
  windows must be on the same monitor as the taskbar; with *Primary monitor
  only* secondary monitors are ignored.

## Notes

* The mod hooks private Explorer internals (`TrayUI::GetAutoHideFlags`,
  `TrayUI::_Hide` and the taskbar views), so a Windows update can break it.
  It was developed and tested on Windows 11 24H2 (build 26100). The part that
  re-enables auto-hide for non-bottom taskbars finds the relevant internal
  check by itself and doesn't depend on a build number.
* The *Follow the taskbar buttons* option reads the position of the buttons
  through UI Automation in the background, about twice a second while the
  taskbar is visible.
* Credits: the auto-hide logic is by m417z. Licensed under GPL v3.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- mode: maximized
  $name: Mode
  $options:
  - intersected: Auto-hide when a window is maximized or intersects the taskbar
  - maximized: Auto-hide only when a window is maximized
  - never: Never auto-hide
- hoverArea: buttons
  $name: Reveal / click area
  $description: >-
    The part of the taskbar window that reacts to the mouse. The rest is
    click-through: it doesn't cover windows below, and hovering over it
    doesn't reveal the auto-hidden taskbar. Useful for dock-style taskbars.
  $options:
  - buttons: Follow the taskbar buttons (padding below)
  - centered: Fixed length, centered (width or height)
  - margins: Fixed start/end margins (left/right or top/bottom)
  - full: Full length (no clipping)
- paddingLeft: 100
  $name: Padding at start (left or top, pixels)
  $description: >-
    "Follow the taskbar buttons" only. Extra space before the first button,
    along the taskbar's long axis.
- paddingRight: 100
  $name: Padding at end (right or bottom, pixels)
  $description: >-
    "Follow the taskbar buttons" only. Extra space after the last button,
    along the taskbar's long axis.
- centeredWidth: 900
  $name: Centered area length (width or height, pixels)
  $description: >-
    "Fixed length, centered" only.
- marginLeft: 500
  $name: Start margin (left or top, pixels)
  $description: >-
    "Fixed start/end margins" only. Click-through space before the active
    area, along the taskbar's long axis.
- marginRight: 500
  $name: End margin (right or bottom, pixels)
  $description: >-
    "Fixed start/end margins" only. Click-through space after the active
    area, along the taskbar's long axis.
- revealDelay: 400
  $name: Reveal delay (milliseconds)
  $description: >-
    How long the mouse has to rest on the screen edge before the auto-hidden
    taskbar slides in (like the macOS Dock/menu bar). Moving the mouse across
    the edge quickly, for example to click the close button of a window that is
    snapped next to the taskbar, doesn't reveal it. 0 = reveal immediately
    (Windows default). If the taskbar doesn't react at all, set it to 0.
- foregroundWindowOnly: false
  $name: Apply only to foreground window
  $description: >-
    Enable this option to apply the auto-hide taskbar feature only to the
    selected window.
- excludedPrograms: [""]
  $name: Excluded programs
  $description: >-
    The taskbar won't auto-hide due to windows of these programs being maximized
    or intersecting the taskbar.

    Entries can be process names, paths or application IDs, for example:

    mspaint.exe

    C:\Windows\System32\notepad.exe

    Microsoft.WindowsCalculator_8wekyb3d8bbwe!App
- primaryMonitorOnly: false
  $name: Primary monitor only
  $description: >-
    Apply the mod's behavior only to the primary monitor taskbar. Secondary
    monitors will use Windows' default auto-hide behavior.
- oldTaskbarOnWin11: false
  $name: Customize the old taskbar on Windows 11
  $description: >-
    Enable this option to customize the old taskbar on Windows 11 (if using
    ExplorerPatcher or a similar tool).
*/
// ==/WindhawkModSettings==

#include <windhawk_utils.h>

#include <dwmapi.h>
#include <psapi.h>
#include <uiautomation.h>

#include <winrt/base.h>

#include <algorithm>
#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Mode {
    intersected,
    maximized,
    never,
};

enum class HoverArea {
    buttons,
    centered,
    margins,
    full,
};

struct {
    Mode mode;
    HoverArea hoverArea;
    int paddingLeft;
    int paddingRight;
    int centeredWidth;
    int marginLeft;
    int marginRight;
    int revealDelay;
    bool foregroundWindowOnly;
    std::unordered_set<std::wstring> excludedPrograms;
    bool primaryMonitorOnly;
    bool oldTaskbarOnWin11;
} g_settings;

enum class WinVersion {
    Unsupported,
    Win10,
    Win11,
    Win11_24H2,
};

WinVersion g_winVersion;

std::atomic<bool> g_taskbarViewDllLoaded;
std::atomic<bool> g_initialized;
std::atomic<bool> g_explorerPatcherInitialized;

bool g_wasAutoHideProcessed;
bool g_wasAutoHideDisabled;
std::mutex g_winEventHookThreadMutex;
std::atomic<HANDLE> g_winEventHookThread;
std::unordered_map<void*, HWND> g_taskbarsKeptShown;
std::unordered_map<HWND, void*> g_taskbarToViewCoordinator;
UINT_PTR g_pendingEventsTimer;
std::atomic<HWND> g_multitaskingViewHwnd;
std::atomic<HWND> g_altTabViewHwnd;

// TrayUI::_HandleTrayPrivateSettingMessage
constexpr UINT kHandleTrayPrivateSettingMessage = WM_USER + 0x1CA;

enum {
    kTrayPrivateSettingAutoHideGet = 3,
    kTrayPrivateSettingAutoHideSet = 4,
};

constexpr WCHAR kUpdateTaskbarStatePendingTickCount[] =
    L"Windhawk_UpdateTaskbarStatePendingTickCount_" WH_MOD_ID;

static const UINT g_getTaskbarRectRegisteredMsg =
    RegisterWindowMessage(L"Windhawk_GetTaskbarRect_" WH_MOD_ID);

static const UINT g_updateTaskbarStateRegisteredMsg =
    RegisterWindowMessage(L"Windhawk_UpdateTaskbarState_" WH_MOD_ID);

enum {
    kTrayUITimerHide = 2,
    kTrayUITimerUnhide = 3,
};

#if __cplusplus < 202302L
// Missing in older MinGW headers.
DECLARE_HANDLE(CO_MTA_USAGE_COOKIE);
WINOLEAPI CoIncrementMTAUsage(CO_MTA_USAGE_COOKIE* pCookie);
WINOLEAPI CoDecrementMTAUsage(CO_MTA_USAGE_COOKIE Cookie);
#endif

// Missing in older MinGW headers.
#ifndef EVENT_OBJECT_CLOAKED
#define EVENT_OBJECT_CLOAKED 0x8017
#endif
#ifndef EVENT_OBJECT_UNCLOAKED
#define EVENT_OBJECT_UNCLOAKED 0x8018
#endif

using IsWindowArranged_t = BOOL(WINAPI*)(HWND hwnd);
IsWindowArranged_t pIsWindowArranged;

// Private API for window band (z-order band).
// https://blog.adeltax.com/window-z-order-in-windows-10/
using GetWindowBand_t = BOOL(WINAPI*)(HWND hWnd, PDWORD pdwBand);
GetWindowBand_t pGetWindowBand;

// https://devblogs.microsoft.com/oldnewthing/20200302-00/?p=103507
bool IsWindowCloaked(HWND hwnd) {
    BOOL isCloaked = FALSE;
    return SUCCEEDED(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked,
                                           sizeof(isCloaked))) &&
           isCloaked;
}

enum class MultitaskingViewType {
    None,
    WinTab,
    AltTab,
};

// Detects Alt+Tab or Win+Tab and returns the type.
MultitaskingViewType GetMultitaskingViewType(HWND hWnd) {
    // Must be in the current process (explorer.exe).
    DWORD dwProcessId = 0;
    DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (!dwThreadId || dwProcessId != GetCurrentProcessId()) {
        return MultitaskingViewType::None;
    }

    WCHAR className[64];
    if (!GetClassName(hWnd, className, ARRAYSIZE(className)) ||
        _wcsicmp(className, L"XamlExplorerHostIslandWindow") != 0) {
        return MultitaskingViewType::None;
    }

    // The Win+Tab window uses band ZBID_IMMERSIVE_APPCHROME.
    constexpr DWORD ZBID_IMMERSIVE_APPCHROME = 5;

    // The Alt+Tab window uses band ZBID_SYSTEM_TOOLS. The virtual desktop
    // switcher (which we don't want) uses band ZBID_IMMERSIVE_EDGY.
    constexpr DWORD ZBID_SYSTEM_TOOLS = 16;

    DWORD band = 0;
    if (!pGetWindowBand || !pGetWindowBand(hWnd, &band)) {
        return MultitaskingViewType::None;
    }

    MultitaskingViewType type;
    if (band == ZBID_IMMERSIVE_APPCHROME) {
        type = MultitaskingViewType::WinTab;
    } else if (band == ZBID_SYSTEM_TOOLS) {
        type = MultitaskingViewType::AltTab;
    } else {
        return MultitaskingViewType::None;
    }

    // Check thread description for "MultitaskingView".
    HANDLE hThread =
        OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, dwThreadId);
    if (!hThread) {
        return MultitaskingViewType::None;
    }

    bool isMultitaskingView = false;
    PWSTR description = nullptr;
    if (SUCCEEDED(GetThreadDescription(hThread, &description)) && description) {
        isMultitaskingView = wcscmp(description, L"MultitaskingView") == 0;
        LocalFree(description);
    }

    CloseHandle(hThread);
    return isMultitaskingView ? type : MultitaskingViewType::None;
}

HWND FindCurrentProcessTaskbarWnd() {
    HWND hTaskbarWnd = nullptr;

    EnumWindows(
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            DWORD dwProcessId;
            WCHAR className[32];
            if (GetWindowThreadProcessId(hWnd, &dwProcessId) &&
                dwProcessId == GetCurrentProcessId() &&
                GetClassName(hWnd, className, ARRAYSIZE(className)) &&
                _wcsicmp(className, L"Shell_TrayWnd") == 0) {
                *reinterpret_cast<HWND*>(lParam) = hWnd;
                return FALSE;
            }
            return TRUE;
        },
        reinterpret_cast<LPARAM>(&hTaskbarWnd));

    return hTaskbarWnd;
}

bool IsTaskbarWindow(HWND hWnd) {
    WCHAR szClassName[32];
    if (!GetClassName(hWnd, szClassName, ARRAYSIZE(szClassName))) {
        return false;
    }

    return _wcsicmp(szClassName, L"Shell_TrayWnd") == 0 ||
           _wcsicmp(szClassName, L"Shell_SecondaryTrayWnd") == 0;
}

HWND FindTaskbarWindows(std::unordered_set<HWND>* secondaryTaskbarWindows) {
    secondaryTaskbarWindows->clear();

    HWND hTaskbarWnd = FindCurrentProcessTaskbarWnd();
    if (!hTaskbarWnd) {
        return nullptr;
    }

    DWORD taskbarThreadId = GetWindowThreadProcessId(hTaskbarWnd, nullptr);
    if (!taskbarThreadId) {
        return nullptr;
    }

    auto enumWindowsProc = [&secondaryTaskbarWindows](HWND hWnd) -> BOOL {
        WCHAR szClassName[32];
        if (GetClassName(hWnd, szClassName, ARRAYSIZE(szClassName)) == 0) {
            return TRUE;
        }

        if (_wcsicmp(szClassName, L"Shell_SecondaryTrayWnd") == 0) {
            secondaryTaskbarWindows->insert(hWnd);
        }

        return TRUE;
    };

    EnumThreadWindows(
        taskbarThreadId,
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            auto& proc = *reinterpret_cast<decltype(enumWindowsProc)*>(lParam);
            return proc(hWnd);
        },
        reinterpret_cast<LPARAM>(&enumWindowsProc));

    return hTaskbarWnd;
}

bool GetTaskbarRectForMonitor(HMONITOR monitor, RECT* rect) {
    SetRectEmpty(rect);

    HWND hTaskbarWnd = FindCurrentProcessTaskbarWnd();
    if (!hTaskbarWnd) {
        return false;
    }

    SendMessage(hTaskbarWnd, g_getTaskbarRectRegisteredMsg, (WPARAM)monitor,
                (LPARAM)rect);
    return true;
}

// https://gist.github.com/m417z/451dfc2dad88d7ba88ed1814779a26b4
std::wstring GetWindowAppId(HWND hWnd) {
    // {c8900b66-a973-584b-8cae-355b7f55341b}
    constexpr winrt::guid CLSID_StartMenuCacheAndAppResolver{
        0x660b90c8,
        0x73a9,
        0x4b58,
        {0x8c, 0xae, 0x35, 0x5b, 0x7f, 0x55, 0x34, 0x1b}};

    // {de25675a-72de-44b4-9373-05170450c140}
    constexpr winrt::guid IID_IAppResolver_8{
        0xde25675a,
        0x72de,
        0x44b4,
        {0x93, 0x73, 0x05, 0x17, 0x04, 0x50, 0xc1, 0x40}};

    struct IAppResolver_8 : public IUnknown {
       public:
        virtual HRESULT STDMETHODCALLTYPE GetAppIDForShortcut() = 0;
        virtual HRESULT STDMETHODCALLTYPE GetAppIDForShortcutObject() = 0;
        virtual HRESULT STDMETHODCALLTYPE
        GetAppIDForWindow(HWND hWnd,
                          WCHAR** pszAppId,
                          void* pUnknown1,
                          void* pUnknown2,
                          void* pUnknown3) = 0;
        virtual HRESULT STDMETHODCALLTYPE
        GetAppIDForProcess(DWORD dwProcessId,
                           WCHAR** pszAppId,
                           void* pUnknown1,
                           void* pUnknown2,
                           void* pUnknown3) = 0;
    };

    HRESULT hr;
    std::wstring result;

    CO_MTA_USAGE_COOKIE cookie;
    bool mtaUsageIncreased = SUCCEEDED(CoIncrementMTAUsage(&cookie));

    winrt::com_ptr<IAppResolver_8> appResolver;
    hr = CoCreateInstance(CLSID_StartMenuCacheAndAppResolver, nullptr,
                          CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
                          IID_IAppResolver_8, appResolver.put_void());
    if (SUCCEEDED(hr)) {
        WCHAR* pszAppId;
        hr = appResolver->GetAppIDForWindow(hWnd, &pszAppId, nullptr, nullptr,
                                            nullptr);
        if (SUCCEEDED(hr)) {
            result = pszAppId;
            CoTaskMemFree(pszAppId);
        }
    }

    appResolver = nullptr;

    if (mtaUsageIncreased) {
        CoDecrementMTAUsage(cookie);
    }

    return result;
}

std::wstring GetProcessFileName(DWORD dwProcessId) {
    HANDLE hProcess =
        OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcessId);
    if (!hProcess) {
        return std::wstring{};
    }

    WCHAR processPath[MAX_PATH];

    DWORD dwSize = ARRAYSIZE(processPath);
    if (!QueryFullProcessImageName(hProcess, 0, processPath, &dwSize)) {
        CloseHandle(hProcess);
        return std::wstring{};
    }

    CloseHandle(hProcess);

    PCWSTR processFileName = wcsrchr(processPath, L'\\');
    if (!processFileName) {
        return std::wstring{};
    }

    processFileName++;
    return processFileName;
}

std::wstring GetWindowLogInfo(HWND hWnd) {
    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    std::wstring processName = GetProcessFileName(dwProcessId);

    WCHAR className[256];
    if (!GetClassName(hWnd, className, ARRAYSIZE(className))) {
        wcscpy_s(className, L"<unknown>");
    }

    WCHAR windowName[256];
    if (!GetWindowText(hWnd, windowName, ARRAYSIZE(windowName))) {
        windowName[0] = L'\0';
    }

    LONG style = GetWindowLong(hWnd, GWL_STYLE);
    LONG exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

    RECT rect{};
    GetWindowRect(hWnd, &rect);

    WCHAR buffer[1024];
    swprintf_s(buffer,
               L"window %08X: PID=%u, process=%s, class=%s, name=%s, "
               L"style=0x%08X, exStyle=0x%08X, rect={%d,%d,%d,%d}",
               (DWORD)(DWORD_PTR)hWnd, dwProcessId, processName.c_str(),
               className, windowName, style, exStyle, rect.left, rect.top,
               rect.right, rect.bottom);
    return buffer;
}

bool IsWindowExcluded(HWND hWnd) {
    if (g_settings.excludedPrograms.empty()) {
        return false;
    }

    DWORD resolvedWindowProcessPathLen = 0;
    WCHAR resolvedWindowProcessPath[MAX_PATH];
    WCHAR resolvedWindowProcessPathUpper[MAX_PATH];

    DWORD dwProcessId = 0;
    if (GetWindowThreadProcessId(hWnd, &dwProcessId)) {
        HANDLE hProcess =
            OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcessId);
        if (hProcess) {
            DWORD dwSize = ARRAYSIZE(resolvedWindowProcessPath);
            if (QueryFullProcessImageName(hProcess, 0,
                                          resolvedWindowProcessPath, &dwSize)) {
                resolvedWindowProcessPathLen = dwSize;
            }

            CloseHandle(hProcess);
        }
    }

    if (resolvedWindowProcessPathLen > 0) {
        LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE,
                      resolvedWindowProcessPath,
                      resolvedWindowProcessPathLen + 1,
                      resolvedWindowProcessPathUpper,
                      resolvedWindowProcessPathLen + 1, nullptr, nullptr, 0);
    } else {
        *resolvedWindowProcessPath = L'\0';
        *resolvedWindowProcessPathUpper = L'\0';
    }

    if (resolvedWindowProcessPathLen > 0 &&
        g_settings.excludedPrograms.contains(resolvedWindowProcessPathUpper)) {
        return true;
    }

    if (PCWSTR programFileNameUpper =
            wcsrchr(resolvedWindowProcessPathUpper, L'\\')) {
        programFileNameUpper++;
        if (*programFileNameUpper &&
            g_settings.excludedPrograms.contains(programFileNameUpper)) {
            return true;
        }
    }

    std::wstring appId = GetWindowAppId(hWnd);
    LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE, appId.data(),
                  appId.length(), appId.data(), appId.length(), nullptr,
                  nullptr, 0);
    if (g_settings.excludedPrograms.contains(appId.c_str())) {
        return true;
    }

    return false;
}

bool CanHideTaskbarForWindow(HWND hWnd,
                             HMONITOR monitor,
                             const MONITORINFO* monitorInfo,
                             const RECT* taskbarRect) {
    if (!IsWindowVisible(hWnd) || IsWindowCloaked(hWnd) || IsIconic(hWnd) ||
        (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_NOACTIVATE)) {
        return false;
    }

    // Ignore the Alt+Tab overlay so it doesn't affect taskbar state.
    if (hWnd == g_altTabViewHwnd) {
        return false;
    }

    if (hWnd == GetShellWindow() || GetProp(hWnd, L"DesktopWindow")) {
        return false;
    }

    // Exclude menus (#32768).
    if (GetClassWord(hWnd, GCW_ATOM) == 32768) {
        return false;
    }

    // Check this after the other checks, as it's the most expensive one.
    if (IsWindowExcluded(hWnd)) {
        return false;
    }

    WINDOWPLACEMENT wp{
        .length = sizeof(WINDOWPLACEMENT),
    };

    if (GetWindowPlacement(hWnd, &wp) && wp.showCmd == SW_SHOWMAXIMIZED) {
        if (MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) == monitor) {
            return true;
        }

        return false;
    }

    bool isWindowArranged = pIsWindowArranged && pIsWindowArranged(hWnd);
    if (isWindowArranged &&
        MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) != monitor) {
        return false;
    }

    RECT windowRect{};
    DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &windowRect,
                          sizeof(windowRect));

    // Don't keep the taskbar shown for a fullscreen window.
    if (EqualRect(&windowRect, &monitorInfo->rcMonitor)) {
        return true;
    }

    // It makes sense to treat arranged windows (e.g. with Win+left) as
    // maximized, as they occupy the whole monitor height. Still check for
    // intersection, as a window can also just occupy the upper side of the
    // screen (e.g. Win+left, Win+up).
    if (g_settings.mode == Mode::intersected ||
        (g_settings.mode == Mode::maximized && isWindowArranged)) {
        RECT intersectRect;
        if (IntersectRect(&intersectRect, &windowRect, taskbarRect)) {
            return true;
        }
    }

    return false;
}

bool ShouldKeepTaskbarShown(HWND hTaskbarWnd, HMONITOR monitor) {
    if (g_settings.primaryMonitorOnly &&
        monitor != MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY)) {
        return false;
    }

    if (g_settings.mode == Mode::never) {
        return true;
    }

    // Always show taskbar when MultitaskingView (Win+Tab) is active.
    if (g_multitaskingViewHwnd) {
        return true;
    }

    MONITORINFO monitorInfo{
        .cbSize = sizeof(MONITORINFO),
    };
    GetMonitorInfo(monitor, &monitorInfo);

    RECT taskbarRect{};
    GetTaskbarRectForMonitor(monitor, &taskbarRect);

    if (g_settings.foregroundWindowOnly) {
        HWND hForegroundWnd = GetForegroundWindow();
        return !hForegroundWnd ||
               !CanHideTaskbarForWindow(hForegroundWnd, monitor, &monitorInfo,
                                        &taskbarRect);
    }

    bool canHideTaskbar = false;

    DWORD dwTaskbarThreadId = GetCurrentThreadId();

    auto enumWindowsProc = [&](HWND hWnd) -> BOOL {
        if (GetWindowThreadProcessId(hWnd, nullptr) == dwTaskbarThreadId) {
            return TRUE;
        }

        canHideTaskbar =
            CanHideTaskbarForWindow(hWnd, monitor, &monitorInfo, &taskbarRect);
        if (!canHideTaskbar) {
            return TRUE;
        }

        Wh_Log(L"Can hide taskbar %08X for %s", (DWORD)(DWORD_PTR)hTaskbarWnd,
               GetWindowLogInfo(hWnd).c_str());
        return FALSE;
    };

    EnumWindows(
        [](HWND hWnd, LPARAM lParam) -> BOOL {
            auto& proc = *reinterpret_cast<decltype(enumWindowsProc)*>(lParam);
            return proc(hWnd);
        },
        reinterpret_cast<LPARAM>(&enumWindowsProc));

    return !canHideTaskbar;
}

void* QueryViaVtable(void* object, void* vtable) {
    void* ptr = object;
    while (*(void**)ptr != vtable) {
        ptr = (void**)ptr + 1;
    }
    return ptr;
}

void* QueryViaVtableBackwards(void* object, void* vtable) {
    void* ptr = object;
    while (*(void**)ptr != vtable) {
        ptr = (void**)ptr - 1;
    }
    return ptr;
}

DWORD WINAPI WinEventHookThread(LPVOID lpThreadParameter);

void AdjustTaskbar(HWND hMMTaskbarWnd, bool clearPendingWhenDone = false) {
    if (g_settings.mode != Mode::never) {
        if (!g_winEventHookThread) {
            std::lock_guard<std::mutex> guard(g_winEventHookThreadMutex);

            if (!g_winEventHookThread) {
                g_winEventHookThread = CreateThread(
                    nullptr, 0, WinEventHookThread, nullptr, 0, nullptr);
            }
        }
    }

    PostMessage(hMMTaskbarWnd, g_updateTaskbarStateRegisteredMsg,
                clearPendingWhenDone, 0);
}

void AdjustAllTaskbars() {
    std::unordered_set<HWND> secondaryTaskbarWindows;
    HWND hWnd = FindTaskbarWindows(&secondaryTaskbarWindows);
    if (hWnd) {
        AdjustTaskbar(hWnd);
    }

    for (HWND hSecondaryWnd : secondaryTaskbarWindows) {
        AdjustTaskbar(hSecondaryWnd);
    }
}

bool AdjustAllTaskbarsIfNotPending() {
    std::unordered_set<HWND> secondaryTaskbarWindows;
    HWND hWnd = FindTaskbarWindows(&secondaryTaskbarWindows);

    DWORD currentTickCount = GetTickCount();
    DWORD pendingTickCount = 0;

    if (hWnd) {
        DWORD tickCount = (DWORD)(DWORD_PTR)GetProp(
            hWnd, kUpdateTaskbarStatePendingTickCount);
        if (tickCount > pendingTickCount) {
            pendingTickCount = tickCount;
        }
    }

    for (HWND hSecondaryWnd : secondaryTaskbarWindows) {
        DWORD tickCount = (DWORD)(DWORD_PTR)GetProp(
            hSecondaryWnd, kUpdateTaskbarStatePendingTickCount);
        if (tickCount > pendingTickCount) {
            pendingTickCount = tickCount;
        }
    }

    // Consider times larger than 10 seconds as expired, to prevent having
    // it stuck in this state.
    if (pendingTickCount && currentTickCount - pendingTickCount < 1000 * 10) {
        return false;
    }

    if (hWnd) {
        SetProp(hWnd, kUpdateTaskbarStatePendingTickCount,
                (HANDLE)(DWORD_PTR)currentTickCount);
        AdjustTaskbar(hWnd, /*clearPendingWhenDone=*/true);
    }

    for (HWND hSecondaryWnd : secondaryTaskbarWindows) {
        SetProp(hSecondaryWnd, kUpdateTaskbarStatePendingTickCount,
                (HANDLE)(DWORD_PTR)currentTickCount);
        AdjustTaskbar(hSecondaryWnd, /*clearPendingWhenDone=*/true);
    }

    return true;
}

using ViewCoordinator_ShouldTaskbarBeExpanded_t =
    bool(WINAPI*)(void* pThis, HWND hMMTaskbarWnd, bool expanded);
ViewCoordinator_ShouldTaskbarBeExpanded_t
    ViewCoordinator_ShouldTaskbarBeExpanded_Original;
bool WINAPI ViewCoordinator_ShouldTaskbarBeExpanded_Hook(void* pThis,
                                                         HWND hMMTaskbarWnd,
                                                         bool expanded) {
    Wh_Log(L"> hMMTaskbarWnd=%08X, expanded=%d",
           (DWORD)(ULONG_PTR)hMMTaskbarWnd, expanded);

    g_taskbarToViewCoordinator[hMMTaskbarWnd] = pThis;

    // Return true if the taskbar should be kept shown.
    for (const auto& pair : g_taskbarsKeptShown) {
        if (pair.second == hMMTaskbarWnd) {
            Wh_Log(L"Returning true for taskbar kept shown");
            return true;
        }
    }

    return ViewCoordinator_ShouldTaskbarBeExpanded_Original(
        pThis, hMMTaskbarWnd, expanded);
}

using ViewCoordinator_UpdateIsExpanded_t = void(WINAPI*)(void* pThis,
                                                         HWND hMMTaskbarWnd,
                                                         int reason);
ViewCoordinator_UpdateIsExpanded_t ViewCoordinator_UpdateIsExpanded_Original;

void UpdateViewCoordinatorIsExpanded(HWND hWnd) {
    // From ViewCoordinator::HandleIsPointerOverTaskbarFrameChanged.
    constexpr int kReasonIsPointerOverTaskbarFrameChanged = 7;

    if (ViewCoordinator_UpdateIsExpanded_Original) {
        auto it = g_taskbarToViewCoordinator.find(hWnd);
        if (it != g_taskbarToViewCoordinator.end()) {
            ViewCoordinator_UpdateIsExpanded_Original(
                it->second, hWnd, kReasonIsPointerOverTaskbarFrameChanged);
        }
    }
}

// Windows 11 24H2+: TrayUI::GetAutoHideFlags and TrayUI::_Hide return early
// (auto-hide disabled) when this feature is enabled and the taskbar isn't at
// the bottom. These are the only two callers, so forcing it off is enough.
//
// The feature id changes between Windows builds, so the check is located by
// its call site at the start of TrayUI::GetAutoHideFlags (see
// HookAutoHideBottomOnlyGate).
using AutoHideBottomOnlyGate_t = bool(WINAPI*)(void* pThis);
AutoHideBottomOnlyGate_t AutoHideBottomOnlyGate_Original;
bool WINAPI AutoHideBottomOnlyGate_Hook(void* pThis) {
    return false;
}

using TrayUI_GetAutoHideFlags_t = UINT(WINAPI*)(void* pThis);
TrayUI_GetAutoHideFlags_t TrayUI_GetAutoHideFlags_Original;

void* TrayUI_vftable_IInspectable;
void* TrayUI_vftable_ITrayComponentHost;
void* CSecondaryTray_vftable_ISecondaryTray;

using TrayUI_GetStuckMonitor_t = HMONITOR(WINAPI*)(void* pThis);
TrayUI_GetStuckMonitor_t TrayUI_GetStuckMonitor_Original;

using CSecondaryTray_GetMonitor_t = HMONITOR(WINAPI*)(void* pThis);
CSecondaryTray_GetMonitor_t CSecondaryTray_GetMonitor_Original;

using TrayUI_GetStuckRectForMonitor_t = bool(WINAPI*)(void* pThis,
                                                      HMONITOR hMonitor,
                                                      RECT* rect);
TrayUI_GetStuckRectForMonitor_t TrayUI_GetStuckRectForMonitor_Original;

using TrayUI_GetStuckRectForMonitor_Win10_t = RECT*(WINAPI*)(void* pThis,
                                                             RECT* rect,
                                                             HMONITOR hMonitor);
TrayUI_GetStuckRectForMonitor_Win10_t
    TrayUI_GetStuckRectForMonitor_Win10_Original;

using TrayUI__Hide_t = void(WINAPI*)(void* pThis);
TrayUI__Hide_t TrayUI__Hide_Original;
void WINAPI TrayUI__Hide_Hook(void* pThis) {
    Wh_Log(L">");

    auto it = g_taskbarsKeptShown.find(pThis);
    if (it != g_taskbarsKeptShown.end()) {
        KillTimer(it->second, kTrayUITimerHide);
        return;
    }

    TrayUI__Hide_Original(pThis);
}

using CSecondaryTray__AutoHide_t = void(WINAPI*)(void* pThis, bool param1);
CSecondaryTray__AutoHide_t CSecondaryTray__AutoHide_Original;
void WINAPI CSecondaryTray__AutoHide_Hook(void* pThis, bool param1) {
    Wh_Log(L">");

    auto it = g_taskbarsKeptShown.find(pThis);
    if (it != g_taskbarsKeptShown.end()) {
        KillTimer(it->second, kTrayUITimerHide);
        return;
    }

    CSecondaryTray__AutoHide_Original(pThis, param1);
}

using TrayUI_Unhide_t = void(WINAPI*)(void* pThis,
                                      int trayUnhideFlags,
                                      int unhideRequest);
TrayUI_Unhide_t TrayUI_Unhide_Original;

using CSecondaryTray__Unhide_t = void(WINAPI*)(void* pThis,
                                               int trayUnhideFlags,
                                               int unhideRequest);
CSecondaryTray__Unhide_t CSecondaryTray__Unhide_Original;

// Reveal delay: Explorer arms the "unhide" timer (kTrayUITimerUnhide) while the
// mouse is on the hot edge of an auto-hidden taskbar, and slides the taskbar in
// on the first tick. Swallowing the ticks until the mouse has rested for
// revealDelay milliseconds gives the macOS-like "dwell" behavior: a quick pass
// over the edge, e.g. to click the close button of a snapped window, doesn't
// reveal the taskbar. The timer keeps ticking as long as Explorer keeps it
// armed, and Explorer kills it itself once the mouse leaves the edge.
struct RevealDwell {
    DWORD start;
    DWORD last;
    POINT pt;
};

std::mutex g_revealDwellMutex;
std::unordered_map<HWND, RevealDwell> g_revealDwell;

// Returns true if this unhide tick must be swallowed.
bool ShouldDelayReveal(HWND hWnd) {
    if (g_settings.revealDelay <= 0) {
        return false;
    }

    POINT pt{};
    GetCursorPos(&pt);
    DWORD now = GetTickCount();

    std::lock_guard<std::mutex> guard(g_revealDwellMutex);

    auto it = g_revealDwell.find(hWnd);
    if (it == g_revealDwell.end() || now - it->second.last > 1000 ||
        abs(pt.x - it->second.pt.x) > 24 || abs(pt.y - it->second.pt.y) > 24) {
        // The mouse just arrived (or moved away and came back): start waiting.
        g_revealDwell[hWnd] = {now, now, pt};
        return true;
    }

    it->second.last = now;

    if (now - it->second.start >= static_cast<DWORD>(g_settings.revealDelay)) {
        g_revealDwell.erase(it);
        return false;
    }

    return true;
}

void ForgetRevealDwell(HWND hWnd) {
    std::lock_guard<std::mutex> guard(g_revealDwellMutex);
    g_revealDwell.erase(hWnd);
}

using TrayUI_WndProc_t = LRESULT(WINAPI*)(void* pThis,
                                          HWND hWnd,
                                          UINT Msg,
                                          WPARAM wParam,
                                          LPARAM lParam,
                                          bool* flag);
TrayUI_WndProc_t TrayUI_WndProc_Original;
LRESULT WINAPI TrayUI_WndProc_Hook(void* pThis,
                                   HWND hWnd,
                                   UINT Msg,
                                   WPARAM wParam,
                                   LPARAM lParam,
                                   bool* flag) {
    if (Msg == WM_NCCREATE) {
        Wh_Log(L"WM_NCCREATE: %08X", (DWORD)(ULONG_PTR)hWnd);
        AdjustTaskbar(hWnd);
    } else if (Msg == WM_NCDESTROY) {
        Wh_Log(L"WM_NCDESTROY: %08X", (DWORD)(ULONG_PTR)hWnd);
        g_taskbarsKeptShown.erase(
            QueryViaVtableBackwards(pThis, TrayUI_vftable_IInspectable));
        g_taskbarToViewCoordinator.erase(hWnd);
        ForgetRevealDwell(hWnd);
    } else if (Msg == WM_TIMER && wParam == kTrayUITimerUnhide) {
        if (ShouldDelayReveal(hWnd)) {
            return 0;
        }
    } else if (Msg == kHandleTrayPrivateSettingMessage) {
        // Prevent auto-hide from being disabled while the mod is loaded.
        if ((DWORD)wParam == 4) {
            BOOL bSetAutoHideEnabled = (BOOL)lParam;
            if (!bSetAutoHideEnabled) {
                return 0;
            }
        }
    } else if (Msg == g_getTaskbarRectRegisteredMsg) {
        HMONITOR monitor = (HMONITOR)wParam;
        RECT* rect = (RECT*)lParam;
        if (TrayUI_GetStuckRectForMonitor_Original) {
            if (!TrayUI_GetStuckRectForMonitor_Original(pThis, monitor, rect)) {
                SetRectEmpty(rect);
            }
        } else if (TrayUI_GetStuckRectForMonitor_Win10_Original) {
            TrayUI_GetStuckRectForMonitor_Win10_Original(pThis, rect, monitor);
        } else {
            SetRectEmpty(rect);
        }
    } else if (Msg == g_updateTaskbarStateRegisteredMsg) {
        if (!g_wasAutoHideProcessed) {
            g_wasAutoHideProcessed = true;
            g_wasAutoHideDisabled =
                !SendMessage(hWnd, kHandleTrayPrivateSettingMessage,
                             kTrayPrivateSettingAutoHideGet, 0);
            if (g_wasAutoHideDisabled) {
                SendMessage(hWnd, kHandleTrayPrivateSettingMessage,
                            kTrayPrivateSettingAutoHideSet, TRUE);
            }
        }

        HMONITOR monitor = TrayUI_GetStuckMonitor_Original(pThis);
        bool keepShown = ShouldKeepTaskbarShown(hWnd, monitor);

        void* pTrayUI_IInspectable =
            QueryViaVtableBackwards(pThis, TrayUI_vftable_IInspectable);

        bool keptShown = g_taskbarsKeptShown.contains(pTrayUI_IInspectable);

        if (keepShown != keptShown) {
            Wh_Log(L"> keepShown=%d", keepShown);

            if (keepShown) {
                g_taskbarsKeptShown[pTrayUI_IInspectable] = hWnd;

                void* pTrayUI_ITrayComponentHost =
                    QueryViaVtable(pThis, TrayUI_vftable_ITrayComponentHost);
                TrayUI_Unhide_Original(pTrayUI_ITrayComponentHost, 0, 0);

                UpdateViewCoordinatorIsExpanded(hWnd);
            } else {
                g_taskbarsKeptShown.erase(pTrayUI_IInspectable);

                SetTimer(hWnd, kTrayUITimerHide, 0, nullptr);

                UpdateViewCoordinatorIsExpanded(hWnd);
            }
        }

        if (wParam) {
            RemoveProp(hWnd, kUpdateTaskbarStatePendingTickCount);
        }
    }

    LRESULT ret =
        TrayUI_WndProc_Original(pThis, hWnd, Msg, wParam, lParam, flag);

    return ret;
}

using CSecondaryTray_v_WndProc_t = LRESULT(
    WINAPI*)(void* pThis, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
CSecondaryTray_v_WndProc_t CSecondaryTray_v_WndProc_Original;
LRESULT WINAPI CSecondaryTray_v_WndProc_Hook(void* pThis,
                                             HWND hWnd,
                                             UINT Msg,
                                             WPARAM wParam,
                                             LPARAM lParam) {
    if (Msg == WM_NCCREATE) {
        Wh_Log(L"WM_NCCREATE: %08X", (DWORD)(ULONG_PTR)hWnd);
        AdjustTaskbar(hWnd);
    } else if (Msg == WM_NCDESTROY) {
        Wh_Log(L"WM_NCDESTROY: %08X", (DWORD)(ULONG_PTR)hWnd);
        g_taskbarsKeptShown.erase(pThis);
        g_taskbarToViewCoordinator.erase(hWnd);
        ForgetRevealDwell(hWnd);
    } else if (Msg == WM_TIMER && wParam == kTrayUITimerUnhide) {
        if (ShouldDelayReveal(hWnd)) {
            return 0;
        }
    } else if (Msg == g_updateTaskbarStateRegisteredMsg) {
        void* pCSecondaryTray_ISecondaryTray =
            QueryViaVtable(pThis, CSecondaryTray_vftable_ISecondaryTray);

        HMONITOR monitor =
            CSecondaryTray_GetMonitor_Original(pCSecondaryTray_ISecondaryTray);

        bool keepShown = ShouldKeepTaskbarShown(hWnd, monitor);

        bool keptShown = g_taskbarsKeptShown.contains(pThis);

        if (keepShown != keptShown) {
            Wh_Log(L"> keepShown=%d", keepShown);

            if (keepShown) {
                g_taskbarsKeptShown[pThis] = hWnd;

                CSecondaryTray__Unhide_Original(pThis, 0, 0);

                UpdateViewCoordinatorIsExpanded(hWnd);
            } else {
                g_taskbarsKeptShown.erase(pThis);

                SetTimer(hWnd, kTrayUITimerHide, 0, nullptr);

                UpdateViewCoordinatorIsExpanded(hWnd);
            }
        }

        if (wParam) {
            RemoveProp(hWnd, kUpdateTaskbarStatePendingTickCount);
        }
    }

    LRESULT ret =
        CSecondaryTray_v_WndProc_Original(pThis, hWnd, Msg, wParam, lParam);

    return ret;
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook,
                           DWORD event,
                           HWND hWnd,
                           LONG idObject,
                           LONG idChild,
                           DWORD dwEventThread,
                           DWORD dwmsEventTime) {
    if (idObject != OBJID_WINDOW ||
        (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) || IsTaskbarWindow(hWnd)) {
        return;
    }

    HWND hParentWnd = GetAncestor(hWnd, GA_PARENT);
    if (hParentWnd && hParentWnd != GetDesktopWindow()) {
        return;
    }

    // Check for Multitasking View window state changes.
    {
        auto multitaskingViewType = GetMultitaskingViewType(hWnd);
        bool isTrackedWinTab = hWnd == g_multitaskingViewHwnd;
        bool isTrackedAltTab = hWnd == g_altTabViewHwnd;

        if (multitaskingViewType != MultitaskingViewType::None ||
            isTrackedWinTab || isTrackedAltTab) {
            auto& targetHwnd =
                (multitaskingViewType == MultitaskingViewType::AltTab ||
                 isTrackedAltTab)
                    ? g_altTabViewHwnd
                    : g_multitaskingViewHwnd;

            bool entering = event == EVENT_OBJECT_SHOW ||
                            event == EVENT_OBJECT_UNCLOAKED ||
                            event == EVENT_OBJECT_CREATE;
            bool leaving = event == EVENT_OBJECT_HIDE ||
                           event == EVENT_OBJECT_CLOAKED ||
                           event == EVENT_OBJECT_DESTROY;

            if (entering) {
                HWND expected = nullptr;
                if (!targetHwnd.compare_exchange_strong(expected, hWnd)) {
                    return;
                }
                Wh_Log(
                    L"MultitaskingView entering (%s)",
                    &targetHwnd == &g_altTabViewHwnd ? L"Alt+Tab" : L"Win+Tab");
            } else if (leaving) {
                HWND expected = hWnd;
                if (!targetHwnd.compare_exchange_strong(expected, nullptr)) {
                    return;
                }
                Wh_Log(
                    L"MultitaskingView leaving (%s)",
                    &targetHwnd == &g_altTabViewHwnd ? L"Alt+Tab" : L"Win+Tab");
            } else {
                return;
            }

            // Fall through to trigger timer for all taskbars.
        }
    }

    Wh_Log(
        L"Event %s for %s",
        [](DWORD event) -> PCWSTR {
            switch (event) {
                case EVENT_OBJECT_CREATE:
                    return L"OBJECT_CREATE";
                case EVENT_OBJECT_DESTROY:
                    return L"OBJECT_DESTROY";
                case EVENT_OBJECT_SHOW:
                    return L"OBJECT_SHOW";
                case EVENT_OBJECT_HIDE:
                    return L"OBJECT_HIDE";
                case EVENT_OBJECT_LOCATIONCHANGE:
                    return L"OBJECT_LOCATIONCHANGE";
                case EVENT_OBJECT_CLOAKED:
                    return L"OBJECT_CLOAKED";
                case EVENT_OBJECT_UNCLOAKED:
                    return L"OBJECT_UNCLOAKED";
                case EVENT_SYSTEM_FOREGROUND:
                    return L"SYSTEM_FOREGROUND";
                default:
                    return L"UNKNOWN";
            }
        }(event),
        GetWindowLogInfo(hWnd).c_str());

    if (g_pendingEventsTimer) {
        return;
    }

    g_pendingEventsTimer = SetTimer(
        nullptr, 0, 200,
        [](HWND hwnd,         // handle of window for timer messages
           UINT uMsg,         // WM_TIMER message
           UINT_PTR idEvent,  // timer identifier
           DWORD dwTime       // current system time
        ) {
            Wh_Log(L">");

            if (!AdjustAllTaskbarsIfNotPending()) {
                Wh_Log(L"Adjustment already pending, will retry later...");
                return;
            }

            KillTimer(nullptr, g_pendingEventsTimer);
            g_pendingEventsTimer = 0;
        });
}

DWORD WINAPI WinEventHookThread(LPVOID lpThreadParameter) {
    HWINEVENTHOOK winObjectEventHook1 =
        SetWinEventHook(EVENT_OBJECT_CREATE, EVENT_OBJECT_HIDE, nullptr,
                        WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (!winObjectEventHook1) {
        Wh_Log(L"Error: SetWinEventHook");
    }

    HWINEVENTHOOK winObjectEventHook2 = SetWinEventHook(
        EVENT_OBJECT_LOCATIONCHANGE, EVENT_OBJECT_LOCATIONCHANGE, nullptr,
        WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (!winObjectEventHook2) {
        Wh_Log(L"Error: SetWinEventHook");
    }

    HWINEVENTHOOK winObjectEventHook3 =
        SetWinEventHook(EVENT_OBJECT_CLOAKED, EVENT_OBJECT_UNCLOAKED, nullptr,
                        WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (!winObjectEventHook3) {
        Wh_Log(L"Error: SetWinEventHook");
    }

    HWINEVENTHOOK winSystemEventHook1 = nullptr;
    if (g_settings.foregroundWindowOnly) {
        winSystemEventHook1 =
            SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
                            nullptr, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
        if (!winSystemEventHook1) {
            Wh_Log(L"Error: SetWinEventHook");
        }
    }

    BOOL bRet;
    MSG msg;
    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
        if (bRet == -1) {
            msg.wParam = 0;
            break;
        }

        if (msg.hwnd == NULL && msg.message == WM_APP) {
            PostQuitMessage(0);
            continue;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (winObjectEventHook1) {
        UnhookWinEvent(winObjectEventHook1);
    }

    if (winObjectEventHook2) {
        UnhookWinEvent(winObjectEventHook2);
    }

    if (winObjectEventHook3) {
        UnhookWinEvent(winObjectEventHook3);
    }

    if (winSystemEventHook1) {
        UnhookWinEvent(winSystemEventHook1);
    }

    g_multitaskingViewHwnd = nullptr;
    g_altTabViewHwnd = nullptr;

    return 0;
}

bool HookTaskbarViewDllSymbols(HMODULE module) {
    // Taskbar.View.dll
    WindhawkUtils::SYMBOL_HOOK symbolHooks[] = {
        {
            {LR"(public: bool __cdecl winrt::Taskbar::implementation::ViewCoordinator::ShouldTaskbarBeExpanded(unsigned __int64,bool))"},
            &ViewCoordinator_ShouldTaskbarBeExpanded_Original,
            ViewCoordinator_ShouldTaskbarBeExpanded_Hook,
            true,
        },
        {
            {LR"(public: void __cdecl winrt::Taskbar::implementation::ViewCoordinator::UpdateIsExpanded(unsigned __int64,enum TaskbarTipTest::TaskbarExpandCollapseReason))"},
            &ViewCoordinator_UpdateIsExpanded_Original,
            nullptr,
            true,
        },
    };

    if (!HookSymbols(module, symbolHooks, ARRAYSIZE(symbolHooks))) {
        Wh_Log(L"HookSymbols failed");
        return false;
    }

    return true;
}

HMODULE GetTaskbarViewModuleHandle() {
    HMODULE module = GetModuleHandle(L"Taskbar.View.dll");
    if (!module) {
        module = GetModuleHandle(L"ExplorerExtensions.dll");
    }

    return module;
}

void HandleLoadedModuleIfTaskbarView(HMODULE module, LPCWSTR lpLibFileName) {
    if (g_winVersion >= WinVersion::Win11 && !g_taskbarViewDllLoaded &&
        GetTaskbarViewModuleHandle() == module &&
        !g_taskbarViewDllLoaded.exchange(true)) {
        Wh_Log(L"Loaded %s", lpLibFileName);

        if (HookTaskbarViewDllSymbols(module)) {
            Wh_ApplyHookOperations();
        }
    }
}

VS_FIXEDFILEINFO* GetModuleVersionInfo(HMODULE hModule, UINT* puPtrLen) {
    void* pFixedFileInfo = nullptr;
    UINT uPtrLen = 0;

    HRSRC hResource =
        FindResource(hModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
    if (hResource) {
        HGLOBAL hGlobal = LoadResource(hModule, hResource);
        if (hGlobal) {
            void* pData = LockResource(hGlobal);
            if (pData) {
                if (!VerQueryValue(pData, L"\\", &pFixedFileInfo, &uPtrLen) ||
                    uPtrLen == 0) {
                    pFixedFileInfo = nullptr;
                    uPtrLen = 0;
                }
            }
        }
    }

    if (puPtrLen) {
        *puPtrLen = uPtrLen;
    }

    return (VS_FIXEDFILEINFO*)pFixedFileInfo;
}

WinVersion GetExplorerVersion() {
    VS_FIXEDFILEINFO* fixedFileInfo = GetModuleVersionInfo(nullptr, nullptr);
    if (!fixedFileInfo) {
        return WinVersion::Unsupported;
    }

    WORD major = HIWORD(fixedFileInfo->dwFileVersionMS);
    WORD minor = LOWORD(fixedFileInfo->dwFileVersionMS);
    WORD build = HIWORD(fixedFileInfo->dwFileVersionLS);
    WORD qfe = LOWORD(fixedFileInfo->dwFileVersionLS);

    Wh_Log(L"Version: %u.%u.%u.%u", major, minor, build, qfe);

    switch (major) {
        case 10:
            if (build < 22000) {
                return WinVersion::Win10;
            } else if (build < 26100) {
                return WinVersion::Win11;
            } else {
                return WinVersion::Win11_24H2;
            }
            break;
    }

    return WinVersion::Unsupported;
}

struct EXPLORER_PATCHER_HOOK {
    PCSTR symbol;
    void** pOriginalFunction;
    void* hookFunction = nullptr;
    bool optional = false;

    template <typename Prototype>
    EXPLORER_PATCHER_HOOK(
        PCSTR symbol,
        Prototype** originalFunction,
        std::type_identity_t<Prototype*> hookFunction = nullptr,
        bool optional = false)
        : symbol(symbol),
          pOriginalFunction(reinterpret_cast<void**>(originalFunction)),
          hookFunction(reinterpret_cast<void*>(hookFunction)),
          optional(optional) {}
};

bool HookExplorerPatcherSymbols(HMODULE explorerPatcherModule) {
    if (g_explorerPatcherInitialized.exchange(true)) {
        return true;
    }

    if (g_winVersion >= WinVersion::Win11) {
        g_winVersion = WinVersion::Win10;
    }

    EXPLORER_PATCHER_HOOK hooks[] = {
        {R"(??_7TrayUI@@6BITrayDeskBand@@@)", &TrayUI_vftable_IInspectable},
        {R"(??_7TrayUI@@6BITrayComponentHost@@@)",
         &TrayUI_vftable_ITrayComponentHost},
        {R"(??_7CSecondaryTray@@6BISecondaryTray@@@)",
         &CSecondaryTray_vftable_ISecondaryTray},
        {R"(?GetStuckMonitor@TrayUI@@UEAAPEAUHMONITOR__@@XZ)",
         &TrayUI_GetStuckMonitor_Original},
        {R"(?GetMonitor@CSecondaryTray@@UEAAPEAUHMONITOR__@@XZ)",
         &CSecondaryTray_GetMonitor_Original},
        {R"(?GetStuckRectForMonitor@TrayUI@@UEAA_NPEAUHMONITOR__@@PEAUtagRECT@@@Z)",
         &TrayUI_GetStuckRectForMonitor_Original},
        {R"(?_Hide@TrayUI@@QEAAXXZ)", &TrayUI__Hide_Original,
         TrayUI__Hide_Hook},
        {R"(?_AutoHide@CSecondaryTray@@AEAAX_N@Z)",
         &CSecondaryTray__AutoHide_Original, CSecondaryTray__AutoHide_Hook},
        {R"(?Unhide@TrayUI@@UEAAXW4TrayUnhideFlags@TrayCommon@@W4UnhideRequest@3@@Z)",
         &TrayUI_Unhide_Original},
        {R"(?_Unhide@CSecondaryTray@@AEAAXW4TrayUnhideFlags@TrayCommon@@W4UnhideRequest@3@@Z)",
         &CSecondaryTray__Unhide_Original},
        {R"(?WndProc@TrayUI@@UEAA_JPEAUHWND__@@I_K_JPEA_N@Z)",
         &TrayUI_WndProc_Original, TrayUI_WndProc_Hook},
        {R"(?v_WndProc@CSecondaryTray@@EEAA_JPEAUHWND__@@I_K_J@Z)",
         &CSecondaryTray_v_WndProc_Original, CSecondaryTray_v_WndProc_Hook,
         // Available in versions newer than 67.1.
         true},
    };

    bool succeeded = true;

    for (const auto& hook : hooks) {
        void* ptr = (void*)GetProcAddress(explorerPatcherModule, hook.symbol);
        if (!ptr) {
            Wh_Log(L"ExplorerPatcher symbol%s doesn't exist: %S",
                   hook.optional ? L" (optional)" : L"", hook.symbol);
            if (!hook.optional) {
                succeeded = false;
            }
            continue;
        }

        if (hook.hookFunction) {
            Wh_SetFunctionHook(ptr, hook.hookFunction, hook.pOriginalFunction);
        } else {
            *hook.pOriginalFunction = ptr;
        }
    }

    if (!succeeded) {
        Wh_Log(L"HookExplorerPatcherSymbols failed");
    } else if (g_initialized) {
        Wh_ApplyHookOperations();
    }

    return succeeded;
}

bool IsExplorerPatcherModule(HMODULE module) {
    WCHAR moduleFilePath[MAX_PATH];
    switch (
        GetModuleFileName(module, moduleFilePath, ARRAYSIZE(moduleFilePath))) {
        case 0:
        case ARRAYSIZE(moduleFilePath):
            return false;
    }

    PCWSTR moduleFileName = wcsrchr(moduleFilePath, L'\\');
    if (!moduleFileName) {
        return false;
    }

    moduleFileName++;

    if (_wcsnicmp(L"ep_taskbar.", moduleFileName, sizeof("ep_taskbar.") - 1) ==
        0) {
        Wh_Log(L"ExplorerPatcher taskbar module: %s", moduleFileName);
        return true;
    }

    return false;
}

// On Windows 11 24H2 and newer the old taskbar only exists through
// ExplorerPatcher. Without it, there's nothing to hook in "old taskbar" mode.
bool IsExplorerPatcherInstalled() {
    WCHAR pattern[MAX_PATH];
    DWORD len = ExpandEnvironmentStrings(
        L"%APPDATA%\\ExplorerPatcher\\ep_taskbar.*.dll", pattern,
        ARRAYSIZE(pattern));
    if (!len || len > ARRAYSIZE(pattern)) {
        return false;
    }

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(pattern, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    FindClose(hFind);
    return true;
}

bool HandleLoadedExplorerPatcher() {
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods),
                           &cbNeeded)) {
        for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++) {
            if (IsExplorerPatcherModule(hMods[i])) {
                return HookExplorerPatcherSymbols(hMods[i]);
            }
        }
    }

    return true;
}

void HandleLoadedModuleIfExplorerPatcher(HMODULE module) {
    if (module && !((ULONG_PTR)module & 3) && !g_explorerPatcherInitialized) {
        if (IsExplorerPatcherModule(module)) {
            HookExplorerPatcherSymbols(module);
        }
    }
}

using LoadLibraryExW_t = decltype(&LoadLibraryExW);
LoadLibraryExW_t LoadLibraryExW_Original;
HMODULE WINAPI LoadLibraryExW_Hook(LPCWSTR lpLibFileName,
                                   HANDLE hFile,
                                   DWORD dwFlags) {
    HMODULE module = LoadLibraryExW_Original(lpLibFileName, hFile, dwFlags);
    if (module) {
        HandleLoadedModuleIfExplorerPatcher(module);
        HandleLoadedModuleIfTaskbarView(module, lpLibFileName);
    }

    return module;
}

// Builds that enable auto-hide only for the bottom taskbar start
// TrayUI::GetAutoHideFlags with:
//   lea rcx, [rip + disp32]   (48 8D 0D)  - feature state
//   call rel32                (E8)        - FeatureImpl<...>::__private_IsEnabled
//   test al, al               (84 C0)
// Returns the called function, or nullptr if the pattern isn't there.
void* FindAutoHideBottomOnlyGate(HMODULE module, const BYTE* func) {
    constexpr size_t kScanSize = 64;

    // Follow the adjustor thunk (sub rcx, 8; jmp rel32), if that's what the
    // symbol resolved to.
    if (func[0] == 0x48 && func[1] == 0x83 && func[2] == 0xE9 &&
        func[3] == 0x08 && func[4] == 0xE9) {
        int32_t rel;
        memcpy(&rel, func + 5, sizeof(rel));
        func = func + 9 + rel;
    }

    for (size_t i = 0; i + 14 <= kScanSize; i++) {
        if (func[i] != 0x48 || func[i + 1] != 0x8D || func[i + 2] != 0x0D ||
            func[i + 7] != 0xE8 || func[i + 12] != 0x84 ||
            func[i + 13] != 0xC0) {
            continue;
        }

        int32_t rel;
        memcpy(&rel, func + i + 8, sizeof(rel));
        const BYTE* target = func + i + 12 + rel;

        HMODULE targetModule = nullptr;
        if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                   GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               reinterpret_cast<PCWSTR>(target),
                               &targetModule) ||
            targetModule != module) {
            return nullptr;
        }

        return const_cast<BYTE*>(target);
    }

    return nullptr;
}

bool HookAutoHideBottomOnlyGate(HMODULE module) {
    if (TrayUI_GetAutoHideFlags_Original) {
        void* gate = FindAutoHideBottomOnlyGate(
            module,
            reinterpret_cast<const BYTE*>(TrayUI_GetAutoHideFlags_Original));
        if (gate) {
            Wh_Log(L"Auto-hide gate hooked: %p", gate);
            Wh_SetFunctionHook(
                gate, reinterpret_cast<void*>(AutoHideBottomOnlyGate_Hook),
                reinterpret_cast<void**>(&AutoHideBottomOnlyGate_Original));
            return true;
        }
    }

    // Fallback: the feature as it's named in Windows 11 24H2 (build 26100).
    WindhawkUtils::SYMBOL_HOOK fallbackHook[] = {
        {
            {LR"(public: bool __cdecl wil::details::FeatureImpl<struct __WilFeatureTraits_Feature_61970341>::__private_IsEnabled(void))"},
            &AutoHideBottomOnlyGate_Original,
            AutoHideBottomOnlyGate_Hook,
            true,
        },
    };

    if (HookSymbols(module, fallbackHook, ARRAYSIZE(fallbackHook)) &&
        AutoHideBottomOnlyGate_Original) {
        Wh_Log(L"Auto-hide gate hooked (by name)");
        return true;
    }

    Wh_Log(L"Auto-hide gate not found, only a bottom taskbar can auto-hide");
    return false;
}

bool HookTaskbarSymbols() {
    HMODULE module;
    if (g_winVersion <= WinVersion::Win10) {
        module = GetModuleHandle(nullptr);
    } else {
        module = LoadLibraryEx(L"taskbar.dll", nullptr,
                               LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!module) {
            Wh_Log(L"Couldn't load taskbar.dll");
            return false;
        }
    }

    // Taskbar.dll, explorer.exe
    WindhawkUtils::SYMBOL_HOOK symbolHooks[] = {
        {
            {
                // Windows 11.
                LR"(const TrayUI::`vftable'{for `IInspectable'})",

                // Windows 10.
                LR"(const TrayUI::`vftable'{for `ITrayDeskBand'})",
            },
            &TrayUI_vftable_IInspectable,
        },
        {
            {LR"(const TrayUI::`vftable'{for `ITrayComponentHost'})"},
            &TrayUI_vftable_ITrayComponentHost,
        },
        {
            {LR"(const CSecondaryTray::`vftable'{for `ISecondaryTray'})"},
            &CSecondaryTray_vftable_ISecondaryTray,
        },
        {
            {LR"(public: virtual struct HMONITOR__ * __cdecl TrayUI::GetStuckMonitor(void))"},
            &TrayUI_GetStuckMonitor_Original,
        },
        {
            {LR"(public: virtual struct HMONITOR__ * __cdecl CSecondaryTray::GetMonitor(void))"},
            &CSecondaryTray_GetMonitor_Original,
        },
        {
            {LR"(public: virtual bool __cdecl TrayUI::GetStuckRectForMonitor(struct HMONITOR__ *,struct tagRECT *))"},
            &TrayUI_GetStuckRectForMonitor_Original,
            nullptr,
            true,  // Windows 11.
        },
        {
            {LR"(public: virtual struct tagRECT __cdecl TrayUI::GetStuckRectForMonitor(struct HMONITOR__ *))"},
            &TrayUI_GetStuckRectForMonitor_Win10_Original,
            nullptr,
            true,  // Windows 10.
        },
        {
            {LR"(public: void __cdecl TrayUI::_Hide(void))"},
            &TrayUI__Hide_Original,
            TrayUI__Hide_Hook,
        },
        {
            {LR"(private: void __cdecl CSecondaryTray::_AutoHide(bool))"},
            &CSecondaryTray__AutoHide_Original,
            CSecondaryTray__AutoHide_Hook,
        },
        {
            {LR"(public: virtual void __cdecl TrayUI::Unhide(enum TrayCommon::TrayUnhideFlags,enum TrayCommon::UnhideRequest))"},
            &TrayUI_Unhide_Original,
        },
        {
            {LR"(private: void __cdecl CSecondaryTray::_Unhide(enum TrayCommon::TrayUnhideFlags,enum TrayCommon::UnhideRequest))"},
            &CSecondaryTray__Unhide_Original,
        },
        {
            {LR"(public: virtual __int64 __cdecl TrayUI::WndProc(struct HWND__ *,unsigned int,unsigned __int64,__int64,bool *))"},
            &TrayUI_WndProc_Original,
            TrayUI_WndProc_Hook,
        },
        {
            {LR"(private: virtual __int64 __cdecl CSecondaryTray::v_WndProc(struct HWND__ *,unsigned int,unsigned __int64,__int64))"},
            &CSecondaryTray_v_WndProc_Original,
            CSecondaryTray_v_WndProc_Hook,
        },
        {
            {LR"(public: virtual unsigned int __cdecl TrayUI::GetAutoHideFlags(void))"},
            &TrayUI_GetAutoHideFlags_Original,
            nullptr,
            true,  // Only used to find the bottom-only gate.
        },
    };

    if (!HookSymbols(module, symbolHooks, ARRAYSIZE(symbolHooks))) {
        return false;
    }

    if (g_winVersion >= WinVersion::Win11) {
        HookAutoHideBottomOnlyGate(module);
    }

    return true;
}

std::mutex g_dockClipThreadMutex;
HANDLE g_dockClipThread;
HANDLE g_dockClipStopEvent;

struct TaskbarButtonsRange {
    bool horizontal;
    std::pair<LONG, LONG> extent;
};

// Button extent along the taskbar's long axis, in screen coordinates.
bool GetTaskbarButtonsRange(IUIAutomation* uia,
                            HWND hWnd,
                            bool horizontal,
                            TaskbarButtonsRange* range) {
    winrt::com_ptr<IUIAutomationElement> root;
    if (FAILED(uia->ElementFromHandle(hWnd, root.put())) || !root) {
        return false;
    }

    VARIANT varButton{};
    varButton.vt = VT_I4;
    varButton.lVal = UIA_ButtonControlTypeId;

    winrt::com_ptr<IUIAutomationCondition> condition;
    if (FAILED(uia->CreatePropertyCondition(UIA_ControlTypePropertyId,
                                            varButton, condition.put()))) {
        return false;
    }

    winrt::com_ptr<IUIAutomationElementArray> buttons;
    if (FAILED(root->FindAll(TreeScope_Descendants, condition.get(),
                             buttons.put())) ||
        !buttons) {
        return false;
    }

    int count = 0;
    if (FAILED(buttons->get_Length(&count))) {
        return false;
    }

    bool found = false;
    LONG minStart = LONG_MAX;
    LONG maxEnd = LONG_MIN;

    for (int i = 0; i < count; i++) {
        winrt::com_ptr<IUIAutomationElement> button;
        RECT rect;
        if (FAILED(buttons->GetElement(i, button.put())) || !button ||
            FAILED(button->get_CurrentBoundingRectangle(&rect))) {
            continue;
        }

        LONG start = horizontal ? rect.left : rect.top;
        LONG end = horizontal ? rect.right : rect.bottom;
        if (end <= start) {
            continue;
        }

        minStart = std::min(minStart, start);
        maxEnd = std::max(maxEnd, end);
        found = true;
    }

    if (found) {
        *range = {horizontal, {minStart, maxEnd}};
    }

    return found;
}

bool IsTaskbarOnScreen(HWND hWnd, const RECT& windowRect) {
    MONITORINFO monitorInfo{
        .cbSize = sizeof(MONITORINFO),
    };
    if (!GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST),
                        &monitorInfo)) {
        return true;
    }

    // A hidden taskbar leaves a thin strip (a couple of pixels) on screen.
    RECT visibleRect;
    return IntersectRect(&visibleRect, &windowRect, &monitorInfo.rcMonitor) &&
           visibleRect.right - visibleRect.left > 8 &&
           visibleRect.bottom - visibleRect.top > 8;
}

// Computes the part of the taskbar window, in window coordinates, that stays
// active. Returns false if the whole window should stay active.
bool ComputeTaskbarActiveRange(int windowLength,
                               LONG windowStart,
                               const std::pair<LONG, LONG>* buttonsRange,
                               int* regionStart,
                               int* regionEnd) {
    switch (g_settings.hoverArea) {
        case HoverArea::buttons:
            if (!buttonsRange) {
                return false;
            }

            *regionStart =
                buttonsRange->first - g_settings.paddingLeft - windowStart;
            *regionEnd =
                buttonsRange->second + g_settings.paddingRight - windowStart;
            break;

        case HoverArea::centered:
            *regionStart = (windowLength - g_settings.centeredWidth) / 2;
            *regionEnd = *regionStart + g_settings.centeredWidth;
            break;

        case HoverArea::margins:
            *regionStart = g_settings.marginLeft;
            *regionEnd = windowLength - g_settings.marginRight;
            break;

        default:
            return false;
    }

    *regionStart = std::max(*regionStart, 0);
    *regionEnd = std::min(*regionEnd, windowLength);
    return *regionEnd > *regionStart;
}

// Limits the taskbar window to the active range, so the empty space beside the
// dock neither covers the windows behind it nor reacts to the mouse.
void UpdateTaskbarRegion(HWND hWnd,
                         const RECT& windowRect,
                         bool horizontal,
                         const std::pair<LONG, LONG>* buttonsRange,
                         std::unordered_set<HWND>& clippedWindows) {
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;

    int regionStart, regionEnd;
    bool clip = width > 0 && height > 0 &&
                ComputeTaskbarActiveRange(horizontal ? width : height,
                                          horizontal ? windowRect.left
                                                     : windowRect.top,
                                          buttonsRange, &regionStart, &regionEnd);
    if (!clip) {
        if (clippedWindows.erase(hWnd)) {
            SetWindowRgn(hWnd, nullptr, TRUE);
        }

        return;
    }
    RECT activeRect = horizontal ? RECT{regionStart, 0, regionEnd, height}
                                 : RECT{0, regionStart, width, regionEnd};

    HRGN currentRegion = CreateRectRgn(0, 0, 0, 0);
    bool unchanged = false;
    if (currentRegion) {
        RECT box;
        if (GetWindowRgn(hWnd, currentRegion) == SIMPLEREGION &&
            GetRgnBox(currentRegion, &box) != ERROR) {
            unchanged = EqualRect(&box, &activeRect);
        }

        DeleteObject(currentRegion);
    }

    if (unchanged) {
        clippedWindows.insert(hWnd);
        return;
    }

    Wh_Log(L"Clipping taskbar %08X to %s=%d..%d", (DWORD)(DWORD_PTR)hWnd,
           horizontal ? L"x" : L"y", regionStart, regionEnd);

    // The system owns the region after a successful call.
    HRGN region = CreateRectRgnIndirect(&activeRect);
    if (region) {
        if (SetWindowRgn(hWnd, region, TRUE)) {
            clippedWindows.insert(hWnd);
        } else {
            DeleteObject(region);
        }
    }
}

std::vector<HWND> GetAllTaskbarWindows() {
    std::unordered_set<HWND> secondaryTaskbarWindows;
    HWND hWnd = FindTaskbarWindows(&secondaryTaskbarWindows);

    std::vector<HWND> result;
    if (hWnd) {
        result.push_back(hWnd);
    }

    result.insert(result.end(), secondaryTaskbarWindows.begin(),
                  secondaryTaskbarWindows.end());
    return result;
}

DWORD WINAPI DockClipThread(LPVOID lpThreadParameter) {
    // {ff48dba4-60ef-4201-aa87-54103eef594e}
    constexpr winrt::guid CLSID_CUIAutomation_{
        0xff48dba4,
        0x60ef,
        0x4201,
        {0xaa, 0x87, 0x54, 0x10, 0x3e, 0xef, 0x59, 0x4e}};

    // {30cbe57d-d9d0-452a-ab13-7ac5ac4825ee}
    constexpr winrt::guid IID_IUIAutomation_{
        0x30cbe57d,
        0xd9d0,
        0x452a,
        {0xab, 0x13, 0x7a, 0xc5, 0xac, 0x48, 0x25, 0xee}};

    HRESULT hrInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // UI Automation is only needed to follow the taskbar buttons.
    winrt::com_ptr<IUIAutomation> uia;
    bool uiaRequired = g_settings.hoverArea == HoverArea::buttons;
    if (uiaRequired) {
        HRESULT hr = CoCreateInstance(CLSID_CUIAutomation_, nullptr,
                                      CLSCTX_INPROC_SERVER, IID_IUIAutomation_,
                                      uia.put_void());
        if (FAILED(hr)) {
            Wh_Log(L"Creating IUIAutomation failed: %08X", hr);
        }
    }

    if (!uiaRequired || uia) {
        // Remembered because the buttons can't be queried reliably while the
        // taskbar is slid out of the screen.
        std::unordered_map<HWND, TaskbarButtonsRange> lastRanges;
        std::unordered_set<HWND> clippedWindows;

        DWORD waitMs;
        do {
            bool anyTaskbarOnScreen = false;

            for (HWND hWnd : GetAllTaskbarWindows()) {
                RECT windowRect;
                if (!GetWindowRect(hWnd, &windowRect)) {
                    continue;
                }
                bool horizontal = windowRect.right - windowRect.left >=
                                  windowRect.bottom - windowRect.top;
                auto cached = lastRanges.find(hWnd);
                if (cached != lastRanges.end() &&
                    cached->second.horizontal != horizontal) {
                    lastRanges.erase(cached);
                }

                bool onScreen = IsTaskbarOnScreen(hWnd, windowRect);
                anyTaskbarOnScreen = anyTaskbarOnScreen || onScreen;

                // Query the buttons while the taskbar is visible (that's when
                // they can change), and once at startup.
                if (uia && (onScreen || !lastRanges.contains(hWnd))) {
                    TaskbarButtonsRange range;
                    if (GetTaskbarButtonsRange(uia.get(), hWnd, horizontal,
                                               &range)) {
                        lastRanges[hWnd] = range;
                    }
                }

                auto it = lastRanges.find(hWnd);
                UpdateTaskbarRegion(
                    hWnd, windowRect, horizontal,
                    it != lastRanges.end() ? &it->second.extent : nullptr,
                    clippedWindows);
            }

            waitMs = anyTaskbarOnScreen ? 500 : 1500;
        } while (WaitForSingleObject(g_dockClipStopEvent, waitMs) ==
                 WAIT_TIMEOUT);

        for (HWND hWnd : clippedWindows) {
            SetWindowRgn(hWnd, nullptr, TRUE);
        }
    }

    uia = nullptr;

    if (SUCCEEDED(hrInit)) {
        CoUninitialize();
    }

    return 0;
}

void StartDockClipThread() {
    std::lock_guard<std::mutex> guard(g_dockClipThreadMutex);

    if (g_settings.hoverArea == HoverArea::full || g_dockClipThread) {
        return;
    }

    g_dockClipStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!g_dockClipStopEvent) {
        return;
    }

    g_dockClipThread =
        CreateThread(nullptr, 0, DockClipThread, nullptr, 0, nullptr);
    if (!g_dockClipThread) {
        CloseHandle(g_dockClipStopEvent);
        g_dockClipStopEvent = nullptr;
    }
}

void StopDockClipThread() {
    std::lock_guard<std::mutex> guard(g_dockClipThreadMutex);

    if (!g_dockClipThread) {
        return;
    }

    SetEvent(g_dockClipStopEvent);
    WaitForSingleObject(g_dockClipThread, 5000);

    CloseHandle(g_dockClipThread);
    g_dockClipThread = nullptr;

    CloseHandle(g_dockClipStopEvent);
    g_dockClipStopEvent = nullptr;
}

void LoadSettings() {
    PCWSTR mode = Wh_GetStringSetting(L"mode");
    g_settings.mode = Mode::intersected;
    if (wcscmp(mode, L"maximized") == 0) {
        g_settings.mode = Mode::maximized;
    } else if (wcscmp(mode, L"never") == 0) {
        g_settings.mode = Mode::never;
    }
    Wh_FreeStringSetting(mode);

    g_settings.foregroundWindowOnly = Wh_GetIntSetting(L"foregroundWindowOnly");

    g_settings.excludedPrograms.clear();

    for (int i = 0;; i++) {
        PCWSTR program = Wh_GetStringSetting(L"excludedPrograms[%d]", i);

        bool hasProgram = *program;
        if (hasProgram) {
            std::wstring programUpper = program;
            LCMapStringEx(
                LOCALE_NAME_USER_DEFAULT, LCMAP_UPPERCASE, &programUpper[0],
                static_cast<int>(programUpper.length()), &programUpper[0],
                static_cast<int>(programUpper.length()), nullptr, nullptr, 0);

            g_settings.excludedPrograms.insert(std::move(programUpper));
        }

        Wh_FreeStringSetting(program);

        if (!hasProgram) {
            break;
        }
    }

    g_settings.primaryMonitorOnly = Wh_GetIntSetting(L"primaryMonitorOnly");
    g_settings.oldTaskbarOnWin11 = Wh_GetIntSetting(L"oldTaskbarOnWin11");

    PCWSTR hoverArea = Wh_GetStringSetting(L"hoverArea");
    g_settings.hoverArea = HoverArea::buttons;
    if (wcscmp(hoverArea, L"centered") == 0) {
        g_settings.hoverArea = HoverArea::centered;
    } else if (wcscmp(hoverArea, L"margins") == 0) {
        g_settings.hoverArea = HoverArea::margins;
    } else if (wcscmp(hoverArea, L"full") == 0) {
        g_settings.hoverArea = HoverArea::full;
    }
    Wh_FreeStringSetting(hoverArea);

    g_settings.paddingLeft = std::max(0, Wh_GetIntSetting(L"paddingLeft"));
    g_settings.paddingRight = std::max(0, Wh_GetIntSetting(L"paddingRight"));
    g_settings.centeredWidth = std::max(1, Wh_GetIntSetting(L"centeredWidth"));
    g_settings.marginLeft = std::max(0, Wh_GetIntSetting(L"marginLeft"));
    g_settings.marginRight = std::max(0, Wh_GetIntSetting(L"marginRight"));
    g_settings.revealDelay =
        std::clamp(Wh_GetIntSetting(L"revealDelay"), 0, 5000);

    // Without ExplorerPatcher, no old taskbar exists on 24H2+, and the mod
    // would silently do nothing. Use the regular taskbar instead.
    if (g_settings.oldTaskbarOnWin11 &&
        GetExplorerVersion() >= WinVersion::Win11_24H2 &&
        !IsExplorerPatcherInstalled()) {
        Wh_Log(L"Old taskbar requested, but ExplorerPatcher isn't installed");
        g_settings.oldTaskbarOnWin11 = false;
    }
}

BOOL Wh_ModInit() {
    Wh_Log(L">");

    LoadSettings();

    HMODULE hUser32Module =
        LoadLibraryEx(L"user32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hUser32Module) {
        pIsWindowArranged = (IsWindowArranged_t)GetProcAddress(
            hUser32Module, "IsWindowArranged");
        pGetWindowBand =
            (GetWindowBand_t)GetProcAddress(hUser32Module, "GetWindowBand");
    }

    g_winVersion = GetExplorerVersion();
    if (g_winVersion == WinVersion::Unsupported) {
        Wh_Log(L"Unsupported Windows version");
        return FALSE;
    }

    if (g_settings.oldTaskbarOnWin11) {
        bool hasWin10Taskbar = g_winVersion < WinVersion::Win11_24H2;

        if (g_winVersion >= WinVersion::Win11) {
            g_winVersion = WinVersion::Win10;
        }

        if (hasWin10Taskbar && !HookTaskbarSymbols()) {
            return FALSE;
        }
    } else if (g_winVersion >= WinVersion::Win11) {
        if (HMODULE taskbarViewModule = GetTaskbarViewModuleHandle()) {
            g_taskbarViewDllLoaded = true;
            if (!HookTaskbarViewDllSymbols(taskbarViewModule)) {
                return FALSE;
            }
        } else {
            Wh_Log(L"Taskbar view module not loaded yet");
        }

        if (!HookTaskbarSymbols()) {
            return FALSE;
        }
    } else {
        if (!HookTaskbarSymbols()) {
            return FALSE;
        }
    }

    if (!HandleLoadedExplorerPatcher()) {
        Wh_Log(L"HandleLoadedExplorerPatcher failed");
        return FALSE;
    }

    HMODULE kernelBaseModule = GetModuleHandle(L"kernelbase.dll");
    auto pKernelBaseLoadLibraryExW = (decltype(&LoadLibraryExW))GetProcAddress(
        kernelBaseModule, "LoadLibraryExW");
    WindhawkUtils::Wh_SetFunctionHookT(pKernelBaseLoadLibraryExW,
                                       LoadLibraryExW_Hook,
                                       &LoadLibraryExW_Original);

    g_initialized = true;

    return TRUE;
}

void Wh_ModAfterInit() {
    Wh_Log(L">");

    if (g_winVersion >= WinVersion::Win11 && !g_taskbarViewDllLoaded) {
        if (HMODULE taskbarViewModule = GetTaskbarViewModuleHandle()) {
            if (!g_taskbarViewDllLoaded.exchange(true)) {
                Wh_Log(L"Got Taskbar.View.dll");

                if (HookTaskbarViewDllSymbols(taskbarViewModule)) {
                    Wh_ApplyHookOperations();
                }
            }
        }
    }

    // Try again in case there's a race between the previous attempt and the
    // LoadLibraryExW hook.
    if (!g_explorerPatcherInitialized) {
        HandleLoadedExplorerPatcher();
    }

    WNDCLASS wndclass;
    if (GetClassInfo(GetModuleHandle(nullptr), L"Shell_TrayWnd", &wndclass)) {
        AdjustAllTaskbars();
    }

    StartDockClipThread();
}

void Wh_ModUninit() {
    Wh_Log(L">");

    StopDockClipThread();

    if (g_winEventHookThread) {
        PostThreadMessage(GetThreadId(g_winEventHookThread), WM_APP, 0, 0);
        WaitForSingleObject(g_winEventHookThread, INFINITE);
        CloseHandle(g_winEventHookThread);
        g_winEventHookThread = nullptr;
    }

    if (g_wasAutoHideDisabled) {
        HWND hTaskbarWnd = FindCurrentProcessTaskbarWnd();
        if (hTaskbarWnd) {
            SendMessage(hTaskbarWnd, kHandleTrayPrivateSettingMessage,
                        kTrayPrivateSettingAutoHideSet, FALSE);
        }
    }
}

BOOL Wh_ModSettingsChanged(BOOL* bReload) {
    Wh_Log(L">");

    bool prevForegroundWindowOnly = g_settings.foregroundWindowOnly;
    bool prevOldTaskbarOnWin11 = g_settings.oldTaskbarOnWin11;

    LoadSettings();

    if (g_settings.oldTaskbarOnWin11 != prevOldTaskbarOnWin11) {
        *bReload = TRUE;
        return TRUE;
    }

    if (g_settings.mode == Mode::never ||
        prevForegroundWindowOnly != g_settings.foregroundWindowOnly) {
        std::lock_guard<std::mutex> guard(g_winEventHookThreadMutex);

        if (g_winEventHookThread) {
            PostThreadMessage(GetThreadId(g_winEventHookThread), WM_APP, 0, 0);
            WaitForSingleObject(g_winEventHookThread, INFINITE);
            CloseHandle(g_winEventHookThread);
            g_winEventHookThread = nullptr;
        }
    }

    StopDockClipThread();
    StartDockClipThread();

    AdjustAllTaskbars();

    return TRUE;
}
