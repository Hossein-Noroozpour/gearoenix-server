#!/bin/bash
find ./tools -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format-10 -i -style=WebKit {} \;