# Copyright (c) 2011-2018 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
# All rights reserved.
#
# Author: Volker Krause <volker.krause@kdab.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

find_program(GLSLANG_VALIDATOR_EXECUTABLE glslangValidator)
if (GLSLANG_VALIDATOR_EXECUTABLE)
  set(Glslang_FOUND TRUE)
endif()

# validate individual shaders
function(glslang_validate_shader)
  if (NOT GLSLANG_VALIDATOR_EXECUTABLE)
    return()
  endif()

  foreach(_shader ${ARGN})
    get_filename_component(_shader_abs ${_shader} ABSOLUTE)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_shader}.validate_shader
      COMMAND ${GLSLANG_VALIDATOR_EXECUTABLE} ${_shader_abs}
      COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${_shader}.validate_shader
      MAIN_DEPENDENCY ${_shader_abs}
    )
    add_custom_target(${_shader}_validate_shader ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_shader}.validate_shader)
  endforeach()

endfunction()

# validate an entire shader program, ie. also check for linker errors
function(glslang_validate_program)
  if (NOT GLSLANG_VALIDATOR_EXECUTABLE)
    return()
  endif()

  foreach(_shader ${ARGN})
    get_filename_component(_shader_abs ${_shader} ABSOLUTE)
    get_filename_component(_shader_name ${_shader} NAME)
    set(_target "${_target}${_shader_name}_")
    list(APPEND _shaders ${_shader_abs})
  endforeach()
  set(_target "${_target}validate_program")

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_target}.stamp
    COMMAND ${GLSLANG_VALIDATOR_EXECUTABLE} -l ${_shaders}
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${_target}.stamp
    DEPENDS ${_shaders}
  )
  add_custom_target(${_target} ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_target}.stamp)
endfunction()
