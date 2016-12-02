#!/usr/bin/env sh
# -*- coding: utf-8 -*-
#
# :: build.sh
# Script to build the project.
# Could be replaced by a Makefile. But that does
# not work as expected, so here is the discussed
# script.

_BUILD_DIR=build/
_CMAKE_OPTS="-DCMAKE_BUILD_TYPE=Debug"
_TARGETS="configure build test clean"

function target() { echo -e "\033[33;1m[TARGET]\033[m $@"; }
function error() { echo -e "\033[31;1m[ERR]\033[m $@" >&2; }
function error_exit() { error "$@"; exit 1; }

function array_has() { for var in ${@:2}; do if [ "$var" == "$1" ]; then return 0; fi; done; return 1; }

function usage() {
    echo -e "Usage $0 [-h] <targets>

Options:
    -h          Print this help message and exit.

Targets:
    configure   Setups everything to be built.
    build       Builds the project (configure will be called).
    test        Runs the tests.
    clean       Cleans the repo by removing the build-directory."
    exit;
}

function parseargs() {
    # help message
    case $1 in
        -h|--help|help)
            usage ;;
        -*)
            error_exit "Invalid argument $1" ;;
    esac

    # fallback
    if [ $# -eq 0 ]; then
        b_build
    fi

    # iterating over arguments
    for arg in "$@"; do
        array_has $arg $_TARGETS || error_exit "Invalid target $arg"
        b_$arg
    done
}

function b_configure() {
    target "configure"
    mkdir -p $_BUILD_DIR
    cd $_BUILD_DIR
    cmake $_CMAKE_OPTS ..
    cd ..
}

function b_build() {
    b_configure
    target "build"
    cd $_BUILD_DIR
    make
    cd ..
}

function b_test() {
    target "test"
    cd $_BUILD_DIR
    make test
    cd ..
}

function b_clean() {
    target "clean"
    rm -rf $_BUILD_DIR
}

parseargs "$@"
