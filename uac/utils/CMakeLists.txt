include(GenerateExportHeader)

set(UAC_UTILS_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/clone.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/serialize_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/to_urx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/validator.cpp)
set(UAC_UTILS_MISC ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
set(UAC_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/clone.h
    ${CMAKE_CURRENT_SOURCE_DIR}/serialize_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/to_urx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/validator.h)

if(WITH_HDF5)
  list(APPEND UAC_UTILS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/io/enums.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.cpp
       ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.cpp)
  list(
    APPEND
    UAC_UTILS_H
    ${CMAKE_CURRENT_SOURCE_DIR}/test/dataset_gen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/io/writer.h)
endif()

set(UAC_UTILS_LIBRARY UacUtils)
add_library(${UAC_UTILS_LIBRARY})
add_library(Uac::${UAC_UTILS_LIBRARY} ALIAS ${UAC_UTILS_LIBRARY})

generate_export_header(
  ${UAC_UTILS_LIBRARY} BASE_NAME UAC_UTILS EXPORT_FILE_NAME
  ${CMAKE_CURRENT_BINARY_DIR}/uac/utils/export.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/uac/utils/export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}/uac/utils/)

target_include_directories(
  ${UAC_UTILS_LIBRARY}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME}>)
set_target_properties(
  ${UAC_UTILS_LIBRARY}
  PROPERTIES OUTPUT_NAME "${UAC_UTILS_LIBRARY}-${git_branch}" DEBUG_POSTFIX "d")

target_sources(
  ${UAC_UTILS_LIBRARY}
  INTERFACE FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../.. FILES
            ${UAC_UTILS_H}
  PRIVATE ${UAC_UTILS_SRCS} ${UAC_UTILS_MISC})
if(UAC_UTILS_SRCS STREQUAL "" AND UAC_UTILS_H STREQUAL "")
  set_target_properties(${UAC_UTILS_LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
endif()

set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${INSTALL_NAME})

target_link_libraries(
  ${UAC_UTILS_LIBRARY}
  PUBLIC Uac::Uac Urx::UrxUtils
  PRIVATE Urx::UrxCompilerFlags)
if(WITH_HDF5)
  target_link_libraries(
    ${UAC_UTILS_LIBRARY} PRIVATE hdf5::hdf5-${HDF5_TYPE}
                                 hdf5::hdf5_cpp-${HDF5_TYPE})
endif()

if(NOT CALL_FROM_SETUP_PY)
  install(
    TARGETS ${UAC_UTILS_LIBRARY}
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
