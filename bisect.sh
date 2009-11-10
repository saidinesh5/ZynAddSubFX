#!/bin/bash

#I used this script to determine when a certain test failed, with git bisect
#look for "git bisect run" in the git help
#it might be useful for others, so it's included here
#assumes cmake build-folder is ./build

make -C build rebuild_cache && make -C build -j5 || exit 125
build/src/Tests/ADnoteTest
