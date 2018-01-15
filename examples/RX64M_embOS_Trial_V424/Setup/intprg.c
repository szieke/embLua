/************************************************************************
*
* Device     : RX/RX600/RX64M
*
* File Name  : intprg.c
*
* Abstract   : Interrupt Program.
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
*********************************************************************/
#include <machine.h>
#include "vect.h"

#pragma section IntPRG

// Exception(Supervisor Instruction)
void Excep_SuperVisorInst(void){/* brk(); */}

// Exception(Access Instruction)
void Excep_AccessInst(void){/* brk(); */}

// Exception(Undefined Instruction)
void Excep_UndefinedInst(void){/* brk(); */}

// Exception(Floating Point)
void Excep_FloatingPoint(void){/* brk(); */}

// NMI
void NonMaskableInterrupt(void){/* brk(); */}

// Dummy
void Dummy(void){/* brk(); */}

// BRK
void Excep_BRK(void){ wait(); }

// BSC BUSERR
void Excep_BSC_BUSERR(void){ }

// RAM RAMERR
void Excep_RAM_RAMERR(void){ }

// FCU FIFERR
void Excep_FCU_FIFERR(void){ }

// FCU FRDYI
void Excep_FCU_FRDYI(void){ }

// ICU SWINT2
void Excep_ICU_SWINT2(void){ }

// ICU SWINT
void Excep_ICU_SWINT(void){ }

// CMT0 CMI0
void Excep_CMT0_CMI0(void){ }

// CMT1 CMI1
void Excep_CMT1_CMI1(void){ }

// CMTW0 CMWI0
void Excep_CMTW0_CMWI0(void){ }

// CMTW1 CMWI1
void Excep_CMTW1_CMWI1(void){ }

// USBA D0FIFO2
void Excep_USBA_D0FIFO2(void){ }

// USBA D1FIFO2
void Excep_USBA_D1FIFO2(void){ }

// USB0 D0FIFO0
void Excep_USB0_D0FIFO0(void){ }

// USB0 D1FIFO0
void Excep_USB0_D1FIFO0(void){ }

// RSPI0 SPRI0
void Excep_RSPI0_SPRI0(void){ }

// RSPI0 SPTI0
void Excep_RSPI0_SPTI0(void){ }

// QSPI SPRI
void Excep_QSPI_SPRI(void){ }

// QSPI SPTI
void Excep_QSPI_SPTI(void){ }

// SDHI SBFAI
void Excep_SDHI_SBFAI(void){ }

// MMCIF MBFAI
void Excep_MMCIF_MBFAI(void){ }

// SSI0 SSITXI0
void Excep_SSI0_SSITXI0(void){ }

// SSI0 SSIRXI0
void Excep_SSI0_SSIRXI0(void){ }

// SSI1 SSIRTI1
void Excep_SSI1_SSIRTI1(void){ }

// SRC IDEI
void Excep_SRC_IDEI(void){ }

// SRC ODFI
void Excep_SRC_ODFI(void){ }

// RIIC0 RXI0
void Excep_RIIC0_RXI0(void){ }

// RIIC0 TXI0
void Excep_RIIC0_TXI0(void){ }

// RIIC2 RXI2
void Excep_RIIC2_RXI2(void){ }

// RIIC2 TXI2
void Excep_RIIC2_TXI2(void){ }

// SCI0 RXI0
void Excep_SCI0_RXI0(void){ }

// SCI0 TXI0
void Excep_SCI0_TXI0(void){ }

// SCI1 RXI1
void Excep_SCI1_RXI1(void){ }

// SCI1 TXI1
void Excep_SCI1_TXI1(void){ }

// SCI2 RXI2
void Excep_SCI2_RXI2(void){ }

// SCI2 TXI2
void Excep_SCI2_TXI2(void){ }

// ICU IRQ0
void Excep_ICU_IRQ0(void){ }

// ICU IRQ1
void Excep_ICU_IRQ1(void){ }

// ICU IRQ2
void Excep_ICU_IRQ2(void){ }

// ICU IRQ3
void Excep_ICU_IRQ3(void){ }

// ICU IRQ4
void Excep_ICU_IRQ4(void){ }

// ICU IRQ5
void Excep_ICU_IRQ5(void){ }

// ICU IRQ6
void Excep_ICU_IRQ6(void){ }

// ICU IRQ7
void Excep_ICU_IRQ7(void){ }

// ICU IRQ8
void Excep_ICU_IRQ8(void){ }

// ICU IRQ9
void Excep_ICU_IRQ9(void){ }

