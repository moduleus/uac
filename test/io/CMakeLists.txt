if(WITH_HDF5)
  set(SRCS_FILES writer_test.cpp)

  add_executable(UacTestIo ${SRCS_FILES})

  target_link_libraries(
    UacTestIo PRIVATE Uac::Uac Uac::UacUtils ${HDF5_CXX_LIBRARIES}
                      Catch2::Catch2WithMain Urx::UrxCompilerFlags)

  if(MSVC)
    target_compile_options(UacTestIo PRIVATE /utf-8)
  endif()

  foreach(TESTI UacTestIo)
    if(WIN32 AND BUILD_SHARED_LIBS)
      add_custom_command(
        TARGET ${TESTI}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
        COMMAND_EXPAND_LISTS)
    endif()
  endforeach()

  catch_discover_tests(UacTestIo)
endif()

# Test link with UacUtils even if HDF5 is disabled.
add_executable(UacTestIoUtils "utils.cpp")
target_link_libraries(UacTestIoUtils PRIVATE Uac::UacUtils)
