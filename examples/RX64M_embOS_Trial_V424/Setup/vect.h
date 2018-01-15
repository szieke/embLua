/************************************************************************
*
* Device     : RX/RX600/RX64M
*
* File Name  : vect.h
*
* Abstract   : Definition of Vector.
*
* History    : 0.10  (2013-05-30)  [Hardware Manual Revision : 0.40]
*            : 0.50  (2013-10-28)  [Hardware Manual Revision : 0.50]
*            : 0.80  (2014-02-10)  [Hardware Manual Revision : 0.80]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2013(2014) Renesas Electronics Corporation and
* Renesas Solutions Corp. All rights reserved.
*
************************************************************************/

// Exception(Supervisor Instruction)
#pragma interrupt (Excep_SuperVisorInst)
void Excep_SuperVisorInst(void);

// Exception(Access Instruction)
#pragma interrupt (Excep_AccessInst)
void Excep_AccessInst(void);

// Exception(Undefined Instruction)
#pragma interrupt (Excep_UndefinedInst)
void Excep_UndefinedInst(void);

// Exception(Floating Point)
#pragma interrupt (Excep_FloatingPoint)
void Excep_FloatingPoint(void);

// NMI
#pragma interrupt (NonMaskableInterrupt)
void NonMaskableInterrupt(void);

// Dummy
#pragma interrupt (Dummy)
void Dummy(void);

// BRK
#pragma interrupt (Excep_BRK(vect=0))
void Excep_BRK(void);

// vector  1 reserved
// vector  2 reserved
// vector  3 reserved
// vector  4 reserved
// vector  5 reserved
// vector  6 reserved
// vector  7 reserved
// vector  8 reserved
// vector  9 reserved
// vector 10 reserved
// vector 11 reserved
// vector 12 reserved
// vector 13 reserved
// vector 14 reserved
// vector 15 reserved

// BSC BUSERR
#pragma interrupt (Excep_BSC_BUSERR(vect=16))
void Excep_BSC_BUSERR(void);

// vector 17 reserved

// RAM RAMERR
#pragma interrupt (Excep_RAM_RAMERR(vect=18))
void Excep_RAM_RAMERR(void);

// vector 19 reserved
// vector 20 reserved

// FCU FIFERR
#pragma interrupt (Excep_FCU_FIFERR(vect=21))
void Excep_FCU_FIFERR(void);

// vector 22 reserved

// FCU FRDYI
#pragma interrupt (Excep_FCU_FRDYI(vect=23))
void Excep_FCU_FRDYI(void);

// vector 24 reserved
// vector 25 reserved

// ICU SWINT2
#pragma interrupt (Excep_ICU_SWINT2(vect=26))
void Excep_ICU_SWINT2(void);

// ICU SWINT
#pragma interrupt (Excep_ICU_SWINT(vect=27))
void Excep_ICU_SWINT(void);

// CMT0 CMI0
//#pragma interrupt (Excep_CMT0_CMI0(vect=28))
//void Excep_CMT0_CMI0(void);

// CMT1 CMI1
#pragma interrupt (Excep_CMT1_CMI1(vect=29))
void Excep_CMT1_CMI1(void);

// CMTW0 CMWI0
#pragma interrupt (Excep_CMTW0_CMWI0(vect=30))
void Excep_CMTW0_CMWI0(void);

// CMTW1 CMWI1
#pragma interrupt (Excep_CMTW1_CMWI1(vect=31))
void Excep_CMTW1_CMWI1(void);

// USBA D0FIFO2
#pragma interrupt (Excep_USBA_D0FIFO2(vect=32))
void Excep_USBA_D0FIFO2(void);

// USBA D1FIFO2
#pragma interrupt (Excep_USBA_D1FIFO2(vect=33))
void Excep_USBA_D1FIFO2(void);

// USB0 D0FIFO0
#pragma interrupt (Excep_USB0_D0FIFO0(vect=34))
void Excep_USB0_D0FIFO0(void);

// USB0 D1FIFO0
#pragma interrupt (Excep_USB0_D1FIFO0(vect=35))
void Excep_USB0_D1FIFO0(void);

// vector 36 reserved
// vector 37 reserved

// RSPI0 SPRI0
#pragma interrupt (Excep_RSPI0_SPRI0(vect=38))
void Excep_RSPI0_SPRI0(void);

