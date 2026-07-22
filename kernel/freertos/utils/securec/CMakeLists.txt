set(COMPONENT_NAME "sec")

set(SUPPORT_LIBC_FORMAT ON)
# src list
if(SUPPORT_LIBC_FORMAT)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCES)

if(${CHIP} MATCHES "ws63")
set(PRIVATE_DEFINES
)
else()
set(PRIVATE_DEFINES
    SECUREC_ENABLE_SPRINTF_FLOAT=1
)
endif()

else()
set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/memcpy_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/memmove_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/memset_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strcpy_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strncpy_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strcat_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strncat_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/strtok_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/sprintf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/snprintf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/sscanf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/vsprintf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/vsnprintf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/secureprintoutput_a.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/vsscanf_s.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/secureinput_a.c
)

# Wide characters and floating point are not supported
set(PRIVATE_DEFINES
	SECUREC_HAVE_WCTOMB=0
	SECUREC_ENABLE_SPRINTF_FLOAT=1
)
endif()

set(PUBLIC_HEADER
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(PRIVATE_HEADER
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
)
endif()

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()

install_sdk(${CMAKE_CURRENT_SOURCE_DIR} "*")
