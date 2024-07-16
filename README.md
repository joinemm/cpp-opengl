![screenshot](/screenshots/2024-06-24.png)

# Install

This game can be installed by using make:

```sh
make install
```

Make will put the game files in `/usr/local/cpp-opengl` by default, and create a wrapper script to load shaders and textures.
This can be changed with the `PREFIX` environment variable.

Nix can also be used to run the project straight from the repo:

```sh
nix run github:joinemm/cpp-opengl
```

# Develop

## Dependencies

- [GLFW](https://github.com/glfw/glfw)
- [GLEW](https://github.com/nigels-com/glew)
- [stb](https://github.com/nothings/stb)
- [GLM](https://github.com/g-truc/glm)

A nix development shell is included which comes installed with the required libraries:

```sh
nix develop
```

If not using nix, you can install the dependencies system wide.
These are the packages for Ubuntu:

```sh
sudo apt install libglfw3-dev libglew-dev libstb-dev libglm-dev
```

For other distros, you'll have to find the matching packages and install them on your system.

## Building

With the dependencies installed and accessible to the compiler,
the included Makefile can be used for compiling and running the program.

```sh
# compile only
make

# compile and run
make run

# remove all generated build files
make clean
```

The compiled binary can be found at `./game`

Nix can also be used to for building:

```sh
nix build
cd result
```
