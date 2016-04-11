/**
 * \file sms_pdy.c
 * \brief note some Some function has been taken in chan-dongle project GPL license
 * .https://github.com/bg111/asterisk-chan-dongle/blob/master/pdu.c  
 * \author Johan Boris Iantila
 * \version 0.1
 * \date 21/01/2016
 *
 *
 */

#include <string.h>			
#include <stdio.h>
#include <errno.h>	
#include <stdlib.h>	
#include <iconv.h>	
#include "sms_pdu.h"
#include "hexa.h"


/**
 * \fn create_pdu 
 * \brief  SMS (Short Message Service) is specified by the ETSI (standards GSM 03.401 and 03.382 ).
 *  It can contain up to 160 characters, when each character is written according the 7-bits GSM default 
 * alphabet .There are two way to recieve and send SMS messages a PDU (protocol discription unit) and Text mode
 * for more information see http://www.smartposition.nl/resources/sms_pdu.html#PDU_CONVERTER
 * this function is used to create pdu mode sms  
 
 * \param t_smscommand smscommand.
 * */
int create_pdu(t_smscommand *smscommand){
	
	int len = 0;
	int data_len;

	int sca_toa = TYPE_ADDRESS_SMSC;
	int dst_toa = TYPE_ADDRESS_SMSC;
	int pdutype = PDUTYPE_MTI_SMS_SUBMIT | PDUTYPE_RD_ACCEPT | PDUTYPE_VPF_RELATIVE | PDUTYPE_SRR_NOT_REQUESTED | PDUTYPE_UDHI_NO_HEADER | PDUTYPE_RP_IS_NOT_SET;
	int dcs;

	unsigned dst_len;
	unsigned sca_len;
	unsigned msg_len;


	if(smscommand->messages == NULL){
		return -EINVAL;
	}
	
	if(smscommand->phoneNumber[0] =='+')
	  smscommand->phoneNumber++;
	  
	/* check buffer has enougth space */
	if( BUFFER_SIZE<  10 + ROUND_UP2(strlen(smscommand->phoneNumber) + 8 + strlen(smscommand->messages) * 4 + 4))
		return -ENOMEM;
		

	/* SCA Length */
	/* Type-of-address of the SMSC */
	/* Address of SMSC */
	smscommand->pduBuffer[len++] = '0';
	smscommand->pduBuffer[len++] = '0';
	
	
	/* PDU-type */
	/* TP-Message-Reference. The "00" value here lets the phone set the message reference number itself */
	/* Address-Length */
	/* Type-of-address of the sender number */
	len += snprintf(smscommand->pduBuffer + len, BUFFER_SIZE - len, "%02X%02X%02X%02X", pdutype, PDU_MESSAGE_REFERENCE, strlen(smscommand->phoneNumber), dst_toa);
	
	len += pdu_store_phonenumber(smscommand->pduBuffer + len, smscommand->phoneNumber, strlen(smscommand->phoneNumber));
	
	/* forward TP-User-Data */
	data_len = str_recode(RECODE_ENCODE, dcs == PDU_DCS_ALPABET_UCS2 ? STR_ENCODING_UCS2_HEX : STR_ENCODING_7BIT_HEX, smscommand->messages,strlen(smscommand->messages), smscommand->pduBuffer + len + 8, BUFFER_SIZE - len - 11);
	
	
	if(data_len < 0)
	{
		return -EINVAL;
	}
	else if(data_len > 160 * 2)
	{
		return -E2BIG;
	}
	
	
	/*find  type of message encoding  7Bit or UCS-2, not use 8Bit */
	dcs = check_encoding(smscommand->messages, strlen(smscommand->messages));
	
	/* calc UDL */
	msg_len = strlen(smscommand->messages);
	if(dcs == PDU_DCS_ALPABET_UCS2)
		msg_len = data_len / 2;

   
	/* TP-PID. Protocol identifier  */
	/* TP-DCS. Data coding scheme */
	/* TP-Validity-Period */
	/* TP-User-Data-Length */
	char tmp = smscommand->pduBuffer[len + 8];
	len += snprintf(smscommand->pduBuffer + len, BUFFER_SIZE - len, "%02X%02X%02X%02X", PDU_PID_SMS, dcs, pdu_relative_validity(3 * 24 * 60), msg_len);
	smscommand->pduBuffer[len] = tmp;
	len += data_len;
	


	return len;

}

/*!
 * \brief Store number in PDU
 * \param buffer -- pointer to place where number will be stored, CALLER MUST be provide length + 2 bytes of buffer
 * \param number -- phone number w/o leading '+'
 * \param length -- length of number
 * \return number of bytes written to buffer
 */
