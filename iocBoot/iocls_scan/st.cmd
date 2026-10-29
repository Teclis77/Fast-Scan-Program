#!../../bin/linux-x86_64/ls_scan

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change ls_scan to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

epicsEnvSet ("STREAM_PROTOCOL_PATH", "${TOP}/db")

## Register all support components
dbLoadDatabase "dbd/ls_scan.dbd"
ls_scan_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadRecords("db/ls_scan.db","user=xlabsrv11")
dbLoadRecords("db/ls_scan.db","user=ls_xena,server=xlabsrv11")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=xlabsrv11"
seq ls_vitruvio_sncscan,"user=ls_xena,server=xlabsrv11"
