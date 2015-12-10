#
# Copyright (c) 2015, Lab A Part
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

cmake_minimum_required(VERSION 2.6)

find_package(Board)
find_package(CMSIS)
find_package(RTOS)

add_definitions(-DRIOT_VERSION="2015.09")

# Remove '-DDEBUG' to avoid to get conflict with the RioT's macro DEBUG() 
string(REPLACE "-DDEBUG" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

set(riot_SRSC riotos.c
              ${RIOT_CORE_ROOT}/atomic.c
              ${RIOT_CORE_ROOT}/bitarithm.c
              ${RIOT_CORE_ROOT}/c11_atomic.c
              ${RIOT_CORE_ROOT}/clist.c
              ${RIOT_CORE_ROOT}/kernel_init.c
              ${RIOT_CORE_ROOT}/lifo.c
              ${RIOT_CORE_ROOT}/msg.c
              ${RIOT_CORE_ROOT}/mutex.c
              ${RIOT_CORE_ROOT}/panic.c
              ${RIOT_CORE_ROOT}/priority_queue.c
              ${RIOT_CORE_ROOT}/ringbuffer.c
              ${RIOT_CORE_ROOT}/reboot.c
              ${RIOT_CORE_ROOT}/sched.c
              ${RIOT_CORE_ROOT}/thread.c)

# ARM Cortex-M support
list(APPEND riot_SRSC ${RIOT_CPU_ROOT}/cortexm_common/atomic_arch.c
                      ${RIOT_CPU_ROOT}/cortexm_common/cortexm_init.c
                      ${RIOT_CPU_ROOT}/cortexm_common/irq_arch.c
                      ${RIOT_CPU_ROOT}/cortexm_common/panic.c
                      ${RIOT_CPU_ROOT}/cortexm_common/reboot_arch.c
                      ${RIOT_CPU_ROOT}/cortexm_common/thread_arch.c)

if (NOT DEFINED SUPPORT_RTOS_NO_CMSIS)
  list(APPEND riot_SRSC cmsis.c)
endif()

add_library(riot_rtos STATIC ${riot_SRSC})
