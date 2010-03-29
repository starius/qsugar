# Copyright 2010, Juri Syrowiecki
# 
# This file is part of QSugar.
# 
# QSugar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# QSugar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QSugar.  If not, see <http://www.gnu.org/licenses/>.


################################################################################
## Libraries ###################################################################

find_package( Qt4
    COMPONENTS
        QtCore
        QtGui
        QtXml
    REQUIRED
)
include( ${QT_USE_FILE} )

include_directories( ../include )


################################################################################
## Source ######################################################################

set( SOURCES
    QSugar-test.cpp
)

set( HEADERS
)


################################################################################
## Qt wrappers #################################################################

qt4_wrap_cpp( HEADERS_COMPILED ${HEADERS} )


################################################################################
## Targets #####################################################################

add_executable( test EXCLUDE_FROM_ALL
    ${SOURCES}
)

target_link_libraries( test
    ${QT_LIBRARIES}
    QSugar
)


################################################################################
## Standards compliance ########################################################

set_target_properties(
    test
PROPERTIES
    COMPILE_FLAGS "-Wall -Wextra"
)
