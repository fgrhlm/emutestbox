# EmuTestBox

A collection of tools to help you test and troubleshoot MOS 6502 emulators.

## What is this?

This library contains a loader for SingleStepTests 6502 test definitions and
a debugger for MOS 6502 emulators.

## Why?

This project was done as a part of a bachelors thesis from Arcada University of Applied Science.

## Building
This projects has only been designed with linux users in mind, compatibility not guaranteed
on other platforms, sorry.

### Dependencies
1. [The Meson Build System](https://mesonbuild.com/)
2. [Clang](https://clangd.llvm.org/)
1. [jansson](https://github.com/akheron/jansson)
2. [cmocka](https://cmocka.org/)

Build from source or use your favourite package manager.

### Get to the source
Clone this repo and its submodules: `git clone --recursive https://github.com/fgrhlm/emutestbox`

### Build reference emulators
This is optional and can be skipped by commenting out the relevant lines in `meson.build`).

Navigate to `tests/f6502` and follow the instructions contained in `README.md`.
Navigate to `tests/6502-emu` and follow the instructions contained in `README.md`.

### Build emutestbox
Navigate to the emutestbox root and run the following commands:

Setup: `meson setup build`
Build: `meson compile -C build`

## Testing
Testing done with [CMocka](https://cmocka.org/).

Each module has its own test definitions in the `tests` directory. When building the project, individual test binaries will
be created in the `build` directory.

Please go ahead and navigate to the `build` directory and run each of the binaries prefixed `test_`.

# Usage

Include the emutestbox api module:

```c
#include "api.h"
```

Declare a new emutestbox 6502 emulator container:

```c
etb_emu_6502 emu;
```

"Hook" the dummy emu to your emulators registers and memory:

```c
etb_emu_6502_hook_pc(&emu, &REGISTER_PC);
etb_emu_6502_hook_s(&emu, &REGISTER_SP);
etb_emu_6502_hook_a(&emu, &REGISTER_A);
etb_emu_6502_hook_x(&emu, &REGISTER_X);
etb_emu_6502_hook_y(&emu, &REGISTER_Y);
etb_emu_6502_hook_p(&emu, &REGISTER_P);
etb_emu_6502_hook_ram(&emu, &RAM);
```

Point to a function that steps the emulation forward ( Define one if you have to :) ):


```c
etb_emu_6502_hook_step(&emu, (void*)step_cpu_wrapper);
````

Run the test by supplying a path to a test definition:

```c
char* path_to_json_file = "test.json"

etb_run_6502_test(&emu, t_path, 1);

```

See `tests/ref_1.c` and `tests/ref_2.c` for more examples.

Documentation for specific modules and functions are embedded in the corresponding source file :).
