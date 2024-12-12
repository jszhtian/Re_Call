/*
	---------------------------------------------------------------------------
	Copyright (c) 2003, Dominik Reichl <dominik.reichl@t-online.de>, Germany.
	All rights reserved.

	Distributed under the terms of the GNU General Public License v2.

	This software is provided 'as is' with no explicit or implied warranties
	in respect of its properties, including, but not limited to, correctness 
	and/or fitness for purpose.
	---------------------------------------------------------------------------
*/

#ifndef ___CRC16_H___
#define ___CRC16_H___

void crc16_init(unsigned short *uCrc16);
void crc16_update(unsigned short *uCrc16, unsigned char *pBuffer, unsigned long uBufSize);
void crc16_final(unsigned short *uCrc16);

void crc16ccitt_init(unsigned short *uCcitt16);
void crc16ccitt_update(unsigned short *uCcitt16, unsigned char *pBuffer, unsigned long uBufSize);
void crc16ccitt_final(unsigned short *uCcitt16);

#endif // ___CRC16_H___
