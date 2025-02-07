### atmega8a controller.

this is a playground for atmega8a controller. i'll be using
it to learn about microcontrollers and how to program them.

currently, you'll find code for generating sine, sqaure, sawtooth
and triangle waves using the atmega8a controller.

### dependencies.

- `gcc-avr`: the gcc compiler for avr microcontrollers. used
to compile the code to hex files that can be uploaded to the
microcontroller.
- `avr-libc`: the standard c library for avr based microcontrollers.
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

### building.

make sure you have the tools mentioned above installed like,
```bash

# install dependencies
sudo apt install gcc-avr avr-libc avrdude make bear simavr gtkwave

# compile the code to hex files that can be directly uploaded to
# the microcontroller's flash
make

# in case you want to delete the compiled files to re-compile
make clean
```

### simulating.

```bash
# run the simulator and generate voltage charge dump file
# (CTRL+C after few seconds so enough traces are gathered)
simavr -m atmega8 main.elf

# view the traces in gtkwave
gtkwave main.vcd
```

### uploading.

i'm using an arduino uno rev3 board that is running the
`Files -> Examples -> 11. Arduino ISP sketch`. make sure
your pins are connected as shown in [this schematic](./schematic/AVRburner.pdf)
before uploading the code to the atmega8a.

```bash
# erase the flash (expecting the arduino uno to be connected to /dev/ttyACM0)
avrdude -v -p atmega8 -c stk500v1 -P /dev/ttyACM0 -b 19200 -e -U lock:w:0xff:m -U hfuse:w:0b11000111:m -U lfuse:w:0b10111111:m
# write the code to the flash of atmega8 through arduino uno
avrdude -v -p atmega8 -c stk500v1 -P /dev/ttyACM0 -b 19200 -U flash:w:main.hex:i -U lock:w:0xff:m
```


### compile_commands.json.

this file is used by lsp servers to provide code completion.
```bash
# generate from make
bear -- make
```

### license.

```
   ╱|、
 (˚ˎ 。7
  |、˜〵
 じしˍ,)ノ

MIT <3
```
[LICENSE.md](LICENSE.md)
