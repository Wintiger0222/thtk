cmake_minimum_required(VERSION 3.0)
set(CMP0048 NEW)

project("Touhou Toolkit"
  VERSION 7)
set(PROJECT_URL "https://github.com/thpatch/thtk")
set(PROJECT_BUGREPORT "chwo9843@gmail.com")

include(CheckCSourceRuns)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckFunctionExists)

check_c_source_runs("
#include <stddef.h>
struct test_t { char a;
#pragma pack(push,1)
int b; };
#pragma pack(pop)

int main() {
return offsetof(struct test_t, b) == 1 ? 0 : 1;
}
" PACK_PRAGMA)
if (NOT ${PACK_PRAGMA})
  check_c_source_runs("
  #include <stddef.h>
  struct test_t { char a; int b; } __attribute__((packed));

  int main() {
  return offsetof(struct test_t, b) == 1 ? 0 : 1;
  }
  " PACK_ATTRIBUTE_AVAILABLE)
  if (${PACK_ATTRIBUTE_AVAILABLE})
    set(PACK_ATTRIBUTE "__attribute__((packed))")
  endif()
endif()

check_type_size(off_t OFF_T)
check_type_size(size_t SIZE_T)
if (NOT ${HAVE_SIZE_T})
  check_c_source_runs("
  #ifdef _WIN32
  # include <BaseTsd.h>
  typedef SIZE_T size_t;
  #else
  # include <sys/types.h>
  #endif

  int main() {
    size_t test = -1;
    return test>0 ? 0 : 1;
  }
  " HAVE_SSIZE_T_BASETSD)
endif()
check_type_size(ssize_t SSIZE_T)
if (NOT ${HAVE_SSIZE_T})
  check_c_source_runs("
  #ifdef _WIN32
  # include <BaseTsd.h>
  typedef SSIZE_T ssize_t;
  #else
  # include <sys/types.h>
  #endif

  int main() {
    ssize_t test = -1;
    return test<0 ? 0 : 1;
  }
  " HAVE_SSIZE_T_BASETSD)
endif()

check_include_file("libgen.h" HAVE_LIBGEN_H)
check_include_file("sys/stat.h" HAVE_SYS_STAT_H)
check_include_file("sys/mman.h" HAVE_SYS_MMAN_H)
check_include_file("unistd.h" HAVE_UNISTD_H)

check_function_exists("_splitpath" HAVE__SPLITPATH)
check_function_exists("mempcpy" HAVE_MEMPCPY)
check_function_exists("mmap" HAVE_MMAP)
check_function_exists("munmap" HAVE_MUNMAP)

check_function_exists("feof" HAVE_FEOF)
check_function_exists("fileno" HAVE_FILENO)
check_function_exists("fread" HAVE_FREAD)
check_function_exists("fwrite" HAVE_FWRITE)
check_function_exists("getc" HAVE_GETC)
check_function_exists("putc" HAVE_PUTC)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

option(WITH_LIBPNG_SOURCE "Compile libpng from source" ON)

if(MSVC)
  # Defining these here rather than in config.h.in also applies them to the
  # zlib-ng and libpng build.
  add_compile_options(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
else()
  add_compile_options(-D_GNU_SOURCE -std=c99)
endif()

function(link_setargv target)
  if(MSVC)
    set_target_properties(${target} PROPERTIES LINK_FLAGS "setargv.obj")
  endif()
endfunction()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(util)
include_directories(util)
add_subdirectory(thanm)
add_subdirectory(thdat)
add_subdirectory(thecl)
add_subdirectory(thmsg)
add_subdirectory(thtk)

configure_file(config.h.in config.h)
