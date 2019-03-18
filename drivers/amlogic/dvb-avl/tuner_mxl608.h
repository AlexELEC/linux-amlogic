/*
 * Driver for MxL608 tuner.
 *
 * Copyright (C) 2019 Igor Mokrushin aka McMCC <mcmcc@mail.ru>
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
#ifndef TUNER_MXL608_H
#define TUNER_MXL608_H

#include <linux/kconfig.h>
#include "dvb_frontend.h"
#include <linux/i2c.h>
#include <linux/types.h>
#include "tuner-i2c.h"

#include "mxl608.h"
#include "tuner_ftm4862.h"

extern struct dvb_frontend *mxl608x_attach(struct dvb_frontend *fe,
					    struct mxl608_config *cfg,
					    struct i2c_adapter *i2c);

#endif /* TUNER_MXL608_H */
