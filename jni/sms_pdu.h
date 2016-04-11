#ifndef USB_DONGLE_SMS_PDU_H_INCLUDED
#define USB_DONGLE_SMS_PDU_H_INCLUDED
#include "command.h"

#define TYPE_ADDRESS_SMSC 0x91



/* Message Type Indicator Parameter */
#define PDUTYPE_MTI_SHIFT			0
#define PDUTYPE_MTI_SMS_DELIVER			(0x00 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_DELIVER_REPORT		(0x00 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_SUBMIT			(0x01 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_SUBMIT_REPORT		(0x01 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_STATUS_REPORT		(0x02 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_COMMAND			(0x02 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_RESERVED			(0x03 << PDUTYPE_MTI_SHIFT)

#define PDUTYPE_MTI_MASK			(0x03 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI(pdutype)			((pdutype) & PDUTYPE_MTI_MASK)

/* Reject Duplicate */
#define PDUTYPE_RD_SHIFT			2
#define PDUTYPE_RD_ACCEPT			(0x00 << PDUTYPE_RD_SHIFT)
#define PDUTYPE_RD_REJECT			(0x01 << PDUTYPE_RD_SHIFT)

/* Validity Period Format */
#define PDUTYPE_VPF_SHIFT			3
#define PDUTYPE_VPF_NOT_PRESENT			(0x00 << PDUTYPE_VPF_SHIFT)
#define PDUTYPE_VPF_RESERVED			(0x01 << PDUTYPE_VPF_SHIFT)
#define PDUTYPE_VPF_RELATIVE			(0x02 << PDUTYPE_VPF_SHIFT)
#define PDUTYPE_VPF_ABSOLUTE			(0x03 << PDUTYPE_VPF_SHIFT)

/* Status Report Request */
#define PDUTYPE_SRR_SHIFT			5
#define PDUTYPE_SRR_NOT_REQUESTED		(0x00 << PDUTYPE_SRR_SHIFT)
#define PDUTYPE_SRR_REQUESTED			(0x01 << PDUTYPE_SRR_SHIFT)

/* User Data Header Indicator */
#define PDUTYPE_UDHI_SHIFT			6
#define PDUTYPE_UDHI_NO_HEADER			(0x00 << PDUTYPE_UDHI_SHIFT)
#define PDUTYPE_UDHI_HAS_HEADER			(0x01 << PDUTYPE_UDHI_SHIFT)
#define PDUTYPE_UDHI_MASK			(0x01 << PDUTYPE_UDHI_SHIFT)
#define PDUTYPE_UDHI(pdutype)			((pdutype) & PDUTYPE_UDHI_MASK)

/* eply Path Parameter */
#define PDUTYPE_RP_SHIFT			7
#define PDUTYPE_RP_IS_NOT_SET			(0x00 << PDUTYPE_RP_SHIFT)
#define PDUTYPE_RP_IS_SET			(0x01 << PDUTYPE_RP_SHIFT)

#define PDU_MESSAGE_REFERENCE			0x00		/* assigned by MS */

#define PDU_PID_SMS				0x00		/* bit5 No interworking, but SME-to-SME protocol = SMS */
#define PDU_PID_EMAIL				0x32		/* bit5 Telematic interworking, bits 4..0 0x 12  = email */
#define PDU_PID_SMS_REPLACE_MASK		0x40		/* bit7 Replace Short Message function activated (TP-PID = 0x41 to 0x47) */

/* DCS */
/*   bits 1..0 Class */
#define PDU_DCS_CLASS_SHIFT			0
#define PDU_DCS_CLASS0				(0x00 << PDU_DCS_CLASS_SHIFT)	/* Class 0, provides display and responds SC */
#define PDU_DCS_CLASS1				(0x01 << PDU_DCS_CLASS_SHIFT)	/* Class 1, saves to MS (NV); or saves to the UIM card when MS is full */
#define PDU_DCS_CLASS2				(0x02 << PDU_DCS_CLASS_SHIFT)	/* Class 2, dedicated for the UIM card The storage status is reported to SC after storage If the UIM card is full, failure and reason are reported to SC */
#define PDU_DCS_CLASS3				(0x03 << PDU_DCS_CLASS_SHIFT)	/* Class 3, stored to TE, MS receives messages and does not sent to TE, but responds to SC */
#define PDU_DCS_CLASS_MASK			(0x03 << PDU_DCS_CLASS_SHIFT)
#define PDU_DCS_CLASS(dcs)			((dcs) & PDU_DCS_CLASS_MASK)

/*   bits 3..2 Alpabet */
#define PDU_DCS_ALPABET_SHIFT			2
#define PDU_DCS_ALPABET_7BIT			(0x00 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_8BIT			(0x01 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_UCS2			(0x02 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_MASK			(0x03 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET(dcs)			((dcs) & PDU_DCS_ALPABET_MASK)

/*   bit 4 */
#define PDU_DCS_BITS10_CTRL_SHIFT		4
#define PDU_DCS_BITS10_RETAIN			(0x00 << PDU_DCS_BIT10_CTRL_SHIFT)
#define PDU_DCS_BITS10_INUSE			(0x01 << PDU_DCS_BIT10_CTRL_SHIFT)
#define PDU_DCS_BITS10_CTRL_MASK		(0x01 << PDU_DCS_BIT10_CTRL_SHIFT)
#define PDU_DCS_BITS10_CTRL(dcs)		((dcs) & PDU_DCS_BITS10_CTRL_MASK)

/*   bit 5 */
#define PDU_DCS_COMPRESSION_SHIFT		5
#define PDU_DCS_NOT_COMPESSED			(0x00 << PDU_DCS_COMPRESSION_SHIFT)
#define PDU_DCS_COMPESSED			(0x01 << PDU_DCS_COMPRESSION_SHIFT)
#define PDU_DCS_COMPRESSION_MASK		(0x01 << PDU_DCS_COMPRESSION_SHIFT)
#define PDU_DCS_COMPRESSION(dcs)		((dcs) & PDU_DCS_COMPRESSION_MASK)

/*   bit 7..6 */
#define PDU_DCS_76_SHIFT			6
#define PDU_DCS_76_00				(0x00 << PDU_DCS_76_SHIFT)
#define PDU_DCS_76_MASK				(0x03 << PDU_DCS_76_SHIFT)
#define PDU_DCS_76(dcs)				((dcs) & PDU_DCS_76_MASK)

#define ITEMS_OF(x)				(sizeof(x)/sizeof((x)[0]))

typedef ssize_t (*coder) (const char* in, size_t in_length, char* out, size_t out_size);

int create_pdu(t_smscommand *smscommand);

char pdu_digit2code(char digit);

int pdu_parse_sca(char ** pdu, size_t * length);

int pdu_store_phonenumber(char* buffer, const char* number, unsigned length);

#endif
