MP-IDS ACU Framework
====================

### Build

The project is intended to be developed with [CLion][0]. As it shall also be
possible to build the project without an IDE (i.e. on the command-line), we
added the [`build.sh`](build.sh) script. Call `./build.sh -h` for more
information on how to use it.

[0]: https://www.jetbrains.com/clion/

### Code-Structure


```
├── build.sh                    # one-line build tool
├── cmake                       # additional cmake files
│   └── FindBroker.cmake
├── CMakeLists.txt              # the cmake build file
├── include                     # headers
│   └── acu
│       └── *.h
├── README.md
├── src                         # sources
│   └── *.cc
└── test                        # test cases
    └── CMakeLists.txt
```
