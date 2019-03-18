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

#include "tuner_mxl608.h"

static int mxl608x_init(struct dvb_frontend *fe)
{
	int ret = 0;

	ret = mxl608_init(fe);

	return ret;
}

static int mxl608x_get_frequency(struct dvb_frontend *fe, u32 *frequency)
{
	struct dual_tuner_priv *state = fe->tuner_priv;

	*frequency = state->frequency;

	return 0;
}

static int mxl608x_get_bandwidth(struct dvb_frontend *fe, u32 *bandwidth)
{
	struct dual_tuner_priv *state = fe->tuner_priv;

	*bandwidth = state->bandwidth;

	return 0;
}

static int mxl608x_get_if_frequency(struct dvb_frontend *fe, u32 *frequency)
{
	int ret = 0;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	*frequency = 0;

	switch (c->delivery_system) {
	case SYS_ATSC:
	case SYS_DVBC_ANNEX_A:
	case SYS_DVBC_ANNEX_C:
	case SYS_DVBC_ANNEX_B:
	case SYS_DVBT:
	case SYS_DVBT2:
		ret = mxl608_get_if_frequency(fe, frequency);
		break;
	default:
		break;
	}

	return ret;
}

static int mxl608x_get_status(struct dvb_frontend *fe, u32 *status)
{
	int ret = 0;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;

	*status = 0;

	switch (c->delivery_system) {
	case SYS_ATSC:
	case SYS_DVBC_ANNEX_A:
	case SYS_DVBC_ANNEX_C:
	case SYS_DVBC_ANNEX_B:
	case SYS_DVBT:
	case SYS_DVBT2:
		ret = mxl608_get_status(fe, status);
		break;
	default:
		break;
	}

	return ret;
}

static int mxl608x_release(struct dvb_frontend *fe)
{
	struct dual_tuner_priv *state = fe->tuner_priv;

	fe->tuner_priv = NULL;
	kfree(state);

	return 0;
}

static int mxl608x_sleep(struct dvb_frontend *fe)
{
	int ret = 0;

	ret = mxl608_sleep(fe);

	return ret;
}

static int mxl608x_set_params(struct dvb_frontend *fe)
{
	int ret = -1;
	struct dtv_frontend_properties *c = &fe->dtv_property_cache;
	struct dual_tuner_priv *state = fe->tuner_priv;

	dev_dbg(&state->i2c->dev,"delivery_system=%d frequency=%d " \
			"symbol_rate=%d bandwidth_hz=%d\n",
			c->delivery_system, c->frequency, c->symbol_rate,
			c->bandwidth_hz);

	switch (c->delivery_system) {
	case SYS_ATSC:
	case SYS_DVBC_ANNEX_A:
	case SYS_DVBC_ANNEX_C:
	case SYS_DVBC_ANNEX_B:
	case SYS_DVBT:
	case SYS_DVBT2:
		ret = mxl608_set_params(fe);
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

static struct dvb_tuner_ops mxl608x_tuner_ops = {
	.info = {
		.name = "MaxLinear MxL608"
	},

	.init              = mxl608x_init,
	.sleep             = mxl608x_sleep,
	.set_params        = mxl608x_set_params,
	.get_status        = mxl608x_get_status,
	.get_frequency     = mxl608x_get_frequency,
	.get_bandwidth     = mxl608x_get_bandwidth,
	.release           = mxl608x_release,
	.get_if_frequency  = mxl608x_get_if_frequency,
};

struct dvb_frontend *mxl608x_attach(struct dvb_frontend *fe,
	struct mxl608_config *cfg, struct i2c_adapter *i2c)
{
	struct dual_tuner_priv *state = NULL;
	int ret = 0;

	state = kzalloc(sizeof(struct dual_tuner_priv), GFP_KERNEL);
	if (!state) {
		ret = -ENOMEM;
		dev_err(&i2c->dev, "kzalloc() failed\n");
		goto err1;
	}

	state->cfg = NULL;
	state->config = cfg;
	state->i2c = i2c;

	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 1);

	ret = mxl608_get_chip_id(state);

	/* check return value of mxl608_get_chip_id */
	if (ret)
		goto err2;

	dev_info(&i2c->dev, "Attaching MxL608\n");

	fe->tuner_priv = state;

	memcpy(&fe->ops.tuner_ops, &mxl608x_tuner_ops,
		sizeof(struct dvb_tuner_ops));

	return fe;
err2:
	if (fe->ops.i2c_gate_ctrl)
		fe->ops.i2c_gate_ctrl(fe, 0);
	kfree(state);
err1:
	return NULL;

}
EXPORT_SYMBOL(mxl608x_attach);

MODULE_DESCRIPTION("MxL608 tuner driver");
MODULE_AUTHOR("Igor Mokrushin <mcmcc@mail.ru>");
MODULE_LICENSE("GPL");
