cmake_minimum_required(VERSION 3.16.5)
project(mkyaffs2image)
enable_language(C)

include_directories($(CMAKE_CURRENT_SOURCE_DIR))
ADD_COMPILE_OPTIONS(-O2 -Wall -Werror -DCONFIG_YAFFS_UTIL)
ADD_COMPILE_OPTIONS(-Wshadow -Wpointer-arith -Wwrite-strings -Wstrict-prototypes -Wmissing-declarations)
ADD_COMPILE_OPTIONS(-Wmissing-prototypes -Wredundant-decls -Wnested-externs -Winline)
ADD_COMPILE_OPTIONS(-fstack-protector-strong)
set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined  <${CMAKE_CURRENT_SOURCE_DIR}> <FLAGS> -o <OBJECT>  -c <SOURCE>")

set(ALL_SRCS mkyaffs2image.c yaffs_packedtags2.c yaffs_hweight.c yaffs_ecc.c)
SET(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/build/config/target_config/brandy/mk_fs_image/nandflash)
add_executable(mkyaffs2image ${ALL_SRCS})
