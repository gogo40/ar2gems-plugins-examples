# This module will first look into the directories defined by the variables:
# - AR2GEMS_PATH

# AR2GEMS_FOUND - System has AR2GEMS
# AR2GEMS_INCLUDE_DIRS - The AR2GEMS include directories
# AR2GEMS_LIBRARIES - The libraries needed to use AR2GEMS
# AR2GEMS_DEFINITIONS - Compiler switches required for using AR2GEMS

# Other standarnd issue macros
include (FindPackageHandleStandardArgs)
include (FindPackageMessage)

set (AR2GEMSFIND_DEBUG TRUE)  # print debug info

set (AR2GEMS_ALL_LIBS_FOUND TRUE) # internal flag to check if all required libs are found, if atleast one missing this flag set to FALSE

if (NOT AR2GEMS_PATH)
    GET_FILENAME_COMPONENT(AR2GEMS_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\SGeMS-ar2Tech-beta-x64;Path]"
        ABSOLUTE CACHE)
endif()

if (APPLE)
	# ???
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
elseif (UNIX)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
elseif (WIN32)
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
	SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
endif (APPLE)

set(LIB_NAMES
ar2gems_actions
ar2gems_appli
ar2gems_charts
ar2gems_extragui
ar2gems_filters
ar2gems_geostat
ar2gems_grid
ar2gems_gui
ar2gems_math
ar2gems_qwt
ar2gems_utils
ar2gems_widgets  
)

set(LIB_SEARCH_PATHES
${AR2GEMS_PATH}
/usr/lib64
/usr/lib
/usr/local/lib64
/usr/local/lib
/sw/lib
/opt/local/lib
)

set(HEADER_SEARCH_PATHES
${AR2GEMS_PATH}/include/
/usr/include
/usr/local/include
/sw/include
/opt/local/include
)
    
        
MACRO(FindAR2GEMS)
    if (AR2GEMSFIND_DEBUG)
        message(STATUS "AR2GEMS path: ${AR2GEMS_PATH}")
    endif ()
    
    find_path(AR2GEMS_INCLUDE_DIR ar2gems/sgems_version.h
        ${HEADER_SEARCH_PATHES}
        DOC "The directory where ar2gems/sgems_version.h resides")
    list(APPEND AR2GEMS_INCLUDE_DIRS ${AR2GEMS_INCLUDE_DIR})
    
    #message("${HEADER_SEARCH_PATHES}")
    #find_path(AR2GEMS_INCLUDE_DIR GsTL/cdf.h
        #${HEADER_SEARCH_PATHES}
        #DOC "The directory where GsTL/cdf/cdf_basics.h resides")
    #message("${AR2GEMS_INCLUDE_DIR}")        
    #list(APPEND AR2GEMS_INCLUDE_DIRS ${AR2GEMS_INCLUDE_DIR})
                  
    if (AR2GEMSFIND_DEBUG)
        message("includes: ${AR2GEMS_INCLUDE_DIRS}")
    endif(AR2GEMSFIND_DEBUG)
                        
    foreach(LIB ${LIB_NAMES})
        find_library(FOUND_${LIB} ${LIB} PATHS ${LIB_SEARCH_PATHES})
        if (AR2GEMSFIND_DEBUG)
            message("Lib: ${FOUND_${LIB}}")
            list(APPEND AR2GEMS_LIBRARIES ${FOUND_${LIB}})
        endif(AR2GEMSFIND_DEBUG)
        
        if (${LIB}-NOTFOUND)
            set(AR2GEMS_ALL_LIBS_FOUND FALSE)
            message("Lib not found: ${LIB}")
        endif (${LIB}-NOTFOUND)
    endforeach()  
    
    if(AR2GEMS_INCLUDE_DIRS AND AR2GEMS_LIBRARIES AND AR2GEMS_ALL_LIBS_FOUND)
        set(AR2GEMS_FOUND TRUE)
        if(AR2GEMSFIND_DEBUG)
            message(STATUS "Found AR2GEMS library ${AR2GEMS_LIBRARIES}")
            message(STATUS "Found AR2GEMS includes ${AR2GEMS_INCLUDE_DIRS}")
        endif(AR2GEMSFIND_DEBUG)
    else()
        set(AR2GEMS_FOUND FALSE)
        message(STATUS "AR2GEMS not found. Specify AR2GEMS_PATH to locate it")
    endif()
ENDMACRO()

