/*
 * altsysid.c -- Altera SYSID driver
 *
 * (C) Copyright 2011, Walter Goossens <waltergoossens@home.nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#define DRV_NAME "altera_sysid"

#define REG_ID		0
#define REG_TIMESTAMP	4

/*
 * Local per-device structure.
 */
struct altera_sysid {
	void __iomem *base;
};

static ssize_t altsysid_show_id(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct altera_sysid *sys = dev_get_drvdata(dev);
	return sprintf(buf, "%u\n", ioread32(sys->base + REG_ID));
}

static ssize_t altsysid_show_timestamp(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	struct altera_sysid *sys = dev_get_drvdata(dev);
	return sprintf(buf, "%u\n", ioread32(sys->base + REG_TIMESTAMP));
}

static DEVICE_ATTR(id, S_IRUGO, altsysid_show_id, NULL);
static DEVICE_ATTR(timestamp, S_IRUGO, altsysid_show_timestamp, NULL);

static struct attribute *altsysid_attrs[] = {
	&dev_attr_id.attr,
	&dev_attr_timestamp.attr,
	NULL
};

static const struct attribute_group altsysid_attr_group = {
	.attrs = altsysid_attrs,
};

static int __devinit altsysid_probe(struct platform_device *pdev)
{
	struct altera_sysid *sys;
	struct tm tstamp;
	struct resource *res;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	sys = devm_kzalloc(&pdev->dev, sizeof(struct altera_sysid), GFP_KERNEL);
	if (!sys)
		return -ENOMEM;

	if (!devm_request_mem_region(&pdev->dev, res->start,
				     resource_size(res), pdev->name)) {
		dev_err(&pdev->dev, "Memory region busy\n");
		return -EBUSY;
	}

	sys->base = devm_ioremap_nocache(&pdev->dev, res->start,
					 resource_size(res));
	if (!sys->base) {
		dev_err(&pdev->dev, "Unable to map registers\n");
		return -EIO;
	}

	platform_set_drvdata(pdev, sys);

	ret = sysfs_create_group(&pdev->dev.kobj, &altsysid_attr_group);
	if (ret)
		return ret;

	time_to_tm(ioread32(sys->base + REG_TIMESTAMP), 0, &tstamp);
	dev_info(&pdev->dev, "System creation hash %08X timestamp "
			"%li-%02i-%02i %02i:%02i:%02i\n",
			ioread32(sys->base + REG_ID), tstamp.tm_year + 1900,
			tstamp.tm_mon + 1, tstamp.tm_mday, tstamp.tm_hour,
			tstamp.tm_min, tstamp.tm_sec);

	return 0;
}

static int __devexit altsysid_remove(struct platform_device *pdev)
{
	sysfs_remove_group(&pdev->dev.kobj, &altsysid_attr_group);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

#ifdef CONFIG_OF
static struct of_device_id altera_sysid_match[] = {
	{
		.compatible = "altr,sysid-1.0",
	},
	{},
}
MODULE_DEVICE_TABLE(of, altera_sysid_match);
#endif /* CONFIG_OF */

static struct platform_driver altera_sysid_platform_driver = {
	.probe	= altsysid_probe,
	.remove	= __devexit_p(altsysid_remove),
	.driver	= {
		.name	= DRV_NAME,
		.owner	= THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table = altera_sysid_match,
#endif
	},
};

module_platform_driver(altera_sysid_platform_driver);

MODULE_DESCRIPTION("Altera sysid driver");
MODULE_AUTHOR("Walter Goossens <waltergoossens@home.nl>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
