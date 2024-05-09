## Dependencies

- [GLFW](https://github.com/glfw/glfw)
- [GLEW](https://github.com/nigels-com/glew)
- [stb](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)

A nix development shell is included which installs the required libraries into the environment.

```sh
nix develop --impure -c $SHELL
```

If not using nix, find the corresponding packages for your distro and install them on your system.

## Compile and run

With the dependencies installed and accessible to the compiler,
the included Makefile can be used for compiling and running the program.

```sh
# compile only
make

# compile and run
make run

# clean up any compiled files
make clean
```
