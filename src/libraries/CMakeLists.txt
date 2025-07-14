if(WITH_QUANTLIB)
  add_library(lib_lib STATIC
    quantlib.cc
  )
  target_link_libraries(lib_lib
    PRIVATE 
      ql_library
  )
endif()