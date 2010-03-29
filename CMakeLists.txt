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


cmake_minimum_required( VERSION 2.6 )

set( PROJECT QSugar )

project( ${PROJECT} )


################################################################################
## Components ##################################################################

add_subdirectory( src )
add_subdirectory( tests )
