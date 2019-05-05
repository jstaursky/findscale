# findscale #

Simple gtk-based application that will display any musical scale in any key on instrument(s). I find this useful for improvising over a chord progression in some key I am unfamiliar with. Initially the only instrument will be piano, but will eventually add guitar and maybe some others.

![piano](./src/imgs/piano.png ) 

- using gtk + cairo

## To do's ##

- ~~finish scale overlaying by note. (mostly just need to finish overlays for black keys)~~ **DONE**
- ~~implement method for inputting a custom scale.~~ **DONE**

- Create widget to select different scales at a given key.
- Create widget to make key note changable.
- Clean up the closing process (not require `ctrl + c` to exit)
- Clean up `free`ing of various memory structures.
  - Valgrind analysis

## Usage ##

Currently must run the program as follows
```
./findscale piano <Key>
```
For example, running `./findscale piano Ab` will result in,

![Ab-major-scale](./examples/Ab-major-scale.png)

__findscale__ will only display the 1st scale listed in the config file found at `findscale/src/conf/scale.list` (selectable scale widget to be added later). So, here we have the major scale in the key of A flat since the scale interval pattern corresponding to the major scale is listed first in `scale.list` and our cmdline argument for the key was `Ab`.

It should also be noted that the `<Key>` passed as an argument to findscale can only be one of the following. `Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G`.

## Tools
This section is more for my own reference, but maybe someone else may find it useful too.

To generate a compilation database suitable for use with editor tools such as ccls, irony-mode, etc.

Goto the `findscale/` directory, use https://github.com/nickdiego/compiledb tool, and follow this up with https://github.com/Sarcasm/compdb.
and you will end up with a `compile_commands.json` suitable for parsing by your favorite tool.

I use `ccls` on ubuntu and have the following in my `.ccls` file:

```
%compile_commands.json
%c -std=c11
%c --gcc-toolchain=/usr
%c -I/usr/lib/gcc/x86_64-linux-gnu/7/include
```

The include line `%c -I/usr/lib/gcc/x86_64-linux-gnu/7/include` is needed for `ccls` to find some of the libc headers (I was getting errors telling me `size_t` was an invalid type).
To find the appropriate `-I` for your system if you run into such errors is
`gcc --print-file-name=include`.
