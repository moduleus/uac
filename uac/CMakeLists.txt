configure_file(uac.h.in uac.h @ONLY)
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/uac.h")

set(UAC_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/compare.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acquisition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dataset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/destination_link.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element_geometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/hw_config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/igroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/impulse_response.h
    ${CMAKE_CURRENT_SOURCE_DIR}/probe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/receive_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/super_group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/transmit_setup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/trigger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wave.h)

add_library(${PROJECT_NAME} INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME} INTERFACE FILE_SET HEADERS BASE_DIRS
                            ${CMAKE_CURRENT_SOURCE_DIR}/.. FILES ${UAC_SRCS})
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/uac.h")

target_link_libraries(${PROJECT_NAME} INTERFACE Urx::Urx)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}-${git_branch}"
                             DEBUG_POSTFIX "d")

add_subdirectory("utils")

# Install library ###

configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}/${PROJECT_NAME}-${git_branch})

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/uac.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/uac")

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${INSTALL_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${git_branch})
