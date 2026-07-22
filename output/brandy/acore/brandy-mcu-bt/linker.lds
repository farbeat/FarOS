





OUTPUT_ARCH(riscv)
ENTRY(reset_vector)
MEMORY
{
    BOOTUP_RAM : ORIGIN = (0x10000), LENGTH = 436
    BOOTUP_LOAD : ORIGIN = ((((((((((((0x70000000) + ((27 + 5) * 4096) + 0 * 4096) + (32 * 4096)) + (15 * 4096)) + (1 * 4096)) + ((0) * 4096)) + ((64) * 4096)) + ((0) * 4096)) + ((112) * 4096)) + ((112) * 4096)) + ((320) * 4096))), LENGTH = 436


    FLASH_VERSION : ORIGIN = (((((((((((((0x70000000) + ((27 + 5) * 4096) + 0 * 4096) + (32 * 4096)) + (15 * 4096)) + (1 * 4096)) + ((0) * 4096)) + ((64) * 4096)) + ((0) * 4096)) + ((112) * 4096)) + ((112) * 4096)) + ((320) * 4096))) + 436), LENGTH = 88

    FLASH_PROGRAM : ORIGIN = ((((((((((((((0x70000000) + ((27 + 5) * 4096) + 0 * 4096) + (32 * 4096)) + (15 * 4096)) + (1 * 4096)) + ((0) * 4096)) + ((64) * 4096)) + ((0) * 4096)) + ((112) * 4096)) + ((112) * 4096)) + ((320) * 4096))) + 436) + 88), LENGTH = ((((1353) * 4096)) - (436 + 88))

    PSRAM_APP_CODE : ORIGIN = (0x6C000000 + (((0x10000) + (64) - 1) & (~((64) - 1)))), LENGTH = (((0xF0000) + (64) - 1) & (~((64) - 1)))
    PSRAM_APP_DATA : ORIGIN = ((0x6C000000 + (((0x10000) + (64) - 1) & (~((64) - 1)))) + (((0xF0000) + (64) - 1) & (~((64) - 1)))), LENGTH = (((0x100000) + (64) - 1) & (~((64) - 1)))

    PSRAM_HEAP : ORIGIN = (((((0x6C000000 + (((0x10000) + (64) - 1) & (~((64) - 1)))) + (((0xF0000) + (64) - 1) & (~((64) - 1)))) + (((0x100000) + (64) - 1) & (~((64) - 1)))) + (((0x2A800) + (64) - 1) & (~((64) - 1)))) + (((0x69E380) + (64) - 1) & (~((64) - 1)))), LENGTH = (((0x700000) + (64) - 1) & (~((64) - 1)))

    L2M_GPU_PROGRAM : ORIGIN = 0x60240000, LENGTH = 0x0
    L2M_DSP_PROGRAM : ORIGIN = 0x60136000, LENGTH = 0x4A000

    ITCM : ORIGIN = (0x10000 + 436), LENGTH = ((0x10000 + 0x30000) - (0x10000 + 436))




    DTCM : ORIGIN = 0x20000000, LENGTH = 0x70000

    IPC_SHARE_MEM : ORIGIN = (0x87000000), LENGTH = 0x4860
    PRESERVE_SHARE_MEM : ORIGIN = ((0x87000000) + 0x4860), LENGTH = 0xfc

    SYSTEM_PM : ORIGIN = ((((((0x87000000) + 0x4860) + 0xfc) + 0x0) + 0x40) + 0x80), LENGTH = 0xC0
    SYSTEM_STATUS : ORIGIN = (((((((0x87000000) + 0x4860) + 0xfc) + 0x0) + 0x40) + 0x80) + 0xC0), LENGTH = 0xc0
    CPUTRACE_RAM : ORIGIN = 0x52006000,LENGTH = 0x1FF8
}

SECTIONS
{

    . = 0;
    startup :
    {
        KEEP (*(.text.entry))
    } > BOOTUP_RAM AT > BOOTUP_LOAD

    .version_str :
    {
        . = ALIGN(8);
        KEEP (*(.flash_version))
    } > FLASH_VERSION
}

