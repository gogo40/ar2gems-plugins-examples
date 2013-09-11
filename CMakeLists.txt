cmake_minimum_required(VERSION 2.8)

project(demo-plugins)



######################################################################
# Please set the variables with your preferences. The defaults should
# work out of the box.
######################################################################

set(CMAKE_BUILD_TYPE Release) # Debug, Release, RelWithDebInfo or MinSizeRel

### AR2GEMS SETTINGS ###################################################
set(AR2GEMS_DIR "c:/code-dev/ar2gems/ar2gems")
set(AR2GEMS_BUILD_DIR "c:/code-dev/ar2gems/ar2gems-msvc11-x64")
set(AR2GEMS_RELEASE_LIB_DIR ${AR2GEMS_BUILD_DIR}/lib/Release)
set(AR2GEMS_DEBUG_LIB_DIR ${AR2GEMS_BUILD_DIR}/lib/Debug)

#Could first check is the env. variable AR2GEMS_PLUGINS_DIR is set
set(AR2GEMS_RELEASE_PLUGINS_DIR c:/code-dev/ar2gems/ar2gems-msvc11-x64/lib/Release/plugins/Geostat)
set(AR2GEMS_DEBUG_PLUGINS_DIR c:/code-dev/ar2gems/ar2gems-msvc11-x64/lib/Debug/plugins/Geostat)


### Enabling Boost ###################################################
#FIND_PACKAGE( Boost REQUIRED )
set(BOOST_ROOT "c:/code-dev/boost_1_52_0")
# Uncomment if Boost isn't in your path:
set(Boost_NO_SYSTEM_PATHS TRUE)
FIND_PACKAGE( BOOST REQUIRED )
if(Boost_FOUND)
  message(STATUS "Boost found in ${BOOST_ROOT}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()


######################################################################
############### DON'T NEED CHANGE ANYTHING HERE ######################
######################################################################

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -march=native")
  set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -ggdb -march=native")
elseif(MSVC)
  # customize flags for Visual Studio
endif()

set(BUILD_SHARED_LIBS TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


### Enabling Qt4 #####################################################
find_package(Qt4 REQUIRED COMPONENTS
             QtCore QtGui QtXml QtSvg QtDesigner QtUiTools)
if(QT4_FOUND)
  message(STATUS "Qt4 found in ${QT_INCLUDE_DIR}")
  include(${QT_USE_FILE})
endif()

### Enabling VTK #####################################################
#find_package(VTK REQUIRED)
find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersSources
  vtkRenderingCore
  vtkRenderingOpenGL
  vtkInteractionStyle
  vtkChartsCore
  vtkRenderingFreeTypeOpenGL
  vtkIOXML  
  vtkRenderingQt
  vtkGUISupportQt
  vtkViewsQt
  vtkViewsContext2D
  vtkInteractionWidgets
)
  
if(VTK_FOUND)
  message(STATUS "VTK found in ${VTK_DIR}")
  include(${VTK_USE_FILE})
endif()

### Enabling Python ##################################################
find_package(PythonLibs 2.7 REQUIRED)
if(PYTHONLIBS_FOUND)
  message(STATUS "Python found in ${PYTHON_INCLUDE_DIR}")
  include_directories(${PYTHON_INCLUDE_DIR})
endif()

### Enabling Eigen ###################################################
include_directories(${CMAKE_SOURCE_DIR})

### Enabling GsTL ####################################################
include_directories(${AR2GEMS_DIR}/ar2GsTL)

### Includes prefixed with the directory name ########################
# The convention is to prefix the subdirectory (e.g. utils/foo.h) when
# referring to headers.
include_directories(${AR2GEMS_DIR}/ar2gems)
include_directories(${AR2GEMS_BUILD_DIR}/ar2gems)


### UI files in binary directory #####################################
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if ( MSVC )
	#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AR2GEMS_DIR}/lib/x64/release/plugins/Geostat )
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${AR2GEMS_PLUGINS_DIR}/lib/x64/debug/plugins/Geostat )
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${AR2GEMS_PLUGINS_DIR}/lib/x64/release/plugins/Geostat )
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${AR2GEMS_PLUGINS_DIR}/lib/x64/debug/plugins/Geostat )
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${AR2GEMS_PLUGINS_DIR}/lib/x64/release/plugins/Geostat )	

  #  set_target_properties( ${targetname} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AR2GEMS_DIR}/lib/x64/release/plugins/geostat )
  #  set_target_properties( ${targetname} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${AR2GEMS_DIR}/lib/x64/debug/plugins/geostat )
  #  set_target_properties( ${targetname} PROPERTIES CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${AR2GEMS_DIR}/lib/x64/release/plugins/geostat )
    # etc for the other available configuration types (MinSizeRel, RelWithDebInfo)
endif ( MSVC )
### Project tree #####################################################
#set(PROJ_DIRS
#  actions appli charts filters geostat grid gui
#  main math qtplugins qwt utils widget_accessor
#)

foreach(dir ${PROJ_DIRS})
  add_subdirectory(${dir})
endforeach()

# Should have a flag to see of these library are needed or not
# such that the user could select them in cmake-gui
set(AR2GEMS_LIB_RELEASE 
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_appli.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_grid.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_math.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_widgets.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_utils.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_charts.lib
	${AR2GEMS_RELEASE_LIB_DIR}/ar2gems_geostat.lib
)
set(AR2GEMS_LIB_DEBUG 
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_appli.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_grid.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_math.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_widgets.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_utils.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_charts.lib
	${AR2GEMS_DEBUG_LIB_DIR}/ar2gems_geostat.lib
)


# Process all headers with Q_OBJECT
QT4_WRAP_CPP(plugins_demo_MOC
  pie_chart_creator.h
  pie_chart.h
)

#QT4_WRAP_UI(plugins_demo_FORMS
#  chart_display_control.ui
#)

## File to compile
add_library(plugins_demo
  add_property_action.cpp
  add_property_action.h
  multiply_property_action.cpp
  multiply_property_action.h
  pie_chart_creator.cpp
  pie_chart_creator.h
  pie_chart.cpp
  pie_chart.h
  local_high_values_algo.cpp
  local_high_values_algo.h
  library_init.cpp
  common.h
  ${plugins_demo_MOC}
#  ${plugins_demo_FORMS}
)

target_link_libraries(plugins_demo
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES}
  general ${AR2GEMS_LIB_RELEASE}
  optimized ${AR2GEMS_LIB_RELEASE}
  debug ${AR2GEMS_LIB_DEBUG}    
#  ar2gems_appli
#  ar2gems_grid
#  ar2gems_math
#  ar2gems_widgets
#  ar2gems_utils
)

source_group(Qt FILES ${plugins_demo_MOC} )



if (WIN32)
  add_definitions(-DPLUGINS_DEMO_EXPORT)
endif()
