set(COMPONENT_NAME "segger")
set(LIBS ${ROOT_DIR}/vendor/segger/riscv70/libsegger.a)
set(WHOLE_LINK
    true
)
build_component()

set(COMPONENT_NAME "m")

set(LIBC_DIR_PATH ${ROOT_DIR}/open_source/musl)
aux_source_directory(${LIBC_DIR_PATH}/src/math SOURCES)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${LIBC_DIR_PATH}/src/include
    ${LIBC_DIR_PATH}/src/internal
    ${LIBC_DIR_PATH}/arch/riscv32
    ${LIBC_DIR_PATH}/arch/generic
    ${LIBC_DIR_PATH}/include
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

if(${CHIP} MATCHES "ws63")
set(COMPONENT_CCFLAGS
	-nostdinc
	-nostdlib
	"-Wno-error=uninitialized"
	"-Wno-error=int-conversion"
	"-Wno-error=long-long"
	"-Wno-unused-variable"
	"-Wno-error=unused-variable"
	"-Wno-error=unused-value"
	"-Wno-error=unused-function"
	"-Wno-error=sign-compare"
	"-Wno-error=unused-parameter"
	"-Wno-error=tautological-compare"
	"-Wno-error=bad-function-cast"
	"-Wno-error=incompatible-pointer-types"
	"-Wno-error=implicit-function-declaration"
	"-Wno-missing-braces"
)
else()
set(COMPONENT_CCFLAGS
	-nostdinc
	-nostdlib
	-march=rv32imfcxlinxma_xlinxmb_xlinxmc
	-mabi=ilp32f
	--target=riscv32
	"-Wno-unused-parameter"
	"-Wno-sign-compare"
	"-Wno-error=out-of-line-declaration"
	"-Wno-error=uninitialized"
	"-Wno-error=int-conversion"
	"-Wno-error=enum-conversion"
	"-Wno-error=long-long"
	"-Wno-unused-variable"
	"-Wno-error=unused-variable"
	"-Wno-error=unused-value"
	"-Wno-error=unused-function"
	"-Wno-error=sign-compare"
	"-Wno-error=unused-parameter"
	"-Wno-error=tautological-compare"
	"-Wno-error=bad-function-cast"
	"-Wno-error=incompatible-pointer-types"
	"-Wno-error=implicit-function-declaration"
	"-Wno-missing-braces"
	"-Wno-main-return-type"
	"-Wno-unused-command-line-argument"
	"-Wno-macro-redefined"
	"-Wno-absolute-value"
)
endif()

if(DEFINED LIBM_WHOLE_LINK)
set(WHOLE_LINK
    true
)
else()
set(WHOLE_LINK
    false
)
endif()

set(MAIN_COMPONENT
    false
)

build_component()

install_sdk(${CMAKE_CURRENT_SOURCE_DIR} "*")
install_sdk(${ROOT_DIR}/vendor/segger/riscv/ "*")
