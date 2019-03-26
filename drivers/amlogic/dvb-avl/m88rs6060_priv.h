/*
 *    Driver for DVBS/S2/S2X Satellite demod/tuner(all-in-one) M88RS6060.
 *
 *    Copyright (C) 2019 Igor Mokrushin aka McMCC <mcmcc@mail.ru>
 *
 *    - Code based on driver for Montage Technology M88RS6000 by Max Nibble <nibble.max@gmail.com>
 *    - DVB-S2X not supported, it was not possible to test!
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef M88RS6060_PRIV_H
#define M88RS6060_PRIV_H

#define FW_DOWN_SIZE 64
#define FW_DOWN_LOOP (0x3000/FW_DOWN_SIZE)
#define RS6060_DEFAULT_FIRMWARE "dvb-demod-m88rs6060.fw"
#define MT_FE_MCLK_KHZ 96000 /* in kHz */
#define MT_FE_CRYSTAL_KHZ   27000 /* in kHz */
#define FREQ_OFFSET_AT_SMALL_SYM_RATE_KHz 3000
#define RS6060_ID	0x6060

struct m88rs6060_state {
	struct i2c_adapter *i2c;
	const struct m88rs6060_config *config;

	struct dvb_frontend frontend;

	u8 tuner_addr;
	u32 preBer;
	u8 skip_fw_load;
	u8 first_lock; /* The first time of signal lock */
	u16 demod_id; /* demod chip type */
	u16 tuner_id; /* tuner chip type */
	fe_delivery_system_t delivery_system;
	u32 iMclkKHz;
};

static u8 rs6060_reg_tbl_def[] = {
	0x04, 0x00,
	0x8a, 0x01,
	0x16, 0xa7,
	0x30, 0x88,
	0x4a, 0x80,
	0x4d, 0x91,
	0xae, 0x09,
	0x22, 0x01,
	0x23, 0x00,
	0x24, 0x00,
	0x27, 0x07,
	0x9c, 0x31,
	0x9d, 0xc1,
	0xcb, 0xf4,
	0xca, 0x02,
	0x7f, 0x04,
	0x78, 0x0c,
	0x85, 0x08,
	0x08, 0x47,
	0xf0, 0x03,
	0xfa, 0x01,
	0xf2, 0x00,
	0xfa, 0x00,
	0x08, 0x43,
	0xe0, 0xf8,
	0x00, 0x00,
	0xbd, 0x83,
	0xbe, 0xa1,
};

#endif /* M88RS6060_PRIV_H */