// RSPI0 SPTI0
#pragma interrupt (Excep_RSPI0_SPTI0(vect=39))
void Excep_RSPI0_SPTI0(void);

// vector 40 reserved
// vector 41 reserved

// QSPI SPRI
#pragma interrupt (Excep_QSPI_SPRI(vect=42))
void Excep_QSPI_SPRI(void);

// QSPI SPTI
#pragma interrupt (Excep_QSPI_SPTI(vect=43))
void Excep_QSPI_SPTI(void);

// SDHI SBFAI
#pragma interrupt (Excep_SDHI_SBFAI(vect=44))
void Excep_SDHI_SBFAI(void);

// MMCIF MBFAI
#pragma interrupt (Excep_MMCIF_MBFAI(vect=45))
void Excep_MMCIF_MBFAI(void);

// SSI0 SSITXI0
#pragma interrupt (Excep_SSI0_SSITXI0(vect=46))
void Excep_SSI0_SSITXI0(void);

// SSI0 SSIRXI0
#pragma interrupt (Excep_SSI0_SSIRXI0(vect=47))
void Excep_SSI0_SSIRXI0(void);

// SSI1 SSIRTI1
#pragma interrupt (Excep_SSI1_SSIRTI1(vect=48))
void Excep_SSI1_SSIRTI1(void);

// vector 49 reserved

// SRC IDEI
#pragma interrupt (Excep_SRC_IDEI(vect=50))
void Excep_SRC_IDEI(void);

// SRC ODFI
#pragma interrupt (Excep_SRC_ODFI(vect=51))
void Excep_SRC_ODFI(void);

// RIIC0 RXI0
#pragma interrupt (Excep_RIIC0_RXI0(vect=52))
void Excep_RIIC0_RXI0(void);

// RIIC0 TXI0
#pragma interrupt (Excep_RIIC0_TXI0(vect=53))
void Excep_RIIC0_TXI0(void);

// RIIC2 RXI2
#pragma interrupt (Excep_RIIC2_RXI2(vect=54))
void Excep_RIIC2_RXI2(void);

// RIIC2 TXI2
#pragma interrupt (Excep_RIIC2_TXI2(vect=55))
void Excep_RIIC2_TXI2(void);

// vector 56 reserved
// vector 57 reserved

// SCI0 RXI0
#pragma interrupt (Excep_SCI0_RXI0(vect=58))
void Excep_SCI0_RXI0(void);

// SCI0 TXI0
#pragma interrupt (Excep_SCI0_TXI0(vect=59))
void Excep_SCI0_TXI0(void);

// SCI1 RXI1
#pragma interrupt (Excep_SCI1_RXI1(vect=60))
void Excep_SCI1_RXI1(void);

// SCI1 TXI1
#pragma interrupt (Excep_SCI1_TXI1(vect=61))
void Excep_SCI1_TXI1(void);

// SCI2 RXI2
#pragma interrupt (Excep_SCI2_RXI2(vect=62))
void Excep_SCI2_RXI2(void);

// SCI2 TXI2
#pragma interrupt (Excep_SCI2_TXI2(vect=63))
void Excep_SCI2_TXI2(void);

// ICU IRQ0
#pragma interrupt (Excep_ICU_IRQ0(vect=64))
void Excep_ICU_IRQ0(void);

// ICU IRQ1
#pragma interrupt (Excep_ICU_IRQ1(vect=65))
void Excep_ICU_IRQ1(void);

// ICU IRQ2
#pragma interrupt (Excep_ICU_IRQ2(vect=66))
void Excep_ICU_IRQ2(void);

// ICU IRQ3
#pragma interrupt (Excep_ICU_IRQ3(vect=67))
void Excep_ICU_IRQ3(void);

// ICU IRQ4
#pragma interrupt (Excep_ICU_IRQ4(vect=68))
void Excep_ICU_IRQ4(void);

// ICU IRQ5
#pragma interrupt (Excep_ICU_IRQ5(vect=69))
void Excep_ICU_IRQ5(void);

// ICU IRQ6
#pragma interrupt (Excep_ICU_IRQ6(vect=70))
void Excep_ICU_IRQ6(void);

// ICU IRQ7
#pragma interrupt (Excep_ICU_IRQ7(vect=71))
void Excep_ICU_IRQ7(void);

// ICU IRQ8
#pragma interrupt (Excep_ICU_IRQ8(vect=72))
void Excep_ICU_IRQ8(void);

