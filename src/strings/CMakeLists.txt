add_library(strings_lib
    strops.cc
    strarray.cc
)
target_compile_options(strings_lib PRIVATE -O3)