# u0date

A terminal-based text editor built using the ncurses library in C. u0date provides fundamental text editing capabilities, including a robust undo and redo system implemented using the **Command design pattern**.

## Features

u0date currently supports the following features:

* **File Reading:** Load content from a specified file upon startup.
* **File Writing:** Save the current buffer content back to the file.
* **Text Insertion:** Add new characters at the cursor position and insert new lines (via the Enter key).
* **Text Deletion:** Remove characters (via Backspace) and join lines.
* **Cursor Navigation:** Move the cursor through the text using arrow keys (Up, Down, Left, Right).
* **Editing Modes:** Switch between Normal (for navigation and commands) and Insert Mode (for typing and text entry).
* **Undo Functionality:** Reverse the last performed change (`u`).
* **Redo Functionality:** Re-apply the last undone change (`Ctrl + R`).
* **Basic Status Display:** A status line at the bottom shows the current editing mode (on the left) and messages like "Saved!", "Undo!", "Redo!" (on the right).
* **Bounds Checking:** Prevents the cursor from moving or operations from writing outside the valid text boundaries within the buffer structure.

## Prerequisites

To build and run u0date, you need:

* A C compiler (like GCC or Clang).
* The ncurses library and its development headers.

You can typically install ncurses development headers using your system's package manager:

* **Debian/Ubuntu:** `sudo apt-get install libncurses-dev`
* **Fedora/RHEL:** `sudo dnf install ncurses-devel`
* **macOS (using Homebrew):** `brew install ncurses`

## Getting Started

First, clone the repository to your local machine:

```bash
git clone https://github.com/3akare/u0date.git
```

Navigate into the cloned repository directory:

```bash
cd u0date
```

Now, build the project using `make`:

```bash
make
```

This will compile the source files and create the executable `u0date` inside the `build/` directory.

## Usage

You can run u0date in a couple of ways after building:

1.  **Directly using the compiled executable:**
    ```bash
    ./build/u0date <name_of_the_file>
    ```
    Replace `<name_of_the_file>` with the path to the file you want to edit. This command requires a filename argument. If the file does not exist, it will be created upon saving.

2.  **Using the build script:**
    ```bash
    ./build.sh <name_of_the_file> [<-flag>]
    ```
    Replace `<name_of_the_file>` with the path to the file you want to edit.
    The optional `[<-flag>]` can be any additional argument (the specific content of the flag doesn't matter; its presence simply indicates a behavior flag to the script), which according to the script's logic, means the file should be deleted upon application exit. Consult the `build.sh` script for its exact implementation details.
    If `<name_of_the_file>` is **not specified** when running `./build.sh`, the script will typically create a default file (like `file.txt`) for editing.

### Key Bindings

* **Normal Mode:**
    * `h`, `j`, `k`, `l` or **Arrow Keys**: Move cursor Left, Down, Up, Right.
    * `i`: Enter **Insert Mode**.
    * `Ctrl + S`: Save the file.
    * `u`: **Undo** the last action.
    * `Ctrl + R`: **Redo** the last undone action.
    * `q`: Quit the editor.
* **Insert Mode:**
    * **Typing any character**: Inserts the character at the cursor position.
    * **Enter**: Splits the current line at the cursor position, creating a new line below.
    * **Backspace**: Deletes the character before the cursor. If at the beginning of a line (and not the first line), joins the current line with the previous one.
    * `Esc`: Exit **Insert Mode** and return to **Normal Mode**.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICEN∏SE.md) file for details.