// ICU IRQ9
#pragma interrupt (Excep_ICU_IRQ9(vect=73))
void Excep_ICU_IRQ9(void);

// ICU IRQ10
#pragma interrupt (Excep_ICU_IRQ10(vect=74))
void Excep_ICU_IRQ10(void);

// ICU IRQ11
#pragma interrupt (Excep_ICU_IRQ11(vect=75))
void Excep_ICU_IRQ11(void);

// ICU IRQ12
#pragma interrupt (Excep_ICU_IRQ12(vect=76))
void Excep_ICU_IRQ12(void);

// ICU IRQ13
#pragma interrupt (Excep_ICU_IRQ13(vect=77))
void Excep_ICU_IRQ13(void);

// ICU IRQ14
#pragma interrupt (Excep_ICU_IRQ14(vect=78))
void Excep_ICU_IRQ14(void);

// ICU IRQ15
#pragma interrupt (Excep_ICU_IRQ15(vect=79))
void Excep_ICU_IRQ15(void);

// SCI3 RXI3
#pragma interrupt (Excep_SCI3_RXI3(vect=80))
void Excep_SCI3_RXI3(void);

// SCI3 TXI3
#pragma interrupt (Excep_SCI3_TXI3(vect=81))
void Excep_SCI3_TXI3(void);

// SCI4 RXI4
#pragma interrupt (Excep_SCI4_RXI4(vect=82))
void Excep_SCI4_RXI4(void);

// SCI4 TXI4
#pragma interrupt (Excep_SCI4_TXI4(vect=83))
void Excep_SCI4_TXI4(void);

// SCI5 RXI5
#pragma interrupt (Excep_SCI5_RXI5(vect=84))
void Excep_SCI5_RXI5(void);

// SCI5 TXI5
#pragma interrupt (Excep_SCI5_TXI5(vect=85))
void Excep_SCI5_TXI5(void);

// SCI6 RXI6
#pragma interrupt (Excep_SCI6_RXI6(vect=86))
void Excep_SCI6_RXI6(void);

// SCI6 TXI6
#pragma interrupt (Excep_SCI6_TXI6(vect=87))
void Excep_SCI6_TXI6(void);

// LVD1 LVD1
#pragma interrupt (Excep_LVD1_LVD1(vect=88))
void Excep_LVD1_LVD1(void);

// LVD2 LVD2
#pragma interrupt (Excep_LVD2_LVD2(vect=89))
void Excep_LVD2_LVD2(void);

// USB0 USBR0
#pragma interrupt (Excep_USB0_USBR0(vect=90))
void Excep_USB0_USBR0(void);

// vector 91 reserved

// RTC ALM
#pragma interrupt (Excep_RTC_ALM(vect=92))
void Excep_RTC_ALM(void);

// RTC PRD
#pragma interrupt (Excep_RTC_PRD(vect=93))
void Excep_RTC_PRD(void);

// USBA USBAR
#pragma interrupt (Excep_USBA_USBAR(vect=94))
void Excep_USBA_USBAR(void);

// IWDT IWUNI
#pragma interrupt (Excep_IWDT_IWUNI(vect=95))
void Excep_IWDT_IWUNI(void);

// WDT WUNI
#pragma interrupt (Excep_WDT_WUNI(vect=96))
void Excep_WDT_WUNI(void);

// PDC PCDFI
#pragma interrupt (Excep_PDC_PCDFI(vect=97))
void Excep_PDC_PCDFI(void);

// SCI7 RXI7
//#pragma interrupt (Excep_SCI7_RXI7(vect=98))
//void Excep_SCI7_RXI7(void);

// SCI7 TXI7
//#pragma interrupt (Excep_SCI7_TXI7(vect=99))
//void Excep_SCI7_TXI7(void);

// SCIFA8 RXIF8
#pragma interrupt (Excep_SCIFA8_RXIF8(vect=100))
void Excep_SCIFA8_RXIF8(void);

// SCIFA8 TXIF8
#pragma interrupt (Excep_SCIFA8_TXIF8(vect=101))
void Excep_SCIFA8_TXIF8(void);

// SCIFA9 RXIF9
#pragma interrupt (Excep_SCIFA9_RXIF9(vect=102))
void Excep_SCIFA9_RXIF9(void);

