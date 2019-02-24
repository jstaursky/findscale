# findscale #

Simple gtk-based application that will display any musical scale in any key on instrument(s). I find this useful for improvising over a chord progression in some key I am unfamiliar with. Initially the only instrument will be piano, but will eventually add guitar and maybe some others.

![piano](./src/imgs/piano.png ) 


- using gtk + cairo

## TODO's ##

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
so for example, running `./findscale piano B` will result in,

![Ab-major-scale](./examples/Ab-major-scale.png)

Currently, findscale will only display the 1st scale listed in the config file found in `findscale/src/conf/scale.list`. It is the next item on the TODO list to implement a widget to change this so any scale listed in `scale.list` can be selected individually and the graphic will be updated accordingly.
