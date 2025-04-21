# Sandpile Simulation

A simplified C++ implementation of the sandpile model with BMP visualization, developed as a console application. This project simulates the behavior of a sandpile according to simple toppling rules and exports the field state as BMP images.

---

## Features

- Sandpile simulation based on a dynamic grid.
- Input format: `.tsv` file with initial cell positions and sand counts.
- Output: `.bmp` images showing the state of the field.
- BMP generation fully implemented manually (no third-party libraries).
- Dynamic grid resizing based on sand overflow.
- Fast iteration until either a stable state is reached or max iteration count is hit.
- Image saving frequency is configurable.

---

## Command Line Arguments

| Option           | Description                                              |
|------------------|----------------------------------------------------------|
| `-i`, `--input`   | Path to input `.tsv` file with initial sandpile data     |
| `-o`, `--output`  | Output directory where BMP images will be saved         |
| `-m`, `--max-iter`| Maximum number of iterations to simulate                |
| `-f`, `--freq`    | Frequency of BMP saving (0 means only final image saved)|

---

## Input Format (TSV)

Each line of the input file describes a single cell:

```text
x<tab>y<tab>sand_count
```

Example:
```text
0	0	5
1	0	1
2	2	3
```

---

## BMP Visualization Rules

Each cell is visualized as one pixel. Color depends on sand count:

| Sand Count | Color   |
|------------|----------|
| 0          | White    |
| 1          | Green    |
| 2          | Purple   |
| 3          | Yellow   |
| >3         | Black    |

> Pixel encoding is compact (no more than 4 bits per pixel).

---

## How it Works

1. Parses `.tsv` input into initial grid.
2. Simulates toppling rules until stable state or max iteration is reached.
3. Dynamically expands grid if sand falls off the edge.
4. Saves images with given frequency to visualize sand behavior over time.

---

## Build Instructions

This project is built using **CMake**:

```bash
mkdir build && cd build
cmake ..
make
```

Run the executable with desired flags:
```bash
./sandpile_sim -i input.tsv -o ./frames -m 1000 -f 10
```

---

## Author

This project was created as part of Lab #3 for the course on data exchange with external devices at ITMO University, Faculty of IT and Programming (FITIP).

---

## License
MIT License

