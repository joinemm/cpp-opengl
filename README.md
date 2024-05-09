## Development

A nix development shell is included which installs the required libraries into the environment.

```sh
nix develop --impure -c $SHELL
```

## Compile and run

With the dependencies installed and accessible to the compiler, 
the included Makefile can be used for compiling and running the program.

```sh
make build
make run
```

Or to compile and run the result immediately afterwards:

```sh
make
```
