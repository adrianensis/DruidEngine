file(GLOB TEST_SOURCES "*.cpp")

foreach(TEST_PATH ${TEST_SOURCES})

  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

  set(TEST_FINAL_NAME "test_${DRUID}_${TEST_NAME}" )

  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID} )
  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endforeach(TEST_PATH ${TEST_SOURCES})