// SCIFA9 TXIF9
#pragma interrupt (Excep_SCIFA9_TXIF9(vect=103))
void Excep_SCIFA9_TXIF9(void);

// SCIFA10 RXIF10
#pragma interrupt (Excep_SCIFA10_RXIF10(vect=104))
void Excep_SCIFA10_RXIF10(void);

// SCIFA10 TXIF10
#pragma interrupt (Excep_SCIFA10_TXIF10(vect=105))
void Excep_SCIFA10_TXIF10(void);

// ICU GROUPBE0
#pragma interrupt (Excep_ICU_GROUPBE0(vect=106))
void Excep_ICU_GROUPBE0(void);

// vector 107 reserved
// vector 108 reserved
// vector 109 reserved

// ICU GROUPBL0
#pragma interrupt (Excep_ICU_GROUPBL0(vect=110))
void Excep_ICU_GROUPBL0(void);

// ICU GROUPBL1
#pragma interrupt (Excep_ICU_GROUPBL1(vect=111))
void Excep_ICU_GROUPBL1(void);

// ICU GROUPAL0
#pragma interrupt (Excep_ICU_GROUPAL0(vect=112))
void Excep_ICU_GROUPAL0(void);

// ICU GROUPAL1
#pragma interrupt (Excep_ICU_GROUPAL1(vect=113))
void Excep_ICU_GROUPAL1(void);

// SCIFA11 RXIF11
#pragma interrupt (Excep_SCIFA11_RXIF11(vect=114))
void Excep_SCIFA11_RXIF11(void);

// SCIFA11 TXIF11
#pragma interrupt (Excep_SCIFA11_TXIF11(vect=115))
void Excep_SCIFA11_TXIF11(void);

// SCI12 RXI12
#pragma interrupt (Excep_SCI12_RXI12(vect=116))
void Excep_SCI12_RXI12(void);

// SCI12 TXI12
#pragma interrupt (Excep_SCI12_TXI12(vect=117))
void Excep_SCI12_TXI12(void);

// vector 118 reserved
// vector 119 reserved

// DMAC DMAC0I
#pragma interrupt (Excep_DMAC_DMAC0I(vect=120))
void Excep_DMAC_DMAC0I(void);

// DMAC DMAC1I
#pragma interrupt (Excep_DMAC_DMAC1I(vect=121))
void Excep_DMAC_DMAC1I(void);

// DMAC DMAC2I
#pragma interrupt (Excep_DMAC_DMAC2I(vect=122))
void Excep_DMAC_DMAC2I(void);

// DMAC DMAC3I
#pragma interrupt (Excep_DMAC_DMAC3I(vect=123))
void Excep_DMAC_DMAC3I(void);

// DMAC DMAC74I
#pragma interrupt (Excep_DMAC_DMAC74I(vect=124))
void Excep_DMAC_DMAC74I(void);

// OST OST
#pragma interrupt (Excep_OST_OST(vect=125))
void Excep_OST_OST(void);

// EXDMAC EXDMAC0I
#pragma interrupt (Excep_EXDMAC_EXDMAC0I(vect=126))
void Excep_EXDMAC_EXDMAC0I(void);

// EXDMAC EXDMAC1I
#pragma interrupt (Excep_EXDMAC_EXDMAC1I(vect=127))
void Excep_EXDMAC_EXDMAC1I(void);

// PERIB INTB128
#pragma interrupt (Excep_PERIB_INTB128(vect=128))
void Excep_PERIB_INTB128(void);

// PERIB INTB129
#pragma interrupt (Excep_PERIB_INTB129(vect=129))
void Excep_PERIB_INTB129(void);

// PERIB INTB130
#pragma interrupt (Excep_PERIB_INTB130(vect=130))
void Excep_PERIB_INTB130(void);

// PERIB INTB131
#pragma interrupt (Excep_PERIB_INTB131(vect=131))
void Excep_PERIB_INTB131(void);

// PERIB INTB132
#pragma interrupt (Excep_PERIB_INTB132(vect=132))
void Excep_PERIB_INTB132(void);

// PERIB INTB133
#pragma interrupt (Excep_PERIB_INTB133(vect=133))
void Excep_PERIB_INTB133(void);

// PERIB INTB134
#pragma interrupt (Excep_PERIB_INTB134(vect=134))
void Excep_PERIB_INTB134(void);

