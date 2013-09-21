cmake_minimum_required(VERSION 2.8)

project(demo-plugins)

######################################################################
# Please set the variables with your preferences. The defaults should
# work out of the box.
######################################################################

set(ALLOW_BUILD_PACKAGE 1)
set(BOOST_ROOT "E:/workspace/ar2externals/Boost/win64/release-V1.49.0-VC2010")
set(VTK_DIR "D:/workspace/VTK-master/build")

# SET RELEVANT VARS FOR PACKAGE
if (ALLOW_BUILD_PACKAGE)
	if (APPLE)
	elseif (UNIX)
	    SET(PACKAGE_TYPE "DEB")    # DEB or RPM
	endif (APPLE)
	
	SET(PACKAGE_MAINTAINER "Alex Santo <asanto@stereoofusion.com>")
	#SET(PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/about.txt")
	#SET(PACKAGE_LICENSE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE-ar2tech.txt")
	    
	SET(PACKAGE_VERSION_MAJOR "1")
	SET(PACKAGE_VERSION_MINOR "0")
	SET(PACKAGE_VERSION_PATCH "0")
	    
	SET(PACKAGE_SYSTEM_NAME "amd64")
endif (ALLOW_BUILD_PACKAGE)

if (APPLE)
     # ...
elseif (UNIX)
    set(INSTALL_LIB_DIR "/usr/lib")
elseif (WIN32)
    SET(INSTALL_LIB_DIR "../../../../bin") # don't set ABS path it will fail at stage of package creation (ABS path forbiden)
    SET(PACKAGEL_LIB_DIR "../../../bin") # don't set ABS path it will fail at stage of package creation (ABS path forbiden)
endif (APPLE)


set(CMAKE_BUILD_TYPE Release) # Debug, Release, RelWithDebInfo or MinSizeRel
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(AR2GEMS_PATH "c:/SGeMS-ar2tech-x64") # if not set then search in registry is enabled
find_package(AR2GEMS REQUIRED)
FindAR2GEMS()
if (AR2GEMS_FOUND)
	include_directories(${AR2GEMS_INCLUDE_DIRS}/ar2gems)
	include_directories(${AR2GEMS_INCLUDE_DIRS}/ar2GsTL)
endif (AR2GEMS_FOUND)
  
include_directories()
     


### Enabling Boost ###################################################
FIND_PACKAGE(Boost REQUIRED )
# Uncomment if Boost isn't in your path:
set(Boost_NO_SYSTEM_PATHS TRUE)
FIND_PACKAGE( Boost REQUIRED )
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

### Enabling Qt4 #####################################################
find_package(Qt4 REQUIRED COMPONENTS
             QtCore QtGui QtXml QtSvg QtDesigner QtUiTools)
if(QT4_FOUND)
  message(STATUS "Qt4 found in ${QT_INCLUDE_DIR}")
  include(${QT_USE_FILE})
endif()

### Enabling VTK #####################################################
#find_package(VTK REQUIRED)
find_package(VTK COMPONENTS REQUIRED
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


# Process all headers with Q_OBJECT
QT4_WRAP_CPP(plugins_demo_MOC
  pie_chart_creator.h
  pie_chart.h
)

## File to compile
add_library(plugins_demo SHARED
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
  general ${AR2GEMS_LIBRARIES}
  optimized ${AR2GEMS_LIBRARIES}
  debug ${AR2GEMS_LIBRARIES}    
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



INSTALL(TARGETS plugins_demo DESTINATION "${INSTALL_LIB_DIR}")

   
# CREATE PACKAGE    
if (ALLOW_BUILD_PACKAGE)
    # COMMON    
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PACKAGE_MAINTAINER})
    
    SET(CPACK_PACKAGE_DESCRIPTION_FILE ${PACKAGE_DESCRIPTION_FILE})
    SET(CPACK_RESOURCE_FILE_LICENSE ${PACKAGE_LICENSE_FILE})
    
    SET(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
    SET(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
    SET(CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION_PATCH})
    
    SET(CPACK_SYSTEM_NAME ${PACKAGE_SYSTEM_NAME})
        	                          
	if (APPLE)
	    # ...
	elseif (UNIX)
	    ## DEB SPECIFIC
	    #if (PACKAGE_TYPE STREQUAL "DEB")
	        #SET(CPACK_GENERATOR "DEB")
	        #SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # autogenerate dependency information (package d-shlibs should be installed)
	        #SET(CPACK_DEBIAN_PACKAGE_DEPENDS "vtk (>= 6.1), libboost-system1.49.0 (>= 1.49.0)")
	    #endif (PACKAGE_TYPE STREQUAL "DEB")
	    
	    ## RPM SPECIFIC
	    #if (PACKAGE_TYPE STREQUAL "RPM")
	        #SET(CPACK_GENERATOR "RPM")
	    
	    #endif(PACKAGE_TYPE STREQUAL "RPM")
	              
	elseif (WIN32)
		set(CPACK_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
	    set(CPACK_GENERATOR "NSIS")
	    
		set(CPACK_PACKAGE_FILE_NAME "plugin")
		set(CPACK_NSIS_INSTALLER_OUTFILE_NAME ${CPACK_PACKAGE_FILE_NAME})
    	 
        #pass NATIVE PATHS to NSIS template
		string(REPLACE "/" "\\\\" WINNATIVE_PACKAGEL_LIB_DIR ${PACKAGEL_LIB_DIR})
	    set(CPACK_NSIS_AR2GEMS_PLUGIN_BINARY_DIR ${WINNATIVE_PACKAGEL_LIB_DIR})
            
	endif (APPLE)
    
    INCLUDE(CPack)
        	
endif (ALLOW_BUILD_PACKAGE)
  
  
