#
# (C) Copyright 2002
# Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
# David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
#
# SAMSUNG SMDK2440 board with S3C2440X (ARM920T) cpu
#
# see http://www.samsung.com/ for more information on SAMSUNG
#

#
# TQ2440 has 1 bank of 64 MB DRAM
#
# 3000'0000 to 3400'0000
#
# Linux-Kernel is expected to be at 3000'8000, entry 3000'8000
# optionally with a ramdisk at 3080'0000
#
# we load ourself to 33F8'0000
#
# download area is 3300'0000
#


TEXT_BASE = 0x33D80000
#TEXT_BASE = 0x37D80000
