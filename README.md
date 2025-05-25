# README

## Description

This is a basic TCP server built with plain C with socket programming. This program also offers concurrency by handling multiple clients at the same time with the thread-per-client model. This is a very simple, easy and lightweight server.

## Prerequisites

- GCC — C compiler for compiling the program from source
- make — A build automation tool to make our life easier

## Building

Building the project is pretty easy to do, thanks to the Makefile. To build the program, run the command from your terminal:

```bash
$ make
```

And let it automatically compile the program.

## Usage

The compiled binary would be available in `bin/` directory. Simply execute the binary file with an optional argument of port number. In case of invalid or no port number, a default port of `4545` would be used for the server.

```bash
$ ./bin/server <port>
```

## Cleanup

To clean up the binaries and object files, you're just one command away. From the root project directory, run the following command to automatically clean up the object files and binaries.

```bash
$ make clean
```

## License

This project uses the MIT license, so feel free to use the source code as you wish. Use it, modify it, distribute it, whatever you wish.

## Contribution

This server is in one of the earliest stages and contributions are always welcome for the project. Feel free to make pull requests and even if you can't contribute by actual code, you can always audit the codebase and review it, create issues or give suggestions.