// PERIB INTB135
#pragma interrupt (Excep_PERIB_INTB135(vect=135))
void Excep_PERIB_INTB135(void);

// PERIB INTB136
#pragma interrupt (Excep_PERIB_INTB136(vect=136))
void Excep_PERIB_INTB136(void);

// PERIB INTB137
#pragma interrupt (Excep_PERIB_INTB137(vect=137))
void Excep_PERIB_INTB137(void);

// PERIB INTB138
#pragma interrupt (Excep_PERIB_INTB138(vect=138))
void Excep_PERIB_INTB138(void);

// PERIB INTB139
#pragma interrupt (Excep_PERIB_INTB139(vect=139))
void Excep_PERIB_INTB139(void);

// PERIB INTB140
#pragma interrupt (Excep_PERIB_INTB140(vect=140))
void Excep_PERIB_INTB140(void);

// PERIB INTB141
#pragma interrupt (Excep_PERIB_INTB141(vect=141))
void Excep_PERIB_INTB141(void);

// PERIB INTB142
#pragma interrupt (Excep_PERIB_INTB142(vect=142))
void Excep_PERIB_INTB142(void);

// PERIB INTB143
#pragma interrupt (Excep_PERIB_INTB143(vect=143))
void Excep_PERIB_INTB143(void);

// PERIB INTB144
#pragma interrupt (Excep_PERIB_INTB144(vect=144))
void Excep_PERIB_INTB144(void);

// PERIB INTB145
#pragma interrupt (Excep_PERIB_INTB145(vect=145))
void Excep_PERIB_INTB145(void);

// PERIB INTB146
#pragma interrupt (Excep_PERIB_INTB146(vect=146))
void Excep_PERIB_INTB146(void);

// PERIB INTB147
#pragma interrupt (Excep_PERIB_INTB147(vect=147))
void Excep_PERIB_INTB147(void);

// PERIB INTB148
#pragma interrupt (Excep_PERIB_INTB148(vect=148))
void Excep_PERIB_INTB148(void);

// PERIB INTB149
#pragma interrupt (Excep_PERIB_INTB149(vect=149))
void Excep_PERIB_INTB149(void);

// PERIB INTB150
#pragma interrupt (Excep_PERIB_INTB150(vect=150))
void Excep_PERIB_INTB150(void);

// PERIB INTB151
#pragma interrupt (Excep_PERIB_INTB151(vect=151))
void Excep_PERIB_INTB151(void);

// PERIB INTB152
#pragma interrupt (Excep_PERIB_INTB152(vect=152))
void Excep_PERIB_INTB152(void);

// PERIB INTB153
#pragma interrupt (Excep_PERIB_INTB153(vect=153))
void Excep_PERIB_INTB153(void);

// PERIB INTB154
#pragma interrupt (Excep_PERIB_INTB154(vect=154))
void Excep_PERIB_INTB154(void);

// PERIB INTB155
#pragma interrupt (Excep_PERIB_INTB155(vect=155))
void Excep_PERIB_INTB155(void);

// PERIB INTB156
#pragma interrupt (Excep_PERIB_INTB156(vect=156))
void Excep_PERIB_INTB156(void);

// PERIB INTB157
#pragma interrupt (Excep_PERIB_INTB157(vect=157))
void Excep_PERIB_INTB157(void);

// PERIB INTB158
#pragma interrupt (Excep_PERIB_INTB158(vect=158))
void Excep_PERIB_INTB158(void);

// PERIB INTB159
#pragma interrupt (Excep_PERIB_INTB159(vect=159))
void Excep_PERIB_INTB159(void);

// PERIB INTB160
#pragma interrupt (Excep_PERIB_INTB160(vect=160))
void Excep_PERIB_INTB160(void);

// PERIB INTB161
#pragma interrupt (Excep_PERIB_INTB161(vect=161))
void Excep_PERIB_INTB161(void);

// PERIB INTB162
#pragma interrupt (Excep_PERIB_INTB162(vect=162))
void Excep_PERIB_INTB162(void);

// PERIB INTB163
#pragma interrupt (Excep_PERIB_INTB163(vect=163))
void Excep_PERIB_INTB163(void);

// PERIB INTB164
#pragma interrupt (Excep_PERIB_INTB164(vect=164))
void Excep_PERIB_INTB164(void);

// PERIB INTB165
#pragma interrupt (Excep_PERIB_INTB165(vect=165))
void Excep_PERIB_INTB165(void);

