cmake_minimum_required(VERSION 3.20)

project(tutorials)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

if(NOT TARGET Uac::Uac)
  find_package(Uac REQUIRED)
endif()

if(NOT MSVC)
  add_link_options("-Wl,--disable-new-dtags")
endif()

add_executable(t1Installation t1_installation.cpp)
target_link_libraries(t1Installation PRIVATE Uac::Uac)

add_executable(t2OpenSave t2_open_save.cpp)
target_link_libraries(t2OpenSave PRIVATE Uac::UacUtils)
target_compile_definitions(
  t2OpenSave PRIVATE "TUTORIAL_SOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

add_executable(t3RfData t3_rf_data.cpp)
target_link_libraries(t3RfData PRIVATE Uac::UacUtils)
target_compile_definitions(
  t3RfData PRIVATE "TUTORIAL_SOURCE_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

add_executable(t4Probe t4_probe.cpp helpers.cpp)
target_link_libraries(t4Probe PRIVATE Uac::UacUtils)

add_executable(t5TransmitSetup t5_transmit_setup.cpp helpers.cpp)
target_link_libraries(t5TransmitSetup PRIVATE Uac::UacUtils)

add_executable(t6ReceiveSetup t6_receive_setup.cpp helpers.cpp)
target_link_libraries(t6ReceiveSetup PRIVATE Uac::UacUtils)

add_executable(t7Event t7_event.cpp helpers.cpp)
target_link_libraries(t7Event PRIVATE Uac::UacUtils)

add_executable(t8Acquisition t8_acquisition.cpp helpers.cpp)
target_link_libraries(t8Acquisition PRIVATE Uac::UacUtils)

add_executable(t9Stream t9_stream.cpp helpers.cpp)
target_link_libraries(t9Stream PRIVATE Uac::UacUtils)

enable_testing()

add_test(NAME t1InstallationTest COMMAND $<TARGET_FILE:t1Installation>)
add_test(NAME t2OpenSaveTest COMMAND $<TARGET_FILE:t2OpenSave>)
add_test(NAME t3RfDataTest COMMAND $<TARGET_FILE:t3RfData>)
add_test(NAME t4ProbeTest COMMAND $<TARGET_FILE:t4Probe>)
add_test(NAME t5TransmitSetupTest COMMAND $<TARGET_FILE:t5TransmitSetup>)
add_test(NAME t6ReceiveSetupTest COMMAND $<TARGET_FILE:t6ReceiveSetup>)
add_test(NAME t7EventTest COMMAND $<TARGET_FILE:t7Event>)
add_test(NAME t8AcquisitionTest COMMAND $<TARGET_FILE:t8Acquisition>)
add_test(NAME t9StreamTest COMMAND $<TARGET_FILE:t9Stream>)

foreach(
  TESTI
  t2OpenSave
  t3RfData
  t4Probe
  t5TransmitSetup
  t6ReceiveSetup
  t7Event
  t8Acquisition
  t9Stream)
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${TESTI}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${TESTI}> $<TARGET_FILE_DIR:${TESTI}>
      COMMAND_EXPAND_LISTS)
  endif()
endforeach()