SECTIONS
{
    .psram_text :
    {
        __psram_text_start = .;
        __psram_text_load = LOADADDR(.psram_text);
        . = ALIGN(8);

        . = ALIGN(8);
        __psram_text_end = .;
    } > PSRAM_APP_CODE AT > FLASH_PROGRAM
    __psram_text_size = __psram_text_end - __psram_text_start;
    .l2m_gpu_text :
    {
        __l2m_gpu_text_start = .;
        __l2m_gpu_text_load = LOADADDR(.l2m_gpu_text);
        . = ALIGN(8);
        *libdisplay_gfx.a:yuv_to_rgb.c.obj* (.text .text* .rodata .rodata* .srodata .srodata*)
        . = ALIGN(8);
        __l2m_gpu_text_end = .;
    } > L2M_GPU_PROGRAM AT > FLASH_PROGRAM
    __l2m_gpu_text_size = __l2m_gpu_text_end - __l2m_gpu_text_start;

    .l2m_gpu_bss :
    {
        __l2m_gpu_bss_start = .;
        . = ALIGN(8);
        *libdisplay_gfx.a:yuv_to_rgb.c.obj* (.bss .bss* .sbss .sbss* COMMON)
        . = ALIGN(8);
        __l2m_gpu_bss_end = .;
    } > L2M_GPU_PROGRAM
     __l2m_gpu_bss_size = __l2m_gpu_bss_end - __l2m_gpu_bss_start;

    .l2m_gpu_data :
    {
        __l2m_gpu_data_start = .;
        __l2m_gpu_data_load = LOADADDR(.l2m_gpu_data);
        . = ALIGN(8);
        *libdisplay_gfx.a:yuv_to_rgb.c.obj* (.data .data* .sdata .sdata*)
        . = ALIGN(8);
        __l2m_gpu_data_end = .;
    } > L2M_GPU_PROGRAM AT > FLASH_PROGRAM
    __l2m_gpu_data_size = __l2m_gpu_data_end - __l2m_gpu_data_start;

    .l2m_dsp_text :
    {
        __l2m_dsp_text_start = .;
        __l2m_dsp_text_load = LOADADDR(.l2m_dsp_text);
        . = ALIGN(8);

        . = ALIGN(8);
        __l2m_dsp_text_end = .;
    } > L2M_DSP_PROGRAM AT > FLASH_PROGRAM
    __l2m_dsp_text_size = __l2m_dsp_text_end - __l2m_dsp_text_start;

    .l2m_dsp_bss :
    {
        __l2m_dsp_bss_start = .;
        . = ALIGN(8);

        . = ALIGN(8);
        __l2m_dsp_bss_end = .;
    } > L2M_DSP_PROGRAM
    __l2m_dsp_bss_size = __l2m_dsp_bss_end - __l2m_dsp_bss_start;

    .l2m_dsp_data :
    {
        __l2m_dsp_data_start = .;
        __l2m_dsp_data_load = LOADADDR(.l2m_dsp_data);
        . = ALIGN(8);

        . = ALIGN(8);
        __l2m_dsp_data_end = .;
    } > L2M_DSP_PROGRAM AT > FLASH_PROGRAM
    __l2m_dsp_data_size = __l2m_dsp_data_end - __l2m_dsp_data_start;
}

