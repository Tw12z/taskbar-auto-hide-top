<h1 align="center">Taskbar auto-hide when maximized</h1>
<p align="center"><b>Top taskbar support</b> · a <a href="https://windhawk.net/">Windhawk</a> mod for Windows 11</p>

<p align="center">
  <a href="LICENSE"><img alt="License: GPL v3" src="https://img.shields.io/badge/license-GPL%20v3-blue.svg"></a>
  <img alt="Windows 11 24H2" src="https://img.shields.io/badge/Windows-11%2024H2-0078D4.svg?logo=windows11&logoColor=white">
  <img alt="Windhawk mod" src="https://img.shields.io/badge/Windhawk-mod-8A2BE2.svg">
  <img alt="Version 1.4.0" src="https://img.shields.io/badge/version-1.4.0-success.svg">
</p>

<p align="center">🇹🇷 Türkçe: <a href="README.tr.md">README.tr.md</a></p>

Hides the taskbar while a window is maximized, and shows it again when no
window needs the screen space. Works with the taskbar at the **top** of the
screen, and lets you click the empty space beside a **dock-style** taskbar
(for example the close button of a maximized window).

Based on the *Taskbar auto-hide when maximized* mod by m417z.

## Features

- ✅ **Auto-hide for a top taskbar.** Windows 11 only allows auto-hide for a
  bottom taskbar; this mod lifts that limit.
- 🖱️ **Click-through dock area.** The empty space beside a dock-style taskbar
  doesn't cover the minimize / maximize / close buttons of the window below, and
  doesn't reveal the taskbar when you touch it.
- 🎯 **Adjustable reveal area.** Follow the taskbar buttons automatically, or use
  a fixed width or fixed margins for your own layout.
- 🔄 **Less tied to one Windows build.** The internal check that blocks auto-hide
  is located at run time instead of by a build-specific name.
- ⚙️ Everything from the original mod stays: modes, excluded programs, foreground
  window only, primary monitor only.

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

## Installation

1. Install [Windhawk](https://windhawk.net/).
2. In Windhawk, click **Create a new Mod**.
3. Replace the whole editor content with [`taskbar-auto-hide-top.wh.cpp`](taskbar-auto-hide-top.wh.cpp).
4. Click **Compile Mod** (the first compile downloads the Explorer symbols and can take a minute), then leave the editor and enable the mod.

## Quick start

1. **Disable the original** *Taskbar auto-hide when maximized* mod. Both mods
   hook the same functions and must not run at the same time.
2. Install and enable this mod.
3. Maximize a window. The taskbar should slide away. Move the mouse to the top
   edge, over the dock, to bring it back.

If you don't have a dock-style taskbar (the taskbar is a full-width bar), set
**Reveal / click area** to *Full width*.

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
| **Fixed width, centered** | A fixed-width area in the center of the taskbar. |
| **Fixed margins** | Everything except a fixed margin on the left and on the right of the screen. |
| **Full width** | No clipping, the original behavior. |

#### How to tune it for your setup

* **Follow the taskbar buttons**: use *Padding on the left / right*. The padding
  has to be at least as large as the part of your dock that sticks out of the
  button span. 100 is a good start. If you use a mod that magnifies icons (for
  example *Taskbar dock animation*) and the magnified icons are cut off,
  increase it. To reveal the taskbar from a smaller part of the screen edge,
  lower it (20-40).
* **Fixed width, centered**: measure your dock from its left to its right edge
  (a screenshot in an image editor works, or the numbers in the log) and enter
  a slightly larger width. The area stays centered.
* **Fixed margins**: look at the top corners of your maximized windows. The
  window controls take about 140-150 pixels at 100% scaling (3 buttons of 46
  pixels). Set the *right* margin a bit above that (and the *left* margin to
  whatever is not part of your dock). Don't make the margins so large that the
  two overlap; then the whole area is ignored and the mod keeps the full width.

Enable **Debug logging** in Windhawk to see the computed area:
`Clipping taskbar 00010A2E to x=514..1418`.

### Other options

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
  padding or the width.
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

## Building / development

The mod is a single file. Windhawk's compiler (clang, MinGW target) builds it;
no other toolchain is needed. To find out how the top taskbar is gated, the
`taskbar.dll` symbols (`TrayUI::GetAutoHideFlags`, `TrayUI::_Hide`) were
inspected with the public Microsoft symbol server and a disassembler.

Pull requests and issue reports are welcome. When reporting a problem, please
include your Windows version (`winver`) and the Windhawk log of the mod.

## License and credits

GNU General Public License v3.0, see [LICENSE](LICENSE).

The auto-hide logic is the work of **m417z** (*Taskbar auto-hide when
maximized*, [windhawk-mods](https://github.com/ramensoftware/windhawk-mods)).
This fork adds top taskbar support and the reveal / click area.