int pdu_store_phonenumber(char* buffer, const char* number, unsigned length){
	int i;
	for(i = 0; length > 1; length -=2, i +=2){
		buffer[i] = pdu_digit2code(number[i + 1]);
		buffer[i + 1] = pdu_digit2code(number[i]);
	}

	if(length){
		buffer[i] = 'F';
		buffer[i+1] = pdu_digit2code(number[i]);
		i += 2;
	}
	return i;
}


char pdu_digit2code(char digit){
	switch(digit){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		case '*':
			digit = 'A';
			break;
		case '#':
			digit = 'B';
			break;
		case 'a':
		case 'A':
			digit = 'C';
			break;
		case 'b':
		case 'B':
			digit = 'D';
			break;
		case 'c':
		case 'C':
			digit = 'E';
			break;
		default:
			return 0;
	}
	return digit;
}

/* convert 2 hex digits of PDU to byte, return < 0 on error */
static int pdu_parse_byte(char ** digits2hex, size_t * length){
	int res = -1;
	int res2;

	if(*length >= 2){
		res = parse_hexdigit(*digits2hex[0]);
		if(res >= 0){
			(*digits2hex)++;
			(*length)--;
			res2 = parse_hexdigit(*digits2hex[0]);
			if(res2 >= 0){
				(*digits2hex)++;
				(*length)--;
				return (res << 4) | res2;
			}
		}
	}
	return res;
}

int pdu_parse_sca(char ** pdu, size_t * length){
	/* get length of SCA field */
	int sca_len = pdu_parse_byte(pdu, length);

	if(sca_len >= 0){
		sca_len *= 2;
		if((size_t)sca_len <= *length){
			*pdu += sca_len;
			*length -= sca_len;

			/* TODO: Parse SCA Address */
			return sca_len + 2;
		}
	}
	return -EINVAL;
}
static ssize_t convert_string (const char* in, size_t in_length, char* out, size_t out_size, char* from, char* to){
	char*	in_ptr = (char*) in;
	size_t			in_bytesleft = in_length;
	char*			out_ptr = out;
	size_t			out_bytesleft = out_size - 1;
	iconv_t			cd = (iconv_t) -1;
	ssize_t			res;

	cd = iconv_open (to, from);
	if (cd == (iconv_t) -1){
		return -2;
	}

	res = iconv (cd, &in_ptr, &in_bytesleft, &out_ptr, &out_bytesleft);
	if (res < 0){
		iconv_close (cd);
		return -3;
	}

	iconv_close (cd);

	*out_ptr = '\0';

	return (out_ptr - out);
}


static ssize_t hexstr_to_8bitchars (const char* in, size_t in_length, char* out, size_t out_size){
	int d1, d2;

	/* odd number of chars check */
	if (in_length & 0x1)
		return -EINVAL;

	in_length = in_length >> 1;

	if (out_size - 1 < in_length){
		return -ENOMEM;
	}
	out_size = in_length;

	for (; in_length; --in_length){
		d1 = parse_hexdigit(*in++);
		if(d1 < 0)
			return -EINVAL;
		d2 = parse_hexdigit(*in++);
		if(d2 < 0)
			return -EINVAL;
		*out++ = (d1 << 4) | d2;
	}

	*out = 0;

	return out_size;
}

static ssize_t chars8bit_to_hexstr (const char* in, size_t in_length, char* out, size_t out_size){
	static const char hex_table[] = "0123456789ABCDEF";
	const unsigned char *in2 = (const unsigned char *)in;	/* for save time of first & 0x0F */

	if (out_size - 1 < in_length * 2){
		return -1;
	}
	out_size = in_length * 2;

	for (; in_length; --in_length, ++in2){
		*out++ = hex_table[*in2 >> 4];
		*out++ = hex_table[*in2 & 0xF];
	}

	*out = 0;

	return out_size;
}

static ssize_t hexstr_ucs2_to_utf8 (const char* in, size_t in_length, char* out, size_t out_size){
	char	buf[out_size];
	ssize_t	res;

	if (out_size - 1 < in_length / 2){
		return -1;
	}

	res = hexstr_to_8bitchars (in, in_length, buf, out_size);
	if (res < 0){
		return res;
	}

	res = convert_string (buf, res, out, out_size, "UCS-2BE", "UTF-8");

	return res;
}

static ssize_t utf8_to_hexstr_ucs2 (const char* in, size_t in_length, char* out, size_t out_size){
	char	buf[out_size];
	ssize_t	res;

	if (out_size - 1 < in_length * 4){
		return -1;
	}

	res = convert_string (in, in_length, buf, out_size, "UTF-8", "UCS-2BE");
	if (res < 0){
		return res;
	}

	res = chars8bit_to_hexstr (buf, res, out, out_size);

	return res;
}