SECTIONS
{
    .plt_ramtext :
    {

        __ramtext_begin__ = .;
        __ramtext_load__ = LOADADDR(.plt_ramtext);
        . = ALIGN(8);


        *libhal*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/xip*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/flash*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/qspi*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/spi*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/lpm*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/dma*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/ulp_aon*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/uart*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/touch*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/libchip_brandy.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/libbrandy_clocks.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/libpmu_brandy.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/flash*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/xip*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/spi*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/uart*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/dma*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/pinctrl*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/systick*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *(.text.__udivdi3);

        *kernel/osal*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *kernel/non_os*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        EXCLUDE_FILE (*lw_weights.c.obj *lw_data_loader.c.obj *lw_forward.c.obj *lw_motion.c.obj)
            *application/brandy/brandy*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)

        *exception/libdfx_exception.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/cpu_trace/libcpu_trace.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *hal/cpu_trace/libhal_cpu_trace.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)







        SORT(*libbase.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libriscv.a:*.o*) (*.text *.text* .rodata .rodata* .srodata .srodata*)



        *utils/pm*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *middleware/chips/brandy/pm*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        KEEP(*(.text.lcd_get_status));

        SORT(*libsec.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libcsysdeps.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)


        *utils/update*.a:upg_common.c.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *utils/update*.a:upg_process.c.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *utils/update*.a:upg_patch.c.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *middleware/chips/brandy/update*.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)


        *utils/nv/libnv.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *chips/brandy/nv/nv_porting/libnv_brandy.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *hal/tcxo/libhal_tcxo.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/tcxo/libtcxo.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *drivers/chips/brandy/porting/tcxo/libtcxo_port.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *driver/systick/libsystick.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        *hal/systick/libhal_systick.a:*.o* (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*systemview.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        . = ALIGN(8);
        __init_array_start = .;
        KEEP(*(.ctors .init_array*))
        __init_array_end = .;
        __fini_array_start = .;
        KEEP(*(.dtors .fini_array*))
        __fini_array_end = .;
        . = ALIGN(8);
    } > ITCM AT > FLASH_PROGRAM

    .initcall.init : {
        . = ALIGN(8);
        __initcall0_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall0.init)))
        __initcall0_end = ABSOLUTE(.);

        __initcall1_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall1.init)))
        __initcall1_end = ABSOLUTE(.);

        __initcall2_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall2.init)))
        __initcall2_end = ABSOLUTE(.);

        __initcall3_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall3.init)))
        __initcall3_end = ABSOLUTE(.);

        __initcall4_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall4.init)))
        __initcall4_end = ABSOLUTE(.);

        __initcall5_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall5.init)))
        __initcall5_end = ABSOLUTE(.);

        __initcall6_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall6.init)))
        __initcall6_end = ABSOLUTE(.);

        __initcall7_start = ABSOLUTE(.);
        KEEP(*(SORT (.initcall7.init)))
        __initcall7_end = ABSOLUTE(.);

        . = ALIGN(8);
    } > ITCM AT > FLASH_PROGRAM


    .sysintcall.init : {
        . = ALIGN(4);
        __sysinitcall_start = ABSOLUTE(.);
        __sysinitcall0_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level0_*)))

        __sysinitcall1_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level1_*)))

        __sysinitcall2_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level2_*)))

        __sysinitcall3_start = ABSOLUTE(.);
        KEEP(SORT(*libinit.a:los_init.c.o*)(.sysinitcall_level3_s0.init))
        KEEP(SORT(*libbase.a:los_task.c.o*)(.sysinitcall_level3_s0.init))
        KEEP(SORT(*libtargets.a:os_adapt.c.o*)(.sysinitcall_level3_s1.init))
        KEEP(SORT(*libinit.a:los_init.c.o*)(.sysinitcall_level3_s1.init))
        KEEP(SORT(*libbase.a:los_hwi.c.o*)(.sysinitcall_level3_s1.init))
        KEEP(*(SORT(.sysinitcall_level3_s2.init)))
        KEEP(*(SORT(.sysinitcall_level3_s3.init)))
        KEEP(*(SORT(.sysinitcall_level3_s4.init)))

        __sysinitcall4_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level4_*)))

        __sysinitcall5_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level5_*)))

        __sysinitcall6_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level6_*)))

        __sysinitcall7_start = ABSOLUTE(.);
        KEEP(*(SORT(.sysinitcall_level7_*)))
        __sysinitcall_end = ABSOLUTE(.);
        . = ALIGN(4);
    } > ITCM AT > FLASH_PROGRAM



    .zInit : {
        . = ALIGN(8);
        __zinitcall_bsp_start = .;
        KEEP (*(.zinitcall.bsp*.init))
        __zinitcall_bsp_end = .;
        __zinitcall_device_start = .;
        KEEP (*(.zinitcall.device*.init))
        __zinitcall_device_end = .;
        __zinitcall_core_start = .;
        KEEP (*(.zinitcall.core*.init))
        __zinitcall_core_end = .;
        __zinitcall_sys_service_start = .;
        KEEP (*(.zinitcall.sys.service*.init))
        __zinitcall_sys_service_end = .;
        __zinitcall_sys_feature_start = .;
        KEEP (*(.zinitcall.sys.feature*.init))
        __zinitcall_sys_feature_end = .;
        __zinitcall_run_start = .;
        KEEP (*(.zinitcall.run*.init))
        __zinitcall_run_end = .;
        __zinitcall_sysex_service_start = .;
        KEEP (*(.zinitcall.sysex.service*.init))
        __zinitcall_sysex_service_end = .;
        __zinitcall_sysex_feature_start = .;
        KEEP (*(.zinitcall.sysex.feature*.init))
        __zinitcall_sysex_feature_end = .;
        __zinitcall_app_service_start = .;
        KEEP (*(.zinitcall.app.service0.init))
        KEEP (*(.zinitcall.app.service1.init))
        KEEP (*(.zinitcall.app.service*.init))
        __zinitcall_app_service_end = .;
        __zinitcall_app_feature_start = .;
        KEEP (*(.zinitcall.app.feature*.init))
        __zinitcall_app_feature_end = .;
        __zinitcall_test_start = .;
        KEEP (*(.zinitcall.test*.init))
        __zinitcall_test_end = .;
        __zinitcall_exit_start = .;
        KEEP (*(.zinitcall.exit*.init))
        __zinitcall_exit_end = .;
        . = ALIGN(8);
    } > ITCM AT > FLASH_PROGRAM
}

