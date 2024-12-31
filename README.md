### atmega8a controller.

this is a playground for atmega8a controller. i'll be using
it to learn about microcontrollers and how to program them.

### tools used.

- `gcc-avr`: the gcc compiler for avr microcontrollers. it's
  is what builds the elf and hex files for the microcontroller.
- `avr-libc`: the standard c library for avr microcontrollers.
- `avrdude`: the program used to upload code to the microcontroller.
- `make`: the build automation tool.
- `bear`: the tool used to generate the compile_commands.json file.
it's used by lsp servers to provide code completion and other features.

> you can also achieve completion and other features by using
`.clangd` file in the root. just make sure your CFLAGS match
between makefile and `.clangd` file. i just found bear to be
more convenient.

### how to build.

make sure you have the tools mentioned above installed like,
```bash
sudo apt install gcc-avr avr-libc avrdude make bear
```

then you can generate the `.elf` and `.hex` files (build the
project) by running,
```bash
make
```

to delete the generated files run,
```bash
make clean
```
