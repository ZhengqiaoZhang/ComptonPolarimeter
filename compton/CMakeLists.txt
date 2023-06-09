
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(pdet)

#shared library
set (LIB det)

#executable
set (BIN run)

#load GEANT
find_package(Geant4 REQUIRED ui_all vis_all)

#GEANT include
include(${Geant4_USE_FILE})

#local include
include_directories(${PROJECT_SOURCE_DIR}/include)

#sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

#BOOST
#set(BOOST_LIB boost_program_options)
#set(BOOST_LIB -I/usr/local/include/boost)

#ROOT

#compiler flags
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_FLAGS_CMD)
string(REPLACE "\n" "" ROOT_FLAGS "${ROOT_FLAGS_CMD}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_FLAGS}")

#libraries
set(ROOT_DEPS Core EG Hist Physics RIO Tree MathCore)

#path to libraries
execute_process(COMMAND root-config --libdir OUTPUT_VARIABLE ROOT_LIBDIR_CMD)
string(REPLACE "\n" "" ROOT_LIBDIR "${ROOT_LIBDIR_CMD}")
link_directories (${ROOT_LIBDIR})

#generate ROOT dictionary for detectors by CINT/CLING
set (DICT G__DET.cxx)
set (CLING rootcint)
set (LINKDEF LinkDef.h)
#set (HDRS_DET Detector.h SensDetDummy.h ExitWinZEUS.h)
set (HDRS_DET Detector.h)

#include for CLING
get_directory_property(incdirs INCLUDE_DIRECTORIES)
foreach(_inc ${incdirs})
  set(dict_include ${dict_include} -I${_inc})
endforeach()

# command to invoke CLING, options -c -p are used by root5
# and deprecated in root6
add_custom_command (
  OUTPUT ${DICT}
  COMMAND ${CLING}
  ARGS -f ${DICT} -c -p ${dict_include}
  ${HDRS_DET} ${LINKDEF}
  DEPENDS  include/${LINKDEF}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#shared library
add_library (${LIB} SHARED ${sources} ${headers} ${DICT})
#add_library (${LIB} SHARED ${sources} ${headers})
target_link_libraries(${LIB} ${Geant4_LIBRARIES} ${ROOT_DEPS})

#binary executable
add_executable(${BIN} run.cxx)
target_link_libraries(${BIN} ${Geant4_LIBRARIES} ${LIB} ${BOOST_LIB})

#copy macros to build directory
set(MACRO
  gui.mac
  init_vis.mac
  vis.mac
  run.mac
)

foreach(_mac ${MACRO})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_mac}
    ${PROJECT_BINARY_DIR}/${_mac}
    COPYONLY
  )
endforeach()

