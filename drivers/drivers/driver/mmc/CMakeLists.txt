set(COMPONENT_NAME "drv_mmc")
set(FREEBSD_PATH ${ROOT_DIR}/open_source/freebsd)
set(FS_PATH ${ROOT_DIR}/middleware/utils/fs)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/block.c
    ${CMAKE_CURRENT_SOURCE_DIR}/core/sdio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/host/sdhci/sdhci_runstop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/host/sdhci/tune/sdhci_tune.c
    ${CMAKE_CURRENT_SOURCE_DIR}/host/sdhci/brandy/mmc_sdhci.c
    ${CMAKE_CURRENT_SOURCE_DIR}/host/hci.c
    ${CMAKE_CURRENT_SOURCE_DIR}/host/sdhci/adma2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mmc_init.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/mmc_adapt.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/mmc.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/mmcsd.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/mmc_sdv3.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/mmc_subr.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/sdio_func.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/core/sdio_ops.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/host/sdhci/sdhci_init.c
    ${FREEBSD_PATH}/liteos/drivers/mmc/host/sdhci/sdhci.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/host
    ${CMAKE_CURRENT_SOURCE_DIR}/host/sdhci
    ${CMAKE_CURRENT_SOURCE_DIR}/proc
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${FREEBSD_PATH}/liteos/drivers/mmc/include
    ${FREEBSD_PATH}/liteos/drivers/mmc/core
    ${FREEBSD_PATH}/liteos/drivers/mmc/host
    ${FREEBSD_PATH}/liteos/drivers/mmc/host/sdhci
    ${ROOT_DIR}/kernel/dpal/include
    ${ROOT_DIR}/kernel/dpal/device/include
    ${FS_PATH}/include
    ${FS_PATH}/vfs/include/bcache
    ${FS_PATH}/vfs/include/driver
    ${FS_PATH}/vfs/include/operation
)

if(${OS} STREQUAL "liteos")
    LIST(APPEND PUBLIC_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/adapt_liteos
    )
else()
    LIST(APPEND PUBLIC_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/adapt_freertos
    )
endif()

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
    CONFIG_DRIVERS_MMC_RW_IN_EXC
)

set(PUBLIC_DEFINES
    DPALCFG_DRIVERS_MMC
    DPALCFG_DRIVERS_EMMC
    DPALCFG_LIB_LIBC
    DPALCFG_COMPAT_POSIX
    DPALCFG_BASE_IPC_EVENT
    DPALCFG_BASE_CORE_TIMESLICE
    DPALCFG_DRIVERS_EMMC_HS200
    DPALCFG_FS_VFS
    SOC_OSAL
)

if (NOT DEFINES MATCHES "BUILD_RECOVERY_IMAGE")
    list(APPEND PUBLIC_DEFINES "DPALCFG_DRIVERS_SDIO")
endif()

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
    -Wno-visibility
    -Wno-unused-parameter
    -Wno-implicit-function-declaration
    -Wno-incompatible-pointer-types
    -Wno-unused-function
    -Wno-typedef-redefinition
    -Wno-parentheses-equality
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
