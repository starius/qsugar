cmake_minimum_required( VERSION 2.6 )

set( PROJECT QSugar )

project( ${PROJECT} )


################################################################################
## Libraries ###################################################################

find_package( Qt4 REQUIRED )
#set( QT_DONT_USE_QTGUI TRUE )
set( QT_USE_QTXML TRUE )
include( ${QT_USE_FILE} )


################################################################################
## Source ######################################################################

set( SOURCES
    QSugar.cpp
)

set( HEADERS
    #QSugar.hpp
)


################################################################################
## Targets #####################################################################

qt4_wrap_cpp( HEADERS_COMPILED
    ${HEADERS}
)

add_library( ${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS_COMPILED}
)

target_link_libraries( ${PROJECT}
    ${QT_LIBRARIES}
)


################################################################################
## Tests #######################################################################

set( TEST_SOURCES
    QSugar-test.cpp
)

add_executable( test EXCLUDE_FROM_ALL
    ${TEST_SOURCES}
)

target_link_libraries( test
    ${QT_LIBRARIES}
    ${PROJECT}
)


################################################################################
## Standards compliance ########################################################

set_target_properties(
    ${PROJECT}
    test
PROPERTIES
    COMPILE_FLAGS "-Wall -Wextra"
)
