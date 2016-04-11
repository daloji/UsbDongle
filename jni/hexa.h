#ifndef USB_DONGLE_HEXA_H
#define USB_DONGLE_HEXA_H


/* Message Type Indicator Parameter */
#define PDUTYPE_MTI_SHIFT			0
#define PDUTYPE_MTI_SMS_DELIVER			(0x00 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_DELIVER_REPORT		(0x00 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_SUBMIT			(0x01 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_SUBMIT_REPORT		(0x01 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_STATUS_REPORT		(0x02 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_SMS_COMMAND			(0x02 << PDUTYPE_MTI_SHIFT)
#define PDUTYPE_MTI_RESERVED			(0x03 << PDUTYPE_MTI_SHIFT)


/*   bits 3..2 Alpabet */
#define PDU_DCS_ALPABET_SHIFT			2
#define PDU_DCS_ALPABET_7BIT			(0x00 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_8BIT			(0x01 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_UCS2			(0x02 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET_MASK			(0x03 << PDU_DCS_ALPABET_SHIFT)
#define PDU_DCS_ALPABET(dcs)			((dcs) & PDU_DCS_ALPABET_MASK)

#define ROUND_UP2(x)				(((x) + 1) & (0xFFFFFFFF << 1))

typedef enum {
	STR_ENCODING_7BIT_HEX		= 0,	/* 7bit encoding */
	STR_ENCODING_8BIT_HEX,			/* 8bit encoding */
	STR_ENCODING_UCS2_HEX,			/* UCS-2 in hex like PDU */
/* TODO: check its really 7bit input from device */
	STR_ENCODING_7BIT,			/* 7bit ASCII  no need recode to utf-8 */
//	STR_ENCODING_8BIT,			/* 8bit */
//	STR_ENCODING_UCS2,			/* UCS2 */
	STR_ENCODING_UNKNOWN,			/* still unknown */
} str_encoding_t;

typedef enum {
	RECODE_DECODE	=	0,		/* from encoded to UTF-8 */
	RECODE_ENCODE				/* from UTF-8 to encoded */
} recode_direction_t;


int check_encoding(const char* msg, unsigned length);
str_encoding_t get_encoding(recode_direction_t hint, const char* in, size_t length);

#endif 
