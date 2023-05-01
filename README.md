# Visuals 

Basic library for rendering in c. 

## Getting Started

For creating a render check the main file or the examples folder.

```bash
# building
make clean && make main 
# run the executable
./main 
# run the executable using .png as output
./main png
```

### Additional Information 

- [Fast, High-Quality Line Antialiasing by Prefiltering with an Optimal Cubic
Polynomial](https://algo2.iti.kit.edu/wassenberg/LineAA/LineAA.pdf)
- [Bresenham's line algorithm](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
- [Alpha Compositing](https://en.wikipedia.org/wiki/Alpha_compositing)


### Implementations

In the `gol` directory, is a implementation of [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
using [SDL](https://www.libsdl.org/).
