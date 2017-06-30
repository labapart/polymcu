#
# Copyright (c) 2017, Lab A Part
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

cmake_minimum_required(VERSION 2.8)

if(EXTERNAL_PROJECT_IN_BINARY_DIR)
  set(MICROPYTHON_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
  set(MICROPYTHON_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
else()
  set(MICROPYTHON_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
  set(MICROPYTHON_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

find_package(Board)
find_package(MicroPython)

set(MICROPYTHON_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(MICROPYTHON_BUILD_HEADER_DIR ${MICROPYTHON_BUILD_DIR}/genhdr)

# List of sources
set(micropython_lib_SRCS micropython.c
                         modpyb.c
                         led.c
                         uart_core.c)

# Add math support
set(LIBM_DIR ${MICROPYTHON_SRC_DIR}/lib/libm)
list(APPEND micropython_imported_SRCS ${LIBM_DIR}/acoshf.c
                                      ${LIBM_DIR}/asinfacosf.c
                                      ${LIBM_DIR}/asinhf.c
                                      ${LIBM_DIR}/atan2f.c
                                      ${LIBM_DIR}/atanf.c
                                      ${LIBM_DIR}/atanhf.c
                                      ${LIBM_DIR}/ef_rem_pio2.c
                                      ${LIBM_DIR}/erf_lgamma.c
                                      ${LIBM_DIR}/fdlibm.h
                                      ${LIBM_DIR}/fmodf.c
                                      ${LIBM_DIR}/kf_cos.c
                                      ${LIBM_DIR}/kf_rem_pio2.c
                                      ${LIBM_DIR}/kf_sin.c
                                      ${LIBM_DIR}/kf_tan.c
                                      ${LIBM_DIR}/log1pf.c
                                      ${LIBM_DIR}/math.c
                                      ${LIBM_DIR}/nearbyintf.c
                                      ${LIBM_DIR}/roundf.c
                                      ${LIBM_DIR}/sf_cos.c
                                      ${LIBM_DIR}/sf_erf.c
                                      ${LIBM_DIR}/sf_frexp.c
                                      ${LIBM_DIR}/sf_ldexp.c
                                      ${LIBM_DIR}/sf_modf.c
                                      ${LIBM_DIR}/sf_sin.c
                                      ${LIBM_DIR}/sf_tan.c
                                      ${LIBM_DIR}/wf_lgamma.c
                                      ${LIBM_DIR}/wf_tgamma.c)
if(CPU STREQUAL "ARM Cortex-M4")
  list(APPEND micropython_imported_SRCS ${LIBM_DIR}/thumb_vfp_sqrtf.c)
else()
  list(APPEND micropython_imported_SRCS ${LIBM_DIR}/ef_sqrt.c)
endif()

if (BOARD MATCHES "Nordic/")
  list(APPEND micropython_lib_SRCS board/nordic/pin.c board/nordic/nRF52_pins.c board/nordic/pin_named_pins.c)
endif()

if (SUPPORT_I2C)
  list(APPEND micropython_lib_SRCS i2c.c)
endif()

if(NOT IS_DIRECTORY ${MICROPYTHON_SRC_DIR})
  include(ExternalProject)

  ExternalProject_Add(micropython_src
    SOURCE_DIR ${MICROPYTHON_SRC_DIR}
    GIT_REPOSITORY https://github.com/micropython/micropython.git
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )

  # Workaround to add a dependency on sources from MicroPython repository to the PolyMCU library
  add_custom_command(OUTPUT ${micropython_imported_SRCS}
                     COMMAND echo
                     DEPENDS micropython_src)
endif()

# Generate flags
set(LIBMICROPYTHON_CFLAGS "${CMAKE_C_FLAGS} -Isrc -I. -I${MICROPYTHON_BUILD_DIR} -I${CMAKE_BINARY_DIR}")
get_directory_property(flags COMPILE_DEFINITIONS)
foreach(flag ${flags})
  set(LIBMICROPYTHON_CFLAGS "${LIBMICROPYTHON_CFLAGS} -D${flag}")
endforeach()
get_directory_property(dirs INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  set(LIBMICROPYTHON_CFLAGS "${LIBMICROPYTHON_CFLAGS} -I${dir}")
endforeach()

# List files for QSTR extraction
set(SOURCE_QSTR "")
foreach(_src ${micropython_lib_SRCS})
  set(SOURCE_QSTR "${SOURCE_QSTR} ${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
endforeach()

if (BOARD STREQUAL "Nordic/nRF52DK")
  # On Nordic boards, any peripherals can be assigned to any pins. There is no need to add
  # the complexity of the auto-generation
else()
  # If we use a script to generate the pin list then we need to set the following variables
  #set(MAKE_PINS board/nordic/make-pins.py)
  #set(BOARD_PINS board/nordic/nRF52DK.csv)
  #set(AF_FILE board/nordic/nRF52_af.csv)
  #set(PREFIX_FILE board/nordic/nRF52_prefix.c)
endif()

# If we use a script to generate the pin list
if (MAKE_PINS)
  set(GEN_PINS_HDR ${MICROPYTHON_BUILD_HEADER_DIR}/pins.h)
  set(GEN_PINS_QSTR ${MICROPYTHON_BUILD_DIR}/pins_qstr.h)
  set(GEN_PINS_AF_CONST ${MICROPYTHON_BUILD_HEADER_DIR}/pins_af_const.h)
  set(GEN_PINS_AF_PY ${MICROPYTHON_BUILD_DIR}/pins_af.py)
  set(GEN_PINS_SRC ${MICROPYTHON_BUILD_DIR}/pins_gen.c)

  add_custom_command(OUTPUT ${GEN_PINS_HDR} ${GEN_PINS_QSTR} ${GEN_PINS_AF_CONST} ${GEN_PINS_AF_PY} ${GEN_PINS_SRC}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${MICROPYTHON_BUILD_HEADER_DIR}
                     COMMAND python ${MAKE_PINS} --board ${BOARD_PINS} --af ${AF_FILE} --prefix ${PREFIX_FILE} --hdr ${GEN_PINS_HDR} --qstr ${GEN_PINS_QSTR} --af-const ${GEN_PINS_AF_CONST} --af-py ${GEN_PINS_AF_PY} > ${GEN_PINS_SRC}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     DEPENDS ${BOARD_PINS} ${AF_FILE} ${PREFIX_FILE}
                     COMMENT "Generate pins"
                     )
  list(APPEND micropython_lib_SRCS ${GEN_PINS_SRC})
endif()

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/libmicropython.a
                   COMMAND make V=1 CC=${CMAKE_C_COMPILER} CFLAGS=${LIBMICROPYTHON_CFLAGS} SOURCE_QSTR=${SOURCE_QSTR} libmicropython.a
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   DEPENDS ${micropython_lib_SRCS} mpconfigport.h
                   COMMENT "Build MicroPython main library"
                   )

# List sources that will be generated
set(micropython_generated_SRCS ${MICROPYTHON_SRC_DIR}/lib/utils/pyexec.c
                               ${MICROPYTHON_SRC_DIR}/lib/libc/string0.c
                               ${MICROPYTHON_SRC_DIR}/lib/mp-readline/readline.c
                               ${MICROPYTHON_SRC_DIR}/stmhal/bufhelper.c
                               ${MICROPYTHON_BUILD_DIR}/_frozen_mpy.c)

add_custom_command(OUTPUT ${micropython_generated_SRCS}
                   COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${MICROPYTHON_SRC_DIR}/py:$ENV{PYTHONPATH}" ${MICROPYTHON_SRC_DIR}/tools/mpy-tool.py -f -q ${MICROPYTHON_BUILD_HEADER_DIR}/qstrdefs.preprocessed.h -mlongint-impl=none ${MICROPYTHON_SRC_DIR}/minimal/frozentest.mpy > ${MICROPYTHON_BUILD_DIR}/_frozen_mpy.c
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libmicropython.a
                   COMMENT "Generate '_frozen_mpy.c'"
                   )

# Add generated source files
list(APPEND micropython_lib_SRCS ${MICROPYTHON_SRC_DIR}/lib/utils/pyexec.c
                                 ${MICROPYTHON_SRC_DIR}/lib/libc/string0.c
                                 ${MICROPYTHON_SRC_DIR}/lib/mp-readline/readline.c
                                 ${MICROPYTHON_SRC_DIR}/stmhal/bufhelper.c
                                 ${MICROPYTHON_BUILD_DIR}/_frozen_mpy.c)

list(APPEND micropython_lib_SRCS ${micropython_imported_SRCS})

add_library(polymcu_micropython STATIC ${micropython_lib_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/libmicropython.a)
