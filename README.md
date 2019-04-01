[1]: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

# Conway's Game of Life

![life](https://upload.wikimedia.org/wikipedia/commons/e/e5/Gospers_glider_gun.gif)

This is a simple implementation of [John Conway's Game of Life][1]. The Game of Life consists of an infinite grid of
square cells, each of which is in one of two possible states, _alive_ or _dead_. In this implementation, the infinite
grid is simulated by making a finite grid a torus. Every cell interacts with its eight neighbours. At each step in
time, the following transitions occur:

1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

An initial pattern is provided to seed the system.

## Usage

First, `life.cpp` need to be compiled with your favorite compiler to produce an executable. The executable can then
be run as follows:

`./life <filename> <iterations> <verbosity>`

where `<filename>` is the path to a configuration file, `<iterations>` is the number of steps to run the simulation,
and `<verbosity>` is 0 (no output), 1 (only show final state), or 2 (display animation).

A configuration file is required to instantiate the world. The configuration file is provided as the first
command-line argument, and should be formatted as follows:

* The first line specifies the grid dimensions: `<rows> <cols>`
* The second line lists the number of coordinate pairs that will follow: `<n>`
* The remaining n lines specify which grid cells should be initialized alive: `<row> <col>`

Example configuration files can be found in [`config/`](config).