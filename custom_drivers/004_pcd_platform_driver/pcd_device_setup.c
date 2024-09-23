#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

/* 1. Create 2 platform data */
struct pcdev_platform_data pcdev_pdata[4U] = {
    [0U] = {
        .size = 512U,
        .perm = RDWR,
        .serial_number = "PCDEVABC1111",
    },
    [1U] = {
        .size = 1024U,
        .perm = RDWR,
        .serial_number = "PCDEVXYZ2222",
    },
    [2U] = {
	    .size = 128U,
	    .perm = RDONLY,
	    .serial_number = "PCDEXYZ3333",
    },
    [3U] = {
	    .size = 32U,
	    .perm = RDWR,
	    .serial_number = "PCDEVXYZ4444",
    },
};

/* 2. Create 2 platform devices */

struct platform_device platform_pcdev_1 = {
    .name = "pcdev-A1x",
    .id = 0U,
    .dev = {
        .platform_data = &pcdev_pdata[0U],
    }
};

struct platform_device platform_pcdev_2 = {
    .name = "pcdev-B1x",
    .id = 1U,
    .dev = {
        .platform_data = &pcdev_pdata[1U],
    }
};

struct platform_device platform_pcdev_3 = {
	.name = "pcdev-C1x",
     	.id = 2U,
	.dev = {
		.platform_data = &pcdev_pdata[2U],
	}	
};

struct platform_device platform_pcdev_4 = {
	.name = "pcdev-D1x",
	.id = 3U,
	.dev = {
		.platform_data = &pcdev_pdata[3U],
	}
};

struct platform_device *platform_pcdevs[] = 
{
	&platform_pcdev_1,
	&platform_pcdev_2,
	&platform_pcdev_3,
	&platform_pcdev_4,
};

static int __init pcdev_platform_init(void)
{
    /* Register platform divice */
    // platform_device_register(&platform_pcdev_1);
    // platform_device_register(&platform_pcdev_2);
    platform_add_devices(platform_pcdevs, ARRAY_SIZE(platform_pcdevs));
    pr_info("Device setup module loaded\n");
    return 0;
}

static void __exit pcdev_platform_exit(void)
{
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
    platform_device_unregister(&platform_pcdev_3);
    platform_device_unregister(&platform_pcdev_4);
    pr_info("Device setup module unloaded\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers platform devices");
