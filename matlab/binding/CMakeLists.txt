set(MATLAB_CPP_PRIVATE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
set(MATLAB_CPP_PUBLIC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/uac/matlab/bindings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uac/matlab/bindings_decl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uac/matlab/bindings_impl.h)

add_library(UacMatlabBinding SHARED)
add_library(uac::UacMatlabBinding ALIAS UacMatlabBinding)

if(UNIX)
  target_link_options(UacMatlabBinding PRIVATE "-static-libgcc"
                      "-static-libstdc++")
endif(UNIX)

target_sources(
  UacMatlabBinding
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} FILES
         ${MATLAB_CPP_PUBLIC_SRCS}
  PRIVATE ${MATLAB_CPP_PRIVATE_SRCS})

target_include_directories(
  UacMatlabBinding
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)

target_link_libraries(
  UacMatlabBinding
  PRIVATE Uac::Uac "$<LINK_LIBRARY:WHOLE_ARCHIVE,Urx::UrxMatlab>"
          Urx::UrxCompilerFlags)
target_compile_definitions(UacMatlabBinding PRIVATE "UrxMatlabBinding_EXPORTS")

target_link_libraries(UacMatlabBinding PRIVATE Uac::UacUtils)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET UacMatlabBinding
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_RUNTIME_DLLS:UacMatlabBinding>
      $<TARGET_FILE_DIR:UacMatlabBinding>
    COMMAND_EXPAND_LISTS)
endif()

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(
  TARGETS UacMatlabBinding
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Generate the MATLAB toolbox
configure_file(windows.prj.in windows.prj @ONLY)
file(
  GENERATE
  OUTPUT "$<TARGET_FILE_DIR:UacMatlabBinding>/windows-$<CONFIG>.prj"
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/windows.prj")
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/windows.prj")

configure_file(linux.prj.in linux.prj @ONLY)
file(
  GENERATE
  OUTPUT "$<TARGET_FILE_DIR:UacMatlabBinding>/linux-$<CONFIG>.prj"
  INPUT "${CMAKE_CURRENT_BINARY_DIR}/linux.prj")
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/linux.prj")

add_custom_command(
  TARGET UacMatlabBinding
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../+uac $<TARGET_FILE_DIR:UacMatlabBinding>/+uac
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/uac
    $<TARGET_FILE_DIR:UacMatlabBinding>/include/uac
  COMMAND
    ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../../uac
    $<TARGET_FILE_DIR:UacMatlabBinding>/include/uac
  COMMAND_EXPAND_LISTS)