static ssize_t char_to_hexstr_7bit (const char* in, size_t in_length, char* out, size_t out_size){
	size_t		i;
	size_t		x = 0;
	size_t		s;
	unsigned char	c;
	unsigned char	b;
	char	buf[] = { 0x0, 0x0, 0x0 };

	x = (in_length - in_length / 8) * 2;
	if (out_size - 1 < x){
		return -1;
	}

	if(in_length > 0){
		in_length--;
		for (i = 0, x = 0, s = 0; i < in_length; i++){
			if (s == 7){
				s = 0;
				continue;
			}

			c = in[i] >> s;
			b = in[i + 1] << (7 - s);
			c = c | b;
			s++;

			snprintf (buf, sizeof(buf), "%.2X", c);

			memcpy (out + x, buf, 2);
			x = x + 2;
		}

		c = in[i] >> s;
		snprintf (buf, sizeof(buf), "%.2X", c);
		memcpy (out + x, buf, 2);
		x = x + 2;
	}
	out[x] = '\0';

	return x;
}

static ssize_t hexstr_7bit_to_char (const char* in, size_t in_length, char* out, size_t out_size){
	size_t		i;
	size_t		x;
	size_t		s;
	int		hexval;
	unsigned char	c;
	unsigned char	b;
	char	buf[] = { 0x0, 0x0, 0x0 };

	in_length = in_length / 2;
	x = in_length + in_length / 7;
	if (out_size - 1 < x){
		return -1;
	}

	for (i = 0, x = 0, s = 1, b = 0; i < in_length; i++){
		memcpy (buf, in + i * 2, 2);
		if (sscanf (buf, "%x", &hexval) != 1){
			return -1;
		}
		c = ((unsigned char) hexval) << s;
		c = (c >> 1) | b;
		b = ((unsigned char) hexval) >> (8 - s);

		if (c == 0 && i + 1 < in_length) {
			/* @ is encoded as NUL */
			c = '@';
		}

		out[x] = c;
		x++; s++;

		if (s == 8){
			if (b == 0 && i + 1 < in_length) {
				/* @ is encoded as NUL */
				b = '@';
			}
			out[x] = b;
			s = 1; b = 0;
			x++;
		}
	}

	out[x] = '\0';

	return x;
}

#/* */
ssize_t just_copy (const char* in, size_t in_length, char* out, size_t out_size){
	// FIXME: or copy out_size-1 bytes only ?
	if (in_length <= out_size - 1){
		memcpy(out, in, in_length);
		out[in_length] = 0;
		return in_length;
	}
	return -ENOMEM;
}

int pdu_relative_validity(unsigned minutes){
#define DIV_UP(x,y)	(((x)+(y)-1)/(y))
	int validity;
	if(minutes <= 720)
		validity = DIV_UP(minutes, 5) - 1;
	else if(minutes <= 1440)
		validity = DIV_UP(minutes, 30) + 119;
	else if(minutes <= 43200)
		validity = DIV_UP(minutes, 1440) + 166;
	else if(minutes <= 635040)
		validity = DIV_UP(minutes, 10080) + 192;
	else
		validity = 0xFF;
	return validity;
#undef DIV_UP
}


static const coder recoders[][2] =
{
/* in order of values STR_ENCODING_*  */
	{ hexstr_7bit_to_char, char_to_hexstr_7bit },		/* STR_ENCODING_7BIT_HEX */
	{ hexstr_to_8bitchars, chars8bit_to_hexstr },		/* STR_ENCODING_8BIT_HEX */
	{ hexstr_ucs2_to_utf8, utf8_to_hexstr_ucs2 },		/* STR_ENCODING_UCS2_HEX */
	{ just_copy, just_copy },				/* STR_ENCODING_7BIT */
};


ssize_t str_recode(recode_direction_t dir, str_encoding_t encoding, const char* in, size_t in_length, char* out, size_t out_size)
{
	unsigned idx = encoding;
	if((dir == RECODE_DECODE || dir == RECODE_ENCODE) && idx < ITEMS_OF(recoders))
		return (recoders[idx][dir])(in, in_length, out, out_size);
	return -EINVAL;
}

void free_sms_cmd_memory(t_smscommand *smscommand){
	if(smscommand == NULL) 
	    return;
   if(smscommand->messages)
		free(smscommand->messages);
  
  if(smscommand->phoneNumber)
		free(smscommand->phoneNumber);
		
   if(smscommand->pduBuffer)
		free(smscommand->pduBuffer);
	free(smscommand);
}