// PERIB INTB166
#pragma interrupt (Excep_PERIB_INTB166(vect=166))
void Excep_PERIB_INTB166(void);

// PERIB INTB167
#pragma interrupt (Excep_PERIB_INTB167(vect=167))
void Excep_PERIB_INTB167(void);

// PERIB INTB168
#pragma interrupt (Excep_PERIB_INTB168(vect=168))
void Excep_PERIB_INTB168(void);

// PERIB INTB169
#pragma interrupt (Excep_PERIB_INTB169(vect=169))
void Excep_PERIB_INTB169(void);

// PERIB INTB170
#pragma interrupt (Excep_PERIB_INTB170(vect=170))
void Excep_PERIB_INTB170(void);

// PERIB INTB171
#pragma interrupt (Excep_PERIB_INTB171(vect=171))
void Excep_PERIB_INTB171(void);

// PERIB INTB172
#pragma interrupt (Excep_PERIB_INTB172(vect=172))
void Excep_PERIB_INTB172(void);

// PERIB INTB173
#pragma interrupt (Excep_PERIB_INTB173(vect=173))
void Excep_PERIB_INTB173(void);

// PERIB INTB174
#pragma interrupt (Excep_PERIB_INTB174(vect=174))
void Excep_PERIB_INTB174(void);

// PERIB INTB175
#pragma interrupt (Excep_PERIB_INTB175(vect=175))
void Excep_PERIB_INTB175(void);

// PERIB INTB176
#pragma interrupt (Excep_PERIB_INTB176(vect=176))
void Excep_PERIB_INTB176(void);

// PERIB INTB177
#pragma interrupt (Excep_PERIB_INTB177(vect=177))
void Excep_PERIB_INTB177(void);

// PERIB INTB178
#pragma interrupt (Excep_PERIB_INTB178(vect=178))
void Excep_PERIB_INTB178(void);

// PERIB INTB179
#pragma interrupt (Excep_PERIB_INTB179(vect=179))
void Excep_PERIB_INTB179(void);

// PERIB INTB180
#pragma interrupt (Excep_PERIB_INTB180(vect=180))
void Excep_PERIB_INTB180(void);

// PERIB INTB181
#pragma interrupt (Excep_PERIB_INTB181(vect=181))
void Excep_PERIB_INTB181(void);

// PERIB INTB182
#pragma interrupt (Excep_PERIB_INTB182(vect=182))
void Excep_PERIB_INTB182(void);

// PERIB INTB183
#pragma interrupt (Excep_PERIB_INTB183(vect=183))
void Excep_PERIB_INTB183(void);

// PERIB INTB184
#pragma interrupt (Excep_PERIB_INTB184(vect=184))
void Excep_PERIB_INTB184(void);

// PERIB INTB185
#pragma interrupt (Excep_PERIB_INTB185(vect=185))
void Excep_PERIB_INTB185(void);

// PERIB INTB186
#pragma interrupt (Excep_PERIB_INTB186(vect=186))
void Excep_PERIB_INTB186(void);

// PERIB INTB187
#pragma interrupt (Excep_PERIB_INTB187(vect=187))
void Excep_PERIB_INTB187(void);

// PERIB INTB188
#pragma interrupt (Excep_PERIB_INTB188(vect=188))
void Excep_PERIB_INTB188(void);

// PERIB INTB189
#pragma interrupt (Excep_PERIB_INTB189(vect=189))
void Excep_PERIB_INTB189(void);

// PERIB INTB190
#pragma interrupt (Excep_PERIB_INTB190(vect=190))
void Excep_PERIB_INTB190(void);

// PERIB INTB191
#pragma interrupt (Excep_PERIB_INTB191(vect=191))
void Excep_PERIB_INTB191(void);

// PERIB INTB192
#pragma interrupt (Excep_PERIB_INTB192(vect=192))
void Excep_PERIB_INTB192(void);

// PERIB INTB193
#pragma interrupt (Excep_PERIB_INTB193(vect=193))
void Excep_PERIB_INTB193(void);

// PERIB INTB194
#pragma interrupt (Excep_PERIB_INTB194(vect=194))
void Excep_PERIB_INTB194(void);

// PERIB INTB195
#pragma interrupt (Excep_PERIB_INTB195(vect=195))
void Excep_PERIB_INTB195(void);

