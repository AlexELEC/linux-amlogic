/*
 * drivers/amlogic/pwm/meson-sysfs.c
 *
 *  add four new device nodes fot txl,they are constant,times,blink_enable
 *                                    blink_times.
 *
 * Copyright (C) 2016 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/

#undef pr_fmt
#define pr_fmt(fmt) "pwm: " fmt

#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/pwm.h>
#include <linux/amlogic/pwm_meson.h>

/**
 * pwm_constant_enable()
 *	- start a constant PWM output toggling
 *	  txl only support 8 channel constant output
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B
 */
int pwm_constant_enable(struct aml_pwm_chip *chip, int index)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;

	if ((id < 0) && (id > 7)) {
		dev_err(our_chip->chip.dev,
				"constant,index is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_set_reg_bits(our_chip->base + REG_MISC_AB,
						(1 << 28), (1 << 28));
		break;
	case PWM_B:
		pwm_set_reg_bits(our_chip->base + REG_MISC_AB,
						(1 << 29), (1 << 29));
		break;
	case PWM_C:
		pwm_set_reg_bits(our_chip->base + REG_MISC_CD,
						(1 << 28), (1 << 28));
		break;
	case PWM_D:
		pwm_set_reg_bits(our_chip->base + REG_MISC_CD,
						(1 << 29), (1 << 29));
		break;
	case PWM_E:
		pwm_set_reg_bits(our_chip->base + REG_MISC_EF,
						(1 << 28), (1 << 28));
		break;
	case PWM_F:
		pwm_set_reg_bits(our_chip->base + REG_MISC_EF,
						(1 << 29), (1 << 29));
		break;
	case PWM_AO_A:
		pwm_set_reg_bits(our_chip->ao_base + REG_MISC_AO_AB,
						(1 << 28), (1 << 28));
		break;
	case PWM_AO_B:
		pwm_set_reg_bits(our_chip->ao_base + REG_MISC_AO_AB,
						(1 << 29), (1 << 29));
		break;
	default:
	break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_constant_enable);


/**
 * pwm_constant_disable() - stop a constant PWM output toggling
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B
 */

int pwm_constant_disable(struct aml_pwm_chip *chip , int index)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;

	if ((id < 0) && (id > 7)) {
		dev_err(our_chip->chip.dev,
				"constant,index is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_set_reg_bits(our_chip->base + REG_MISC_AB,
						(0 << 28), (0 << 28));
		break;
	case PWM_B:
		pwm_set_reg_bits(our_chip->base + REG_MISC_AB,
						(0 << 29), (0 << 29));
		break;
	case PWM_C:
		pwm_set_reg_bits(our_chip->base + REG_MISC_CD,
						(0 << 28), (0 << 28));
		break;
	case PWM_D:
		pwm_set_reg_bits(our_chip->base + REG_MISC_CD,
						(0 << 29), (0 << 29));
		break;
	case PWM_E:
		pwm_set_reg_bits(our_chip->base + REG_MISC_EF,
						(0 << 28), (0 << 28));
		break;
	case PWM_F:
		pwm_set_reg_bits(our_chip->base + REG_MISC_EF,
						(0 << 29), (0 << 29));
		break;
	case PWM_AO_A:
		pwm_set_reg_bits(our_chip->ao_base + REG_MISC_AO_AB,
						(0 << 28), (0 << 28));
		break;
	case PWM_AO_B:
		pwm_set_reg_bits(our_chip->ao_base + REG_MISC_AO_AB,
						(0 << 29), (0 << 29));
		break;
	default:
	break;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(pwm_constant_disable);


static ssize_t pwm_constant_show(struct device *child,
			       struct device_attribute *attr, char *buf)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	return sprintf(buf, "%d\n", chip->variant.constant);
}

static ssize_t pwm_constant_store(struct device *child,
				  struct device_attribute *attr,
				  const char *buf, size_t size)

