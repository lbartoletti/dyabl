#!/bin/sh
printf "libutils\n"
clang-tidy10 libutils/include/*h libutils/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libutils/include
printf "libarray\n"
clang-tidy10 libarray/include/*h libarray/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libarray/include -I./libutils/include -I./librand/include
printf "liblists\n"
clang-tidy10 liblists/include/*h liblists/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./liblists/include -I./libutils/include
printf "libfpn\n"
clang-tidy10 libfpn/include/*h libfpn/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libfpn/include -I./libutils/include
printf "libgraph\n"
clang-tidy10 libgraph/include/*h libgraph/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libgraph/include -I./libutils/include -I./liblists/include
printf "libhash\n"
clang-tidy10 libhash/include/*h libhash/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libhash/include -I./libutils/include -I./liblists/include
printf "libmath\n"
clang-tidy10 libmath/include/*h libmath/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libmath/include -I./libutils/include
printf "librand\n"
clang-tidy10 librand/include/*h librand/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./librand/include -I./libutils/include
printf "libsort\n"
clang-tidy10 libsort/include/*h libsort/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libsort/include -I./libutils/include -I./libarray/include
printf "libstring\n"
clang-tidy10 libstring/include/*h libstring/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libstring/include -I./libutils/include
printf "libtree\n"
clang-tidy10 libtree/include/*h libtree/src/*c -checks="-*,clang-analyzer-*,cert-*,bugprone-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-header-filter=.*" -- -I./libtree/include -I./libutils/include -I./liblists/include
