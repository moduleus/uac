if(BUILD_TESTING)
  include(Catch)

  add_subdirectory(uac)

  add_subdirectory(io)

  add_subdirectory(utils)
endif()

if(WITH_PYTHON)
  add_subdirectory(python)
endif()