{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	int val, ret , id , res;

	res = sscanf(buf, "%d %d", &val, &id);
	if (res != 2) {
		dev_err(child, "Can't parse pwm id,usage:[value index]\n");
		return -EINVAL;
	}
	if ((id < 0) && (id > 7)) {
		dev_err(chip->chip.dev,
				"constant,index is not within the scope!\n");
		return -EINVAL;
	}

	switch (val) {
	case 0:
		ret = pwm_constant_disable(chip, id);
		chip->variant.constant = 0;
		break;
	case 1:
		ret = pwm_constant_enable(chip, id);
		chip->variant.constant = 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret ? : size;
}

/**
 * pwm_set_times() - set PWM times output toggling
 *					 set pwm a1 and pwm a2 timer together
 *					 and pwm a1 should be set first
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B,range from 1 to 15
 * @value: blink times to set,range from 1 to 255
 */

int pwm_set_times(struct aml_pwm_chip *chip,
						int index, int value)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;
	int val = value;

	if (((val <= 0) && (val > 255)) || ((id < 0) && (id > 15))) {
		dev_err(our_chip->chip.dev,
				"index or value is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_AB, 0xff << 24);
		pwm_write_reg1(our_chip->base + REG_TIME_AB, val << 24);
		break;
	case PWM_B:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_AB, 0xff << 8);
		pwm_write_reg1(our_chip->base + REG_TIME_AB, val << 8);
		break;
	case PWM_C:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_CD, 0xff << 24);
		pwm_write_reg1(our_chip->base + REG_TIME_CD, val << 24);
		break;
	case PWM_D:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_CD, 0xff << 8);
		pwm_write_reg1(our_chip->base + REG_TIME_CD, val << 8);
		break;
	case PWM_E:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_EF, 0xff << 24);
		pwm_write_reg1(our_chip->base + REG_TIME_EF, val << 24);
		break;
	case PWM_F:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_EF, 0xff << 8);
		pwm_write_reg1(our_chip->base + REG_TIME_EF, val << 8);
		break;
	case PWM_AO_A:
		pwm_clear_reg_bits(our_chip->ao_base + REG_TIME_AO_AB,
							0xff << 24);
		pwm_write_reg1(our_chip->ao_base + REG_TIME_AO_AB, val << 24);
		break;
	case PWM_AO_B:
		pwm_clear_reg_bits(our_chip->ao_base + REG_TIME_AO_AB,
							0xff << 8);
		pwm_write_reg1(our_chip->ao_base + REG_TIME_AO_AB, val << 8);
		break;
	case PWM_A2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_AB, 0xff << 16);
		pwm_write_reg1(our_chip->base + REG_TIME_AB, val << 16);
		break;
	case PWM_B2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_AB, 0xff);
		pwm_write_reg1(our_chip->base + REG_TIME_AB, val);
		break;
	case PWM_C2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_CD, 0xff << 16);
		pwm_write_reg1(our_chip->base + REG_TIME_CD, val << 16);
		break;
	case PWM_D2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_CD, 0xff);
		pwm_write_reg1(our_chip->base + REG_TIME_CD, val);
		break;
	case PWM_E2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_EF, 0xff << 16);
		pwm_write_reg1(our_chip->base + REG_TIME_EF, val << 16);
		break;
	case PWM_F2:
		pwm_clear_reg_bits(our_chip->base + REG_TIME_EF, 0xff);
		pwm_write_reg1(our_chip->base + REG_TIME_EF, val);
		break;
	case PWM_AO_A2:
		pwm_clear_reg_bits(our_chip->ao_base + REG_TIME_AO_AB,
							0xff << 16);
		pwm_write_reg1(our_chip->ao_base + REG_TIME_AO_AB, val << 16);
		break;
	case PWM_AO_B2:
		pwm_clear_reg_bits(our_chip->ao_base + REG_TIME_AO_AB, 0xff);
		pwm_write_reg1(our_chip->ao_base + REG_TIME_AO_AB, val);
		break;
	default:
	break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_set_times);

static ssize_t pwm_times_show(struct device *child,
			       struct device_attribute *attr, char *buf)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	return sprintf(buf, "%d\n", chip->variant.times);
}

