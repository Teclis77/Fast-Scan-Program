# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = ls_scan
# ls_scan.dbd will be created and installed
DBD += ls_scan.dbd

# ls_scan.dbd will be made up from these files:
ls_scan_DBD += base.dbd

# Include dbd files from all support applications:
#ls_scan_DBD += xxx.dbd
ls_scan_DBD += stream-base.dbd
ls_scan_DBD += drvAsynIPPort.dbd
ls_scan_DBD += asyn.dbd
ls_scan_DBD += calc.dbd

# Add all the support libraries needed by this IOC
#ls_scan_LIBS += xxx
ls_scan_LIBS += stream
ls_scan_LIBS += asyn
ls_scan_LIBS += calc

# ls_scan_registerRecordDeviceDriver.cpp derives from ls_scan.dbd
ls_scan_SRCS += ls_scan_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ls_scan_SRCS_DEFAULT += ls_scanMain.cpp
ls_scan_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#ls_scan_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
ls_scan_LIBS += qsrv
ls_scan_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
ls_scan_DBD += PVAServerRegister.dbd
ls_scan_DBD += qsrv.dbd
endif

ifneq ($(SNCSEQ),)
ls_scan_SNCFLAGS += +r
ls_scan_DBD += snc_ls_scan.dbd
ls_scan_SRCS += ls_vitruvio_sncscan.stt
ls_scan_SRCS += vitruvio_utils.c
ls_scan_SRCS += lissajous_utils.c
ls_scan_LIBS += seq pv
endif


# Finally link to the EPICS Base libraries
ls_scan_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

