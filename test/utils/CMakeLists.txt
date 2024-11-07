# Test link with UacUtils even if HDF5 is disabled.
add_executable(UacTestUtilsToUrx "to_urx.cpp")
target_link_libraries(
  UacTestUtilsToUrx PRIVATE Urx::Urx Urx::UrxUtils Uac::UacUtils
                            Catch2::Catch2WithMain)

foreach(TESTI UacTestUtilsToUrx)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()

  catch_discover_tests(${TESTI})
endforeach()
