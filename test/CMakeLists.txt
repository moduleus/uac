include(Catch)

add_subdirectory(uac)

add_subdirectory(io)

add_subdirectory(utils)

if(WITH_PYTHON)
  add_subdirectory(python)
endif()