// PERIB INTB196
#pragma interrupt (Excep_PERIB_INTB196(vect=196))
void Excep_PERIB_INTB196(void);

// PERIB INTB197
#pragma interrupt (Excep_PERIB_INTB197(vect=197))
void Excep_PERIB_INTB197(void);

// PERIB INTB198
#pragma interrupt (Excep_PERIB_INTB198(vect=198))
void Excep_PERIB_INTB198(void);

// PERIB INTB199
#pragma interrupt (Excep_PERIB_INTB199(vect=199))
void Excep_PERIB_INTB199(void);

// PERIB INTB200
#pragma interrupt (Excep_PERIB_INTB200(vect=200))
void Excep_PERIB_INTB200(void);

// PERIB INTB201
#pragma interrupt (Excep_PERIB_INTB201(vect=201))
void Excep_PERIB_INTB201(void);

// PERIB INTB202
#pragma interrupt (Excep_PERIB_INTB202(vect=202))
void Excep_PERIB_INTB202(void);

// PERIB INTB203
#pragma interrupt (Excep_PERIB_INTB203(vect=203))
void Excep_PERIB_INTB203(void);

// PERIB INTB204
#pragma interrupt (Excep_PERIB_INTB204(vect=204))
void Excep_PERIB_INTB204(void);

// PERIB INTB205
#pragma interrupt (Excep_PERIB_INTB205(vect=205))
void Excep_PERIB_INTB205(void);

// PERIB INTB206
#pragma interrupt (Excep_PERIB_INTB206(vect=206))
void Excep_PERIB_INTB206(void);

// PERIB INTB207
#pragma interrupt (Excep_PERIB_INTB207(vect=207))
void Excep_PERIB_INTB207(void);

// PERIA INTA208
#pragma interrupt (Excep_PERIA_INTA208(vect=208))
void Excep_PERIA_INTA208(void);

// PERIA INTA209
#pragma interrupt (Excep_PERIA_INTA209(vect=209))
void Excep_PERIA_INTA209(void);

// PERIA INTA210
#pragma interrupt (Excep_PERIA_INTA210(vect=210))
void Excep_PERIA_INTA210(void);

// PERIA INTA211
#pragma interrupt (Excep_PERIA_INTA211(vect=211))
void Excep_PERIA_INTA211(void);

// PERIA INTA212
#pragma interrupt (Excep_PERIA_INTA212(vect=212))
void Excep_PERIA_INTA212(void);

// PERIA INTA213
#pragma interrupt (Excep_PERIA_INTA213(vect=213))
void Excep_PERIA_INTA213(void);

// PERIA INTA214
#pragma interrupt (Excep_PERIA_INTA214(vect=214))
void Excep_PERIA_INTA214(void);

// PERIA INTA215
#pragma interrupt (Excep_PERIA_INTA215(vect=215))
void Excep_PERIA_INTA215(void);

// PERIA INTA216
#pragma interrupt (Excep_PERIA_INTA216(vect=216))
void Excep_PERIA_INTA216(void);

// PERIA INTA217
#pragma interrupt (Excep_PERIA_INTA217(vect=217))
void Excep_PERIA_INTA217(void);

// PERIA INTA218
#pragma interrupt (Excep_PERIA_INTA218(vect=218))
void Excep_PERIA_INTA218(void);

// PERIA INTA219
#pragma interrupt (Excep_PERIA_INTA219(vect=219))
void Excep_PERIA_INTA219(void);

// PERIA INTA220
#pragma interrupt (Excep_PERIA_INTA220(vect=220))
void Excep_PERIA_INTA220(void);

// PERIA INTA221
#pragma interrupt (Excep_PERIA_INTA221(vect=221))
void Excep_PERIA_INTA221(void);

// PERIA INTA222
#pragma interrupt (Excep_PERIA_INTA222(vect=222))
void Excep_PERIA_INTA222(void);

// PERIA INTA223
#pragma interrupt (Excep_PERIA_INTA223(vect=223))
void Excep_PERIA_INTA223(void);

// PERIA INTA224
#pragma interrupt (Excep_PERIA_INTA224(vect=224))
void Excep_PERIA_INTA224(void);

// PERIA INTA225
#pragma interrupt (Excep_PERIA_INTA225(vect=225))
void Excep_PERIA_INTA225(void);

// PERIA INTA226
#pragma interrupt (Excep_PERIA_INTA226(vect=226))
void Excep_PERIA_INTA226(void);

