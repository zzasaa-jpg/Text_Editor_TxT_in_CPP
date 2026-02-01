# Editor Modules Overview

This editor is designed as a collection of small, independent modules.
Each module has a single responsibility.

No frameworks.
No hidden magic.
Pure C++.

---

## Core / Editor

Controls the main application loop.

Responsibilities:

- Initializes terminal
- Starts editor
- Runs input loop
- Coordinates rendering and scrolling
- Handles shutdown

Files:

- editor.hpp
- editor.cpp

---

## Terminal

Low-level Windows console handling.

Responsibilities:

- Cursor movement
- Screen clearing
- Console size detection
- Raw input setup

This module directly talks to Win32 API.

No editor logic here.

---

## Buffer

Holds the actual text content.

Responsibilities:

- Stores lines as vector<string>
- Insert characters
- Delete characters
- New lines
- Backspace logic

Acts like editor memory.

No rendering.

---

## Render

Draws everything on screen.

Responsibilities:

- Draw editor content
- Draw status bar
- Draw controller row
- Cursor positioning
- Full redraw

Does NOT modify buffer.

Only displays.

---

## Input

Handles keyboard input.

Responsibilities:

- Key detection
- Normal mode input
- Controller mode input
- Arrow keys
- ESC handling
- Enter handling

Routes actions to Buffer or File_Controller.

---

## Scroll

Manages viewport.

Responsibilities:

- Vertical scrolling
- Horizontal scrolling
- Keeps cursor visible

Pure math + position logic.

---

## File_Controller

Command mode logic (vim-like).

Responsibilities:

- Controller mode (:)
- Command parsing
- Error messages
- Quit protection
- Modified flag handling
- Dispatch commands to File_Engine

Examples:

:q  
:w  
:o file  

No file IO here.

---

## File_Engine

Pure file system operations.

Responsibilities:

- Load file into Buffer
- Save Buffer to disk
- Report errors

No rendering.
No input.
No UI.

Only file IO.

---

## Status Bar

Displays editor state.

Shows:

- Line number
- Column
- File name
- Modified state
- Messages

Updated during redraw.

---

## Controller State

Global controller context.

Holds:

- controller_buffer
- file_path
- modified flag
- error state
- quit flag

Acts as bridge between modules.

---

# Design Philosophy

Each module:

- Does ONE job
- Avoids cross logic
- Communicates through shared state

This makes the editor:

- debuggable
- extendable
- predictable

---

Raw C++.
No framework.
Full control.