__ramtext_end__ = .;
__ramtext_size__ = __ramtext_end__ - __ramtext_begin__;

SECTIONS
{

    .gpu_ramtext :
    {
        . = ALIGN(8);

        . = ALIGN(8);
    } > FLASH_PROGRAM

    .text :
    {
        FILL(0xFF)
        . = ALIGN(8);


        SORT(*libc.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libcmsis.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libdriverbase.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libinit.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libinterrupt.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*liblinux.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libposix.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libm.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libtargets.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)

        SORT(*libsegger*.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libbt_host_sdk.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libbt_host_brandy_service.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libbt_host_brandy_sample.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libbt_test_audio_manager_wrapper.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libhmf_demuxer.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libplugin_demuxer_hmf.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libplugin_demuxer_mjpeg.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libplugin_protocol_https.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libgfx_rm.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)
        SORT(*libclang_rt.builtins-riscv32.a:*.o*) (.text .text* .rodata .rodata* .srodata .srodata*)






        *(.ramtext)
        *(.text)
        *(.text*)
        *(.flashtext)
        *(.flashtext*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
    } > FLASH_PROGRAM

    g_ram_begin = ORIGIN(DTCM);
    g_ram_size = LENGTH(DTCM);


    .stacks (0x20000000): ALIGN(16)
    {
        __int_stack_start = .;
        g_system_stack_begin = .;
        . += 0xC00;
        . = ALIGN(16);
        __stack_top__ = .;
        . += 0xC00;
        . = ALIGN(16);
        __irq_stack_top__ = .;
        . += 0xC00;
        . = ALIGN(16);
        __excp_stack_top__ = .;
        . += 0xC00;
        . = ALIGN(16);
        __nmi_stack_top__ = .;
        g_system_stack_end = .;
        __int_stack_end = .;
    } > DTCM
    g_system_stack_size = g_system_stack_end - g_system_stack_begin;
    g_stack_system = g_system_stack_size;
    __exc_stack_top = __excp_stack_top__;
    __nmi_stack_top = __nmi_stack_top__;


    .data :
    {
        __data_begin__ = .;
        __data_load__ = LOADADDR(.data);
        . = ALIGN(8);

        SORT(*libbase.a:*.o*) (.data .data*)
        SORT(*libc.a:*.o*) (.data .data*)
        SORT(*libcmsis.a:*.o*) (.data .data*)
        SORT(*libcsysdeps.a:*.o*) (.data .data*)
        SORT(*libdriverbase.a:*.o*) (.data .data*)
        SORT(*libinit.a:*.o*) (.data .data*)
        SORT(*libinterrupt.a:*.o*) (.data .data*)
        SORT(*liblinux.a:*.o*) (.data .data*)
        SORT(*libm.a:*.o*) (.data .data*)
        SORT(*libposix.a:*.o*) (.data .data*)
        SORT(*libsec:*.o*) (.data .data*)
        SORT(*libtargets.a:*.o*) (.data .data*)

        SORT(*libsegger*.a:*.o*) (.data .data*)
        SORT(*libbt_host_sdk.a:*.o*) (.data .data*)
        SORT(*libbt_host_brandy_service.a:*.o*) (.data .data*)
        SORT(*libbt_host_brandy_sample.a:*.o*) (.data .data*)
        SORT(*libbt_test_audio_manager_wrapper.a:*.o*) (.data .data*)
        SORT(*libhmf_demuxer.a:*.o*) (.data .data*)
        SORT(*libplugin_demuxer_hmf.a:*.o*) (.data .data*)
        SORT(*libplugin_demuxer_mjpeg.a:*.o*) (.data .data*)
        SORT(*libplugin_protocol_https.a:*.o*) (.data .data*)
        SORT(*libgfx_rm.a:*.o*) (.data .data*)





        SORT(*systemview.a:*.o*) (.data .data*)
        *(.data)
        *(.data*)
        *(.got)
        *(.got*)
        *(.igot)
        *(.igot*)
        _gp_ = . + 0x800;

        SORT(*libbase.a:*.o*) (.sdata .sdata*)
        SORT(*libc.a:*.o*) (.sdata .sdata*)
        SORT(*libcmsis.a:*.o*) (.sdata .sdata*)
        SORT(*libcsysdeps.a:*.o*) (.sdata .sdata*)
        SORT(*libdriverbase.a:*.o*) (.sdata .sdata*)
        SORT(*libinit.a:*.o*) (.sdata .sdata*)
        SORT(*libinterrupt.a:*.o*) (.sdata .sdata*)
        SORT(*liblinux.a:*.o*) (.sdata .sdata*)
        SORT(*libm.a:*.o*) (.sdata .sdata*)
        SORT(*libposix.a:*.o*) (.sdata .sdata*)
        SORT(*libsec:*.o*) (.sdata .sdata*)
        SORT(*libtargets.a:*.o*) (.sdata .sdata*)

        SORT(*libsegger*.a:*.o*) (.sdata .sdata*)
        SORT(*libgfx_rm.a:*.o*) (.sdata .sdata*)




        *(.sdata)
        *(.sdata*)
        . = ALIGN(0x4);
        KEEP(*( SORT (.liteos.table.*)));
        __data_end__ = .;
        . = ALIGN(8);
    } > DTCM AT > FLASH_PROGRAM
    __data_size__ = __data_end__ - __data_begin__;
    __global_pointer$ = _gp_;


    .bss (NOLOAD):
    {
        . = ALIGN(8);
        __bss_start = .;
        __bss_begin__ = .;


        SORT(*libbase.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libc.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libcmsis.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libcsysdeps.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libdriverbase.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libinit.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libinterrupt.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*liblinux.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libm.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libposix.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libsec:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libtargets.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)

        SORT(*libsegger*.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libbt_host_sdk.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libbt_host_brandy_service.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libbt_host_brandy_sample.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libbt_test_audio_manager_wrapper.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libhmf_demuxer.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libplugin_demuxer_hmf.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libplugin_demuxer_mjpeg.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libplugin_protocol_https.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        SORT(*libgfx_rm.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)





        SORT(*systemview.a:*.o*) (.bss .bss* .sbss .sbss* COMMON)
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(8);
        __eh_frame_start = ABSOLUTE(.);
        __eh_frame_end = ABSOLUTE(.);
        __bss_end__ = . ;
    } > DTCM
    __bss_size__ = __bss_end__ - __bss_begin__;

    .dtcm_stack (NOLOAD):
    {
        . = ALIGN(8);
        __heap_start = .;
        g_intheap_begin = ABSOLUTE(.);
        g_intheap_size = (ORIGIN(DTCM) + LENGTH(DTCM)) - g_intheap_begin;
    } > DTCM


    .preserve (NOLOAD):
    {
        . = ALIGN(8);
        KEEP(*(preserve))
    } > PRESERVE_SHARE_MEM
    .extend_heap (NOLOAD):
    {
        . = ALIGN(8);
        __extend_heap_start = .;
        g_extend_heap_begin = ABSOLUTE(.);
        g_extend_heap_size = (ORIGIN(L2M_DSP_PROGRAM) + LENGTH(L2M_DSP_PROGRAM)) - g_extend_heap_begin;
    } > L2M_DSP_PROGRAM


    .ipc_share_mem (NOLOAD) :
    {
        . = ALIGN(8);
        __ipc_share_mem_start__ = .;
        KEEP(*(.ipc_share_mem))
        KEEP(*(.ipc_share_mem0_1))
        KEEP(*(.ipc_share_mem0_2))
        KEEP(*(.ipc_share_mem1_0))
        KEEP(*(.ipc_share_mem1_2))
        KEEP(*(.ipc_share_mem2_0))
        KEEP(*(.ipc_share_mem2_1))
        __ipc_share_mem_end__ = .;
    } > IPC_SHARE_MEM

    .sys_status (NOLOAD) :
    {
        . = ALIGN(8);
        __sys_status_start__ = .;
        *(.sys_status)
        *(.sys_status*)
        __sys_status_end__ = .;
    } > SYSTEM_STATUS

    .cputrace_mem (NOLOAD):
    {
        . = ALIGN(8);
        KEEP(*(cputrace_mem))
    } > CPUTRACE_RAM


    .gcc_except_table :
    {
        . = ALIGN(8);
        *(.gcc_except_table)
        *(.gcc_except_table*)
    } > FLASH_PROGRAM
}


ASSERT((((((((((((((((0x70000000) + ((27 + 5) * 4096) + 0 * 4096) + (32 * 4096)) + (15 * 4096)) + (1 * 4096)) + ((0) * 4096)) + ((64) * 4096)) + ((0) * 4096)) + ((112) * 4096)) + ((112) * 4096)) + ((320) * 4096)) + ((1353) * 4096)) + ((4) * 4096)) + ((3) * 4096)) <= (0x70000000 + 0x800000)), "The used norflash size exceeds the maximum!");
