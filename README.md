# TxT --- Raw C++ Terminal Editor (Windows Only)

TxT is a minimal terminal-based text editor written in **pure C++ using
Win32 APIs**.

No frameworks.\
No GUI libraries.\
No dependencies.

Built for learning low‑level systems programming on Windows.

------------------------------------------------------------------------

## Platform Support

✅ Windows 10 / 11\
❌ Linux\
❌ macOS

This editor relies on Win32 console APIs (`HANDLE`, `ReadConsoleInput`,
cursor control).

------------------------------------------------------------------------

## Features (v1)

### Core Editor

-   Line based buffer
-   Arrow navigation
-   Character insert / delete
-   New line support
-   Vertical + horizontal scrolling
-   Status bar

### Controller Mode (Vim‑like)

Press **ESC** to enter controller mode.

Commands:

-   `:q` → quit (blocked if modified)
-   `:q!` → force quit
-   `:w` → save current file
-   `:w filename` → save as
-   `:o filename` → open file

------------------------------------------------------------------------

### File Engine

-   Open existing files
-   Create new files
-   Write buffer to disk
-   Modified flag
-   Quit protection
-   Invalid command handling
-   Cursor + scroll reset after load

------------------------------------------------------------------------

### Status Bar

Displays:

-   File name
-   Modified indicator
-   Cursor position

------------------------------------------------------------------------

## Open File at Startup

    txt.exe myfile.txt

Uses argv to load file.

------------------------------------------------------------------------

## Build (Windows)

Tested with MinGW‑w64 (GCC):

    g++ -std=c++17 main.cpp -o txt

(compile all source files)

------------------------------------------------------------------------

## Project Structure

src/ terminal/ render/ buffer/ input/ scroll/ file_controller/
file_engine/ editor/ main.cpp

Each module is separated:

-   terminal → Win32 console
-   render → drawing
-   buffer → text storage
-   input → keyboard
-   file_controller → command mode
-   file_engine → disk I/O
-   editor → core loop

------------------------------------------------------------------------

## Philosophy

TxT is designed to be:

-   Offline first
-   Single binary
-   Predictable
-   Hackable
-   Educational

Built from scratch to understand how editors actually work.

------------------------------------------------------------------------

## License

MIT (Open source)

------------------------------------------------------------------------

Mahendra Kumar --- Raw C++ Editor Project
