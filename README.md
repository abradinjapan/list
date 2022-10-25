# LIST

A program to print a tree of directories in linux.

## Usage

> list

Caveats:
- Always prints the directory structure of where it was called.
- Shows all files and folders (hidden or not) always.
- Directories and files that require permission are NOT PRINTED [NEEDS FIX].
- Always shows file size in bytes.
- Does not follow symbolic links.
- No options or save files of any kind; removing the executable removes the everything.

## Build

All you need is the C standard library and linux headers to compile.

There is only one C file in the whole project, headers are organized at the top.

> make

OR

> make debug