// PERIA INTA227
#pragma interrupt (Excep_PERIA_INTA227(vect=227))
void Excep_PERIA_INTA227(void);

// PERIA INTA228
#pragma interrupt (Excep_PERIA_INTA228(vect=228))
void Excep_PERIA_INTA228(void);

// PERIA INTA229
#pragma interrupt (Excep_PERIA_INTA229(vect=229))
void Excep_PERIA_INTA229(void);

// PERIA INTA230
#pragma interrupt (Excep_PERIA_INTA230(vect=230))
void Excep_PERIA_INTA230(void);

// PERIA INTA231
#pragma interrupt (Excep_PERIA_INTA231(vect=231))
void Excep_PERIA_INTA231(void);

// PERIA INTA232
#pragma interrupt (Excep_PERIA_INTA232(vect=232))
void Excep_PERIA_INTA232(void);

// PERIA INTA233
#pragma interrupt (Excep_PERIA_INTA233(vect=233))
void Excep_PERIA_INTA233(void);

// PERIA INTA234
#pragma interrupt (Excep_PERIA_INTA234(vect=234))
void Excep_PERIA_INTA234(void);

// PERIA INTA235
#pragma interrupt (Excep_PERIA_INTA235(vect=235))
void Excep_PERIA_INTA235(void);

// PERIA INTA236
#pragma interrupt (Excep_PERIA_INTA236(vect=236))
void Excep_PERIA_INTA236(void);

// PERIA INTA237
#pragma interrupt (Excep_PERIA_INTA237(vect=237))
void Excep_PERIA_INTA237(void);

// PERIA INTA238
#pragma interrupt (Excep_PERIA_INTA238(vect=238))
void Excep_PERIA_INTA238(void);

// PERIA INTA239
#pragma interrupt (Excep_PERIA_INTA239(vect=239))
void Excep_PERIA_INTA239(void);

// PERIA INTA240
#pragma interrupt (Excep_PERIA_INTA240(vect=240))
void Excep_PERIA_INTA240(void);

// PERIA INTA241
#pragma interrupt (Excep_PERIA_INTA241(vect=241))
void Excep_PERIA_INTA241(void);

// PERIA INTA242
#pragma interrupt (Excep_PERIA_INTA242(vect=242))
void Excep_PERIA_INTA242(void);

// PERIA INTA243
#pragma interrupt (Excep_PERIA_INTA243(vect=243))
void Excep_PERIA_INTA243(void);

// PERIA INTA244
#pragma interrupt (Excep_PERIA_INTA244(vect=244))
void Excep_PERIA_INTA244(void);

// PERIA INTA245
#pragma interrupt (Excep_PERIA_INTA245(vect=245))
void Excep_PERIA_INTA245(void);

// PERIA INTA246
#pragma interrupt (Excep_PERIA_INTA246(vect=246))
void Excep_PERIA_INTA246(void);

// PERIA INTA247
#pragma interrupt (Excep_PERIA_INTA247(vect=247))
void Excep_PERIA_INTA247(void);

// PERIA INTA248
#pragma interrupt (Excep_PERIA_INTA248(vect=248))
void Excep_PERIA_INTA248(void);

// PERIA INTA249
#pragma interrupt (Excep_PERIA_INTA249(vect=249))
void Excep_PERIA_INTA249(void);

// PERIA INTA250
#pragma interrupt (Excep_PERIA_INTA250(vect=250))
void Excep_PERIA_INTA250(void);

// PERIA INTA251
#pragma interrupt (Excep_PERIA_INTA251(vect=251))
void Excep_PERIA_INTA251(void);

// PERIA INTA252
#pragma interrupt (Excep_PERIA_INTA252(vect=252))
void Excep_PERIA_INTA252(void);

// PERIA INTA253
#pragma interrupt (Excep_PERIA_INTA253(vect=253))
void Excep_PERIA_INTA253(void);

// PERIA INTA254
#pragma interrupt (Excep_PERIA_INTA254(vect=254))
void Excep_PERIA_INTA254(void);

// PERIA INTA255
#pragma interrupt (Excep_PERIA_INTA255(vect=255))
void Excep_PERIA_INTA255(void);

//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
extern void PowerON_Reset_PC(void);                                                                                                                
//;<<VECTOR DATA END (POWER ON RESET)>>
