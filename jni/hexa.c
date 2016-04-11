#include <string.h>			/* memcpy() */
#include <stdio.h>	
#include "hexa.h"

/* convert 1 hex digits of PDU to byte, return < 0 on error */
 int parse_hexdigit(int hex)
{
	if(hex >= '0' && hex <= '9')
		return hex - '0';
	if(hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	if(hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;
	return -1;
}




str_encoding_t get_encoding(recode_direction_t hint, const char* in, size_t length)
{
	if(hint == RECODE_ENCODE)
	{
		for(; length; --length, ++in)
			if(*in & 0x80)
				return STR_ENCODING_UCS2_HEX;
		return STR_ENCODING_7BIT_HEX;
	}
	else
	{
		size_t x;
		for(x = 0; x < length; ++x)
		{
			if(parse_hexdigit(in[x]) < 0) {
				return STR_ENCODING_7BIT;
			}
		}
		// TODO: STR_ENCODING_7BIT_HEX or STR_ENCODING_8BIT_HEX or STR_ENCODING_UCS2_HEX
	}

	return STR_ENCODING_UNKNOWN;
}

int check_encoding(const char* msg, unsigned length)
{
	str_encoding_t possible_enc = get_encoding(RECODE_ENCODE, msg, length);
	if(possible_enc == STR_ENCODING_7BIT_HEX)
		return PDU_DCS_ALPABET_7BIT;
	return PDU_DCS_ALPABET_UCS2;
}
