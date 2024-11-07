find_file(URX_PYTHON_TEST_DIRECTORY test_bindings_acquisition.py
          PATHS "${URX_PYTHON_TEST_PATH}" "${URX_SRC}/test/python"
                "${CMAKE_CURRENT_SOURCE_DIR}/../../urx/test/python")
get_filename_component(URX_PYTHON_TEST_DIRECTORY "${URX_PYTHON_TEST_DIRECTORY}"
                       PATH)

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/urx_python_path.txt
     "${URX_PYTHON_TEST_DIRECTORY}")
