#===============================================================================
# @brief    cmake script path
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================

set(SCRIPT_DIR "${ROOT_DIR}/build/script")

set(PRO_GEN ${SCRIPT_DIR}/sdk_generator/project_generator.py)
set(BUILD_UTILS ${SCRIPT_DIR}/utils/build_utils.py)
set(ELF_TO_DU ${SCRIPT_DIR}/utils/elftodu.py)
set(MEM_STATE ${SCRIPT_DIR}/utils/mem_stats.py)
set(GEN_HEX ${SCRIPT_DIR}/utils/create_hex.py)
set(GEN_PATCH ${SCRIPT_DIR}/patch/patch_${ARCH_FAMILY}.py)

# HSO script
set(HDB_SCRIPT_DIR ${SCRIPT_DIR}/hdbxml)
set(MAK_HSO_XML ${HDB_SCRIPT_DIR}/mk_hso_prim_xml.py)
set(BT_STATUS_HSO_XML ${HDB_SCRIPT_DIR}/bt_status_to_hso_prim_xml.py)
set(OTA_MSG_HSO_XML ${HDB_SCRIPT_DIR}/ota_msg_to_hso_prim_xml.py)
set(HSO_XML_MERGE ${HDB_SCRIPT_DIR}/hso_prim_xml_merge.py)
set(HSO_XML_DB_CREATE ${HDB_SCRIPT_DIR}/database_create.py)
set(HSO_XML_PRE_PROCESS ${HDB_SCRIPT_DIR}/process_pregenerated_xml.py)

# PARSE script
set(OUTPUT_DIR "${ROOT_DIR}/output")
set(HSO_PARSE_DIR ${SCRIPT_DIR}/parse_tool)
set(HSO_PARSE_MAIN ${HSO_PARSE_DIR}/parse_main_phase1.py)

# sign tool
set(SIGN_TOOL ${ROOT_DIR}/tools/bin/sign_tool/sign_tool_pltuni)

# IMAGE ANALYSIS script
set(IMAGE_ANALSIS_TOOL ${SCRIPT_DIR}/image_analysis/build_data_analyzer_allinone.py)

# CODESIZE_STATISTIC script
set(CODESIZE_STATISTIC_TOOL ${SCRIPT_DIR}/codesize_statistic.py)