// ICU IRQ10
void Excep_ICU_IRQ10(void){ }

// ICU IRQ11
void Excep_ICU_IRQ11(void){ }

// ICU IRQ12
void Excep_ICU_IRQ12(void){ }

// ICU IRQ13
void Excep_ICU_IRQ13(void){ }

// ICU IRQ14
void Excep_ICU_IRQ14(void){ }

// ICU IRQ15
void Excep_ICU_IRQ15(void){ }

// SCI3 RXI3
void Excep_SCI3_RXI3(void){ }

// SCI3 TXI3
void Excep_SCI3_TXI3(void){ }

// SCI4 RXI4
void Excep_SCI4_RXI4(void){ }

// SCI4 TXI4
void Excep_SCI4_TXI4(void){ }

// SCI5 RXI5
void Excep_SCI5_RXI5(void){ }

// SCI5 TXI5
void Excep_SCI5_TXI5(void){ }

// SCI6 RXI6
void Excep_SCI6_RXI6(void){ }

// SCI6 TXI6
void Excep_SCI6_TXI6(void){ }

// LVD1 LVD1
void Excep_LVD1_LVD1(void){ }

// LVD2 LVD2
void Excep_LVD2_LVD2(void){ }

// USB0 USBR0
void Excep_USB0_USBR0(void){ }

// RTC ALM
void Excep_RTC_ALM(void){ }

// RTC PRD
void Excep_RTC_PRD(void){ }

// USBA USBAR
void Excep_USBA_USBAR(void){ }

// IWDT IWUNI
void Excep_IWDT_IWUNI(void){ }

// WDT WUNI
void Excep_WDT_WUNI(void){ }

// PDC PCDFI
void Excep_PDC_PCDFI(void){ }

// SCI7 RXI7
void Excep_SCI7_RXI7(void){ }

// SCI7 TXI7
void Excep_SCI7_TXI7(void){ }

// SCIFA8 RXIF8
void Excep_SCIFA8_RXIF8(void){ }

// SCIFA8 TXIF8
void Excep_SCIFA8_TXIF8(void){ }

// SCIFA9 RXIF9
void Excep_SCIFA9_RXIF9(void){ }

// SCIFA9 TXIF9
void Excep_SCIFA9_TXIF9(void){ }

// SCIFA10 RXIF10
void Excep_SCIFA10_RXIF10(void){ }

// SCIFA10 TXIF10
void Excep_SCIFA10_TXIF10(void){ }

// ICU GROUPBE0
void Excep_ICU_GROUPBE0(void){ }

// ICU GROUPBL0
void Excep_ICU_GROUPBL0(void){ }

// ICU GROUPBL1
void Excep_ICU_GROUPBL1(void){ }

// ICU GROUPAL0
void Excep_ICU_GROUPAL0(void){ }

// ICU GROUPAL1
void Excep_ICU_GROUPAL1(void){ }

// SCIFA11 RXIF11
void Excep_SCIFA11_RXIF11(void){ }

// SCIFA11 TXIF11
void Excep_SCIFA11_TXIF11(void){ }

// SCI12 RXI12
void Excep_SCI12_RXI12(void){ }

// SCI12 TXI12
void Excep_SCI12_TXI12(void){ }

// DMAC DMAC0I
void Excep_DMAC_DMAC0I(void){ }

// DMAC DMAC1I
void Excep_DMAC_DMAC1I(void){ }

// DMAC DMAC2I
void Excep_DMAC_DMAC2I(void){ }

// DMAC DMAC3I
void Excep_DMAC_DMAC3I(void){ }

// DMAC DMAC74I
void Excep_DMAC_DMAC74I(void){ }

// OST OST
void Excep_OST_OST(void){ }

// EXDMAC EXDMAC0I
void Excep_EXDMAC_EXDMAC0I(void){ }

// EXDMAC EXDMAC1I
void Excep_EXDMAC_EXDMAC1I(void){ }

// PERIB INTB128
void Excep_PERIB_INTB128(void){ }

// PERIB INTB129
void Excep_PERIB_INTB129(void){ }

// PERIB INTB130
void Excep_PERIB_INTB130(void){ }

// PERIB INTB131
void Excep_PERIB_INTB131(void){ }

// PERIB INTB132
void Excep_PERIB_INTB132(void){ }

// PERIB INTB133
void Excep_PERIB_INTB133(void){ }

// PERIB INTB134
void Excep_PERIB_INTB134(void){ }

// PERIB INTB135
void Excep_PERIB_INTB135(void){ }

