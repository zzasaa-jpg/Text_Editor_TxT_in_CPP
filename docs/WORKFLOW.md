# Editor Workflow

## Startup

1. main.cpp receives argv (optional file path)
2. contrl_state.file_path is set
3. Editor_boot() runs
4. File_Engine loads file (if path exists)
5. Render system initializes
6. Input loop starts

---

## Normal Mode

- Arrow keys move cursor
- Characters modify buffer
- modified flag updated

---

## Controller Mode (:)

- ESC → exit controller
- :q → quit
- :w → save
- :o <file> → open

Controller input stored in:

contrl_state.controller_buffer

---

## Save Flow

:w  
→ File_Controller::Parse_Command  
→ File_Engine::Save  
→ modified = false  
→ status message

---

## Open Flow

:o filename  
→ File_Engine::Load  
→ Reset_view_after_load  
→ redraw

---

## Quit Protection

:q  
if modified → error  
:q! → force quit

