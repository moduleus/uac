set(UAC_TEST_UTILS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/to_urx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/validator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clone_generic_test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/clone_generic_test.cpp)

# Test link with UacUtils even if HDF5 is disabled.
add_executable(UacTestUtils ${UAC_TEST_UTILS_FILES})
target_link_libraries(UacTestUtils PRIVATE Urx::Urx Urx::UrxUtils Uac::UacUtils
                                           Catch2::Catch2WithMain)

foreach(TESTI UacTestUtils)
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