// PERIB INTB136
void Excep_PERIB_INTB136(void){ }

// PERIB INTB137
void Excep_PERIB_INTB137(void){ }

// PERIB INTB138
void Excep_PERIB_INTB138(void){ }

// PERIB INTB139
void Excep_PERIB_INTB139(void){ }

// PERIB INTB140
void Excep_PERIB_INTB140(void){ }

// PERIB INTB141
void Excep_PERIB_INTB141(void){ }

// PERIB INTB142
void Excep_PERIB_INTB142(void){ }

// PERIB INTB143
void Excep_PERIB_INTB143(void){ }

// PERIB INTB144
void Excep_PERIB_INTB144(void){ }

// PERIB INTB145
void Excep_PERIB_INTB145(void){ }

// PERIB INTB146
void Excep_PERIB_INTB146(void){ }

// PERIB INTB147
void Excep_PERIB_INTB147(void){ }

// PERIB INTB148
void Excep_PERIB_INTB148(void){ }

// PERIB INTB149
void Excep_PERIB_INTB149(void){ }

// PERIB INTB150
void Excep_PERIB_INTB150(void){ }

// PERIB INTB151
void Excep_PERIB_INTB151(void){ }

// PERIB INTB152
void Excep_PERIB_INTB152(void){ }

// PERIB INTB153
void Excep_PERIB_INTB153(void){ }

// PERIB INTB154
void Excep_PERIB_INTB154(void){ }

// PERIB INTB155
void Excep_PERIB_INTB155(void){ }

// PERIB INTB156
void Excep_PERIB_INTB156(void){ }

// PERIB INTB157
void Excep_PERIB_INTB157(void){ }

// PERIB INTB158
void Excep_PERIB_INTB158(void){ }

// PERIB INTB159
void Excep_PERIB_INTB159(void){ }

// PERIB INTB160
void Excep_PERIB_INTB160(void){ }

// PERIB INTB161
void Excep_PERIB_INTB161(void){ }

// PERIB INTB162
void Excep_PERIB_INTB162(void){ }

// PERIB INTB163
void Excep_PERIB_INTB163(void){ }

// PERIB INTB164
void Excep_PERIB_INTB164(void){ }

// PERIB INTB165
void Excep_PERIB_INTB165(void){ }

// PERIB INTB166
void Excep_PERIB_INTB166(void){ }

// PERIB INTB167
void Excep_PERIB_INTB167(void){ }

// PERIB INTB168
void Excep_PERIB_INTB168(void){ }

// PERIB INTB169
void Excep_PERIB_INTB169(void){ }

// PERIB INTB170
void Excep_PERIB_INTB170(void){ }

// PERIB INTB171
void Excep_PERIB_INTB171(void){ }

// PERIB INTB172
void Excep_PERIB_INTB172(void){ }

// PERIB INTB173
void Excep_PERIB_INTB173(void){ }

// PERIB INTB174
void Excep_PERIB_INTB174(void){ }

// PERIB INTB175
void Excep_PERIB_INTB175(void){ }

// PERIB INTB176
void Excep_PERIB_INTB176(void){ }

// PERIB INTB177
void Excep_PERIB_INTB177(void){ }

// PERIB INTB178
void Excep_PERIB_INTB178(void){ }

// PERIB INTB179
void Excep_PERIB_INTB179(void){ }

// PERIB INTB180
void Excep_PERIB_INTB180(void){ }

// PERIB INTB181
void Excep_PERIB_INTB181(void){ }

// PERIB INTB182
void Excep_PERIB_INTB182(void){ }

// PERIB INTB183
void Excep_PERIB_INTB183(void){ }

// PERIB INTB184
void Excep_PERIB_INTB184(void){ }

// PERIB INTB185
void Excep_PERIB_INTB185(void){ }

// PERIB INTB186
void Excep_PERIB_INTB186(void){ }

// PERIB INTB187
void Excep_PERIB_INTB187(void){ }

// PERIB INTB188
void Excep_PERIB_INTB188(void){ }

// PERIB INTB189
void Excep_PERIB_INTB189(void){ }

// PERIB INTB190
void Excep_PERIB_INTB190(void){ }

// PERIB INTB191
void Excep_PERIB_INTB191(void){ }

// PERIB INTB192
void Excep_PERIB_INTB192(void){ }

// PERIB INTB193
void Excep_PERIB_INTB193(void){ }

// PERIB INTB194
void Excep_PERIB_INTB194(void){ }

// PERIB INTB195
void Excep_PERIB_INTB195(void){ }

