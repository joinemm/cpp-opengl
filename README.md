## Development

A nix development shell is included which installs GLFW and GLEW.

```sh
nix develop --impure -c $SHELL
```

## Compile and run

With the needed libraries installed in your system:

```sh
g++ main.cpp -lGL -lglfw -lGLEW

./a.out
```
