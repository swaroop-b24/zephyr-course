# Copyright (c) 2024 Swaroop 
# SPDX-License-Identifier: Apache-2.0 
 
if(CONFIG_OUR_DRIVER) 
    zephyr_library() 
    zephyr_library_sources(our_driver.c) 
    zephyr_library_include_directories( 
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include 
    ) 
endif() 
