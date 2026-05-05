/*
 * Copyright (c) 2024 Swaroop
 * SPDX-License-Identifier: Apache-2.0
 *
 * Board initialization - runs before main()
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int scratch_board_init(void)
{
	printk("Board Initialized\n");
	return 0;
}

/* APPLICATION level runs after kernel+drivers init but before main() */
SYS_INIT(scratch_board_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