static ssize_t pwm_times_store(struct device *child,
				  struct device_attribute *attr,
				  const char *buf, size_t size)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	int val, ret , id , res;

	res = sscanf(buf, "%d %d", &val, &id);
	if (res != 2) {
		dev_err(child,
		"Can't parse pwm id and value,usage:[value index]\n");
		return -EINVAL;
	}
	if (((val <= 0) && (val > 255)) || ((id < 0) && (id > 15))) {
		dev_err(chip->chip.dev,
		"index or value is not within the scope!\n");
		return -EINVAL;
	}

	ret = pwm_set_times(chip, id, val);
	chip->variant.times = val;

	return ret ? : size;
}

/**
 * pwm_blink_enable()
 *	- start a blink PWM output toggling
 *	  txl only support 8 channel blink output
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B
 */
int pwm_blink_enable(struct aml_pwm_chip *chip, int index)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;

	if ((id < 0) && (id > 7)) {
		dev_err(our_chip->chip.dev, "index is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_AB,
						(1 << 8), (1 << 8));
		break;
	case PWM_B:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_AB,
						(1 << 9), (1 << 9));
		break;
	case PWM_C:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_CD,
						(1 << 8), (1 << 8));
		break;
	case PWM_D:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_CD,
						(1 << 9), (1 << 9));
		break;
	case PWM_E:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_EF,
						(1 << 8), (1 << 8));
		break;
	case PWM_F:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_EF,
						(1 << 9), (1 << 9));
		break;
	case PWM_AO_A:
		pwm_set_reg_bits(our_chip->ao_blink_base + REG_BLINK_AO_AB,
						(1 << 8), (1 << 8));
		break;
	case PWM_AO_B:
		pwm_set_reg_bits(our_chip->ao_blink_base + REG_BLINK_AO_AB,
						(1 << 9), (1 << 9));
		break;
	default:
	break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_blink_enable);

/**
 * pwm_blink_disable() - stop a constant PWM output toggling
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B
 */
int pwm_blink_disable(struct aml_pwm_chip *chip , int index)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;

	if ((id < 1) && (id > 7)) {
		dev_err(our_chip->chip.dev, "index is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_AB,
						(1 << 8), (0 << 8));
		break;
	case PWM_B:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_AB,
						(1 << 9), (0 << 9));
		break;
	case PWM_C:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_CD,
						(1 << 8), (0 << 8));
		break;
	case PWM_D:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_CD,
						(1 << 9), (0 << 9));
		break;
	case PWM_E:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_EF,
						(1 << 8), (0 << 8));
		break;
	case PWM_F:
		pwm_set_reg_bits(our_chip->base + REG_BLINK_EF,
						(1 << 9), (0 << 9));
		break;
	case PWM_AO_A:
		pwm_set_reg_bits(our_chip->ao_blink_base + REG_BLINK_AO_AB,
						(1 << 8), (0 << 8));
		break;
	case PWM_AO_B:
		pwm_set_reg_bits(our_chip->ao_blink_base + REG_BLINK_AO_AB,
						(1 << 9), (0 << 9));
		break;
	default:
	break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_blink_disable);

static ssize_t pwm_blink_enable_show(struct device *child,
			       struct device_attribute *attr, char *buf)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	return sprintf(buf, "%d\n", chip->variant.blink_enable);
}

static ssize_t pwm_blink_enable_store(struct device *child,
				  struct device_attribute *attr,
				  const char *buf, size_t size)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	int val, ret , id , res;

	res = sscanf(buf, "%d %d", &val, &id);
	if (res != 2) {
		dev_err(child,
		"blink enable,Can't parse pwm id,usage:[value index]\n");
		return -EINVAL;
	}
	if ((id < 1) && (id > 7)) {
		dev_err(chip->chip.dev, "index is not within the scope!\n");
		return -EINVAL;
	}

	switch (val) {
	case 0:
		ret = pwm_blink_disable(chip, id);
		chip->variant.blink_enable = 0;
		break;
	case 1:
		ret = pwm_blink_enable(chip, id);
		chip->variant.blink_enable = 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret ? : size;

}

