set(MODULE_NAME sec)
set(LIBC_SEC_DIR_PATH ../../platform/libsec) # relative path due to makefile


# add global include, compile macro and options to public target #
set(MODULE_INCLUDE_PUB)
set(MODULE_CXXINCLUDE_PUB)
set(MODULE_COPTS_PUB)
set(MODULE_ASOPTS_PUB)
set(MODULE_CXXOPTS_PUB)
set(MODULE_CMACRO_PUB)
set(MODULE_ASMACRO_PUB)
set(MODULE_CXXMACRO_PUB)


# add local include, compile macro and options to private target #
set(MODULE_COPTS_PRI ${LITEOS_NON_SECURE_LOCAL_OPTS})
set(MODULE_ASOPTS_PRI ${LITEOS_NON_SECURE_LOCAL_OPTS})
set(MODULE_CXXOPTS_PRI ${LITEOS_NON_SECURE_LOCAL_OPTS})
set(MODULE_CMACRO_PRI)
set(MODULE_ASMACRO_PRI)
set(MODULE_CXXMACRO_PRI)


# add srcs to private target #
# In the minimum LIBC scenario, only the required memX_s/strX_s family of functions is included,
# and wide characters and floating point operations are not supported, if need you can add files.
if(NOT LOSCFG_LIB_FORMAT)
    set(LOCAL_SRCS_y
            memcpy_s.c
            memmove_s.c
            memset_s.c
            strcpy_s.c
            strncpy_s.c
            strcat_s.c
            strncat_s.c
            strtok_s.c
            snprintf_s.c
            vsnprintf_s.c
            secureprintoutput_a.c
    )
    list(TRANSFORM LOCAL_SRCS_y PREPEND ${LIBC_SEC_DIR_PATH}/src/)
    # Wide characters and floating point are not supported
    list(APPEND MODULE_CMACRO_PUB SECUREC_HAVE_WCTOMB=0 SECUREC_ENABLE_SPRINTF_FLOAT=0)
else()
    # STDIO macro enabled with all features (wide character and floating point default enabled)
    aux_source_directory(${LIBC_SEC_DIR_PATH}/src LOCAL_SRCS_y)
endif()

if (LOSCFG_KERNEL_LMS)
    list(APPEND MODULE_COPTS_PRI -fsanitize=kernel-address)
endif()

set(LOCAL_SRCS ${LOCAL_SRCS_y})
include(${MODULE})
