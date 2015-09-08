CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# install edj
ADD_CUSTOM_COMMAND(OUTPUT crash-viewer.edj
		COMMAND edje_cc -id ${CMAKE_CURRENT_SOURCE_DIR}/images/private
		${CMAKE_CURRENT_SOURCE_DIR}/crash-viewer.edc ${CMAKE_BINARY_DIR}/crash-viewer.edj
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crash-viewer.edc
)
ADD_CUSTOM_TARGET(edj_build DEPENDS crash-viewer.edj)
ADD_DEPENDENCIES(${PROJECT_NAME} edj_build)
INSTALL(FILES ${CMAKE_BINARY_DIR}/crash-viewer.edj DESTINATION ${EDJDIR})
