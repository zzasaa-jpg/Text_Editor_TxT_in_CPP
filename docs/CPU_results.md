# CPU results of Editor(TxT) performance.

The CPU results were tested **before** creating the ``` File Controller ``` and ``` File Engine ``` module.

## Tested System Info
| Component  | Component Info  |
|------------|-----------------|
| CPU(Processor) | CPU 11th Gen Intel(R) Core(TM) i3 - 1115G4 @ 3.00 GHz |
| RAM | 8 GB |
| System-Type | 64-bit operating system, x64-based processor |
| Operating system | Windows |

---

## CPU results
The results are measured in ``` % ``` percentage, where the start to end percentage is the percentage of CPU running during the test i.e: 2% and end percentage is the end of a particular task or operation i.e. 4%.
This gives CPU results of ``` 2% - 4% ``` (start - end). All test cases are tested **Offline**, Without **Internet**.

| Test Cases | Results(%) |
|------------|------------|
| Left top end (Cursor line: 1, Cursor column: 1) | 6% to 8% |
| Right bottom end (Cursor line & Cursor column: end of the file content) | 4% to 8% |
| Vertical & Horizontal Scroll (With arrow keys **not mouse** scroll) | Vertical: 4% to 13%, Horizontal: 4% to 10% |
| Remove all content (Backspace key, Tested by 25 lines) | 3% to 8% |
| Backspace & Enter key | Backspace: 3% to 8%, Enter key: 5% to 13% |
| Arrow up/down/left/right (Tested with content file not a **blank file**) | 3% to 7% |
| Half content remove in file | 3% to 12% |

All CPU results is **different to different** across Operating systems.
