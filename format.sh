#!/bin/bash
find ./src -iname *.hpp -o -iname *.cpp | xargs clang-format-10 -i --style=WebKit