// PERIB INTB196
void Excep_PERIB_INTB196(void){ }

// PERIB INTB197
void Excep_PERIB_INTB197(void){ }

// PERIB INTB198
void Excep_PERIB_INTB198(void){ }

// PERIB INTB199
void Excep_PERIB_INTB199(void){ }

// PERIB INTB200
void Excep_PERIB_INTB200(void){ }

// PERIB INTB201
void Excep_PERIB_INTB201(void){ }

// PERIB INTB202
void Excep_PERIB_INTB202(void){ }

// PERIB INTB203
void Excep_PERIB_INTB203(void){ }

// PERIB INTB204
void Excep_PERIB_INTB204(void){ }

// PERIB INTB205
void Excep_PERIB_INTB205(void){ }

// PERIB INTB206
void Excep_PERIB_INTB206(void){ }

// PERIB INTB207
void Excep_PERIB_INTB207(void){ }

// PERIA INTA208
void Excep_PERIA_INTA208(void){ }

// PERIA INTA209
void Excep_PERIA_INTA209(void){ }

// PERIA INTA210
void Excep_PERIA_INTA210(void){ }

// PERIA INTA211
void Excep_PERIA_INTA211(void){ }

// PERIA INTA212
void Excep_PERIA_INTA212(void){ }

// PERIA INTA213
void Excep_PERIA_INTA213(void){ }

// PERIA INTA214
void Excep_PERIA_INTA214(void){ }

// PERIA INTA215
void Excep_PERIA_INTA215(void){ }

// PERIA INTA216
void Excep_PERIA_INTA216(void){ }

// PERIA INTA217
void Excep_PERIA_INTA217(void){ }

// PERIA INTA218
void Excep_PERIA_INTA218(void){ }

// PERIA INTA219
void Excep_PERIA_INTA219(void){ }

// PERIA INTA220
void Excep_PERIA_INTA220(void){ }

// PERIA INTA221
void Excep_PERIA_INTA221(void){ }

// PERIA INTA222
void Excep_PERIA_INTA222(void){ }

// PERIA INTA223
void Excep_PERIA_INTA223(void){ }

// PERIA INTA224
void Excep_PERIA_INTA224(void){ }

// PERIA INTA225
void Excep_PERIA_INTA225(void){ }

// PERIA INTA226
void Excep_PERIA_INTA226(void){ }

// PERIA INTA227
void Excep_PERIA_INTA227(void){ }

// PERIA INTA228
void Excep_PERIA_INTA228(void){ }

// PERIA INTA229
void Excep_PERIA_INTA229(void){ }

// PERIA INTA230
void Excep_PERIA_INTA230(void){ }

// PERIA INTA231
void Excep_PERIA_INTA231(void){ }

// PERIA INTA232
void Excep_PERIA_INTA232(void){ }

// PERIA INTA233
void Excep_PERIA_INTA233(void){ }

// PERIA INTA234
void Excep_PERIA_INTA234(void){ }

// PERIA INTA235
void Excep_PERIA_INTA235(void){ }

// PERIA INTA236
void Excep_PERIA_INTA236(void){ }

// PERIA INTA237
void Excep_PERIA_INTA237(void){ }

// PERIA INTA238
void Excep_PERIA_INTA238(void){ }

// PERIA INTA239
void Excep_PERIA_INTA239(void){ }

// PERIA INTA240
void Excep_PERIA_INTA240(void){ }

// PERIA INTA241
void Excep_PERIA_INTA241(void){ }

// PERIA INTA242
void Excep_PERIA_INTA242(void){ }

// PERIA INTA243
void Excep_PERIA_INTA243(void){ }

// PERIA INTA244
void Excep_PERIA_INTA244(void){ }

// PERIA INTA245
void Excep_PERIA_INTA245(void){ }

// PERIA INTA246
void Excep_PERIA_INTA246(void){ }

// PERIA INTA247
void Excep_PERIA_INTA247(void){ }

// PERIA INTA248
void Excep_PERIA_INTA248(void){ }

// PERIA INTA249
void Excep_PERIA_INTA249(void){ }

// PERIA INTA250
void Excep_PERIA_INTA250(void){ }

// PERIA INTA251
void Excep_PERIA_INTA251(void){ }

// PERIA INTA252
void Excep_PERIA_INTA252(void){ }

// PERIA INTA253
void Excep_PERIA_INTA253(void){ }

// PERIA INTA254
void Excep_PERIA_INTA254(void){ }

// PERIA INTA255
void Excep_PERIA_INTA255(void){ }
