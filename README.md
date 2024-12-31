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
- `simavr`: the simulator for avr microcontrollers. it's used to
simulate the code and generate traces for debugging before uploading
the code to the microcontroller.

> you can also achieve completion and other features by using
`.clangd` file in the root. just make sure your CFLAGS match
between makefile and `.clangd` file. i just found bear to be
more convenient.

### how to build.

make sure you have the tools mentioned above installed like,
```bash
sudo apt install gcc-avr avr-libc avrdude make bear simavr
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

### simulating the code.

you can simulate the code using `simavr` by running,
```bash
simavr -m atmega8 main.elf
```
> we don't use atmega8a because simavr doesn't support it
but atmega8 is exactly the same as atmega8a except for the
power consumption.

this will generate `debug_trace.vcd` (exit after
some seconds). now you can load `debug_trace.vcd`
in gtkwave to see the traces.


### making compile_commands.json.

you can generate the `compile_commands.json` file by running,
```bash
bear -- make
```
this will generate the `compile_commands.json` file which
can be used by lsp servers to provide code completion and
other features.


### license.

```
   ╱|、
 (˚ˎ 。7
  |、˜〵
 じしˍ,)ノ

MIT <3
```
[LICENSE.md](LICENSE.md)
