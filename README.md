## Development

A nix development shell is included which installs the required libraries into the environment.

```sh
nix develop --impure -c $SHELL
```

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
