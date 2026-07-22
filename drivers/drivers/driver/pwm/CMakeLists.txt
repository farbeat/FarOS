#===============================================================================
# @brief    cmake make file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "pwm")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pwm.c
)

set(PRIVATE_HEADER
)

set(PUBLIC_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    CONFIG_PWM_CHANNEL_NUM=6
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)
build_component()