/**
 * pwm_set_blink_times() - set PWM blink times output toggling
 * @chip: aml_pwm_chip struct
 * @index: pwm channel to choose,like PWM_A or PWM_B
 * @value: blink times to set,range from 1 to 15
 */
int pwm_set_blink_times(struct aml_pwm_chip *chip,
								int index,
								int value)
{
	struct aml_pwm_chip *our_chip = chip;
	int id = index;
	int val = value;

	if (((val <= 0) && (val > 15)) || ((id < 1) && (id > 7))) {
		dev_err(our_chip->chip.dev,
		"value or index is not within the scope!\n");
		return -EINVAL;
	}

	switch (id) {
	case PWM_A:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_AB, 0xf);
		pwm_write_reg(our_chip->base + REG_BLINK_AB, val);
		break;
	case PWM_B:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_AB, 0xf << 4);
		pwm_write_reg(our_chip->base + REG_BLINK_AB, val<<4);
		break;
	case PWM_C:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_CD, 0xf);
		pwm_write_reg(our_chip->base + REG_BLINK_CD, val);
		break;
	case PWM_D:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_CD, 0xf << 4);
		pwm_write_reg(our_chip->base + REG_BLINK_CD, val<<4);
		break;
	case PWM_E:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_EF, 0xf);
		pwm_write_reg(our_chip->base + REG_BLINK_EF, val);
		break;
	case PWM_F:
		pwm_clear_reg_bits(our_chip->base + REG_BLINK_EF, 0xf << 4);

		pwm_write_reg(our_chip->base + REG_BLINK_EF, val<<4);
		break;
	case PWM_AO_A:
		pwm_clear_reg_bits(our_chip->ao_blink_base +
							REG_BLINK_AO_AB, 0xf);
		pwm_write_reg(our_chip->ao_blink_base +
						REG_BLINK_AO_AB, val);
		break;
	case PWM_AO_B:
		pwm_clear_reg_bits(our_chip->ao_blink_base + REG_BLINK_AO_AB,
							0xf << 4);
		pwm_write_reg(our_chip->ao_blink_base + REG_BLINK_AO_AB,
							val<<4);
		break;
	default:
	break;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(pwm_set_blink_times);

static ssize_t pwm_blink_times_show(struct device *child,
			       struct device_attribute *attr, char *buf)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	return sprintf(buf, "%d\n", chip->variant.blink_times);
}

static ssize_t pwm_blink_times_store(struct device *child,
				  struct device_attribute *attr,
				  const char *buf, size_t size)
{
	struct aml_pwm_chip *chip =
		(struct aml_pwm_chip *)dev_get_drvdata(child);
	int val, ret , id , res;

	res = sscanf(buf, "%d %d", &val, &id);
	if (res != 2) {
		dev_err(child,
		"Can't parse pwm id and value,usage:[value index]\n");
		return -EINVAL;
	}
	if (((val <= 0) && (val > 15)) || ((id < 0) && (id > 7))) {
		dev_err(chip->chip.dev,
		"value or index is not within the scope!\n");
		return -EINVAL;
	}

	ret = pwm_set_blink_times(chip, id, val);
	chip->variant.blink_times = val;

	return ret ? : size;
}


static DEVICE_ATTR(constant, 0644,
						pwm_constant_show,
						pwm_constant_store);
static DEVICE_ATTR(times, 0644,
						pwm_times_show,
						pwm_times_store);
static DEVICE_ATTR(blink_enable, 0644,
						pwm_blink_enable_show,
						pwm_blink_enable_store);
static DEVICE_ATTR(blink_times, 0644,
						pwm_blink_times_show,
						pwm_blink_times_store);

static struct attribute *pwm_attrs[] = {
		&dev_attr_constant.attr,
		&dev_attr_times.attr,
		&dev_attr_blink_enable.attr,
		&dev_attr_blink_times.attr,
		NULL,
};

static struct attribute_group pwm_attr_group = {
		.attrs = pwm_attrs,
};

int meson_pwm_sysfs_init(struct device *dev)
{
	int retval;
	retval = sysfs_create_group(&dev->kobj, &pwm_attr_group);
	return retval;
}
