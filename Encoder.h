/* encoder.h*/

#ifndef ENCODER_H_
#define ENCODER_H_

#define PORT14_IOCR4_PC5_POS	(11UL)
#define PORT14_IOCR4_PC6_POS	(19UL)
#define PORT14_IOCR4_PC7_POS	(27UL)
#define PORT0_IOCR12_PC13_POS	(11UL)
#define PORT2_IOCR4_PC7_POS		(27UL)
#define INPUT_PULL_UP_MSK		(0x2UL)

#define SCU_PRESET0_CCU40RS_MSK	(0x4UL)
#define SCU_PRESET0_CCU40RS_POS	(2UL)
#define SCU_PRCLR0_CCU40RS_MSK	(0x4UL)
#define SCU_PRCLR0_CCU40RS_POS	(2UL)
#define SCU_CLKSET_CCUCEN_MSK	(0x10UL)
#define CCU4_GIDLC_SPRB_MSK		(0x100UL)
#define CCU4_GCTRL_PCIS_POS		(4UL)
#define CCU4_GCTRL_PCIS_MOD_CLK_MSK		(0x3UL)		//Needs to be negated ~
#define CCU4_GCTRL_PCIS_POS		(4UL)
#define CCU4_TC_TCM_EDGE_MSK	(1UL)				//Needs to be negated ~
#define CCU4_CC4_PRS_VALUE		(65535UL)
#define CCU4_GCSS_S0SE_MSK		(0x1UL)

#define POSIF_PCONF_FSEL_QUAD_MSK	(0x1UL)
#define POSIF_PCONF_INSEL0_POS		(8UL)
#define POSIF_PCONF_INSEL1_POS		(10UL)
#define POSIF_PCONF_INSEL2_POS		(12UL)
#define POSIF_IN_B_MSK				(0x1UL)
#define POSIF_PCONF_LPC_POS			(28UL)
#define POSIF_LOW_PASS_4			(0x3UL)

#define CCU4_INS_EV0IS_POS			(0UL)
#define CCU4_INS_EV1IS_POS			(4UL)
#define CCU4_INS_EV0EM_POS			(16UL)
#define CCU4_INS_EV1EM_POS			(18UL)
#define CCU4_IN_E_MSK				(0x4UL)
#define CCU4_IN_F_MSK				(0x5UL)
#define RISING_EDGE_ENC				(0x1UL)
#define	CCU4_CMC_CNTS_POS			(14UL)
#define CCU4_CMC_UDS_POS			(10UL)
#define CCU4_EVENT_0_MSK			(0x1UL)
#define CCU4_EVENT_1_MSK			(0x2UL)

#define POSIF_PRUNC_CRB_MSK			(0x1UL)
#define POSIF_PRUNC_CSM_MSK			(0x2UL)
#define CCU4_GIDLC_CS0I_MSK			(0x1UL)
#define CCU4_TCSET_TRBS_MSK			(0x1UL)
#define POSIF_PRUNS_SRB_MSK			(0x1UL)

#define PORTS_PDISC_5_MSK			(0x20UL)
#define PORTS_PDISC_6_MSK			(0x40UL)
#define PORTS_PDISC_7_MSK			(0x80UL)

#define SCU_PRESET0_POSIF0RS_MSK	(0x200UL)
#define SCU_PRCLR0_POSIF0RS_MSK		(0x200UL)

#define SCU_CLK_CGATCLR0_ERU1_MSK	(0x10000UL)
#define SCU_RESET_PRSET0_ERU1RS_MSK	(0x10000UL)
#define SCU_RESET_PRCLR0_ERU1RS_MSK	(0x10000UL)
#define EXS1B_ERU_1B0_MSK			(0x3UL)				//needs to be negated
#define EXS1B_ERU_1B0_POS			(6UL)
#define ERU0_EXICEL_EXS2A_2A2_MSK	(0x2UL)
#define ERU0_EXICEL_EXS2A_2A2_POS	(8UL)
#define ERU0_EXICON0_PE_MSK			(0x1UL)
#define ERU0_EXICON0_RE_MSK			(0x4UL)
#define ERU0_EXICON0_FE_MSK			(0x8UL)
#define ERU0_EXICON0_SS_A_MSK		(0x3UL)				//needs to be negated
#define ERU0_EXICON0_SS_POS			(8UL)
#define ERU0_EXOCON0_GP_MSK			(0x1UL)
#define ERU0_EXOCON0_GP_POS			(4UL)
#define ERU0_EXICON0_FL_MSK			(1UL << 7UL)
#define ERU0_EXICON_OCS_OGU2_MSK	(0x20UL)


extern void encoder_init (void);
extern void encoder_interrupt_init (void);
extern void encoder_interrupt_loop (void);


#endif /* ENCODER_H_ */


