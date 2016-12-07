MP-IDS ACU Framework
====================

### Build

The project is intended to be developed with [CLion][0]. As it shall also be
possible to build the project without an IDE (i.e. on the command-line), we
added the [`Makefile`](Makefile). Call `make` to build the project inside the
`build/` directory.

[0]: https://www.jetbrains.com/clion/

### Code-Structure


```
├── Makefile                    # Makefile for 'IDE-less' compilation
│
├── cmake                       # additional cmake files
│   └── FindBroker.cmake
├── CMakeLists.txt              # the cmake build file
│
├── include                     # headers
│   └── acu
│       └── *.h
├── README.md
├── src                         # sources
│   ├── *.cc
│   └── CMakeLists.txt
└── test                        # test cases
    ├── *.cc
    └── CMakeLists.txt
```
