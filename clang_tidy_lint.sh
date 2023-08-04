#!/bin/sh
clang-tidy build/compile_commands.json -checks=-*,clang-analyzer-*,cppcoreguidelines-*
