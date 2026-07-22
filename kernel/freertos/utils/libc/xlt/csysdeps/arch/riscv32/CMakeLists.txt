set(COMPONENT_NAME "csysdeps")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/memcpy.S
    ${CMAKE_CURRENT_SOURCE_DIR}/memset.S
    ${CMAKE_CURRENT_SOURCE_DIR}/setjmp.S
    ${CMAKE_CURRENT_SOURCE_DIR}/strcmp.S
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
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
)
endif()

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
