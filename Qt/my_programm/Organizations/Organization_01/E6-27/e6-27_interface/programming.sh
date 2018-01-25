#!/bin/bash

#openocd -f interface/stlink-v1.cfg -f board/stm32vldiscovery.cfg -c "program stm32template.hex verify reset"
openocd -f interface/jlink.cfg -f target/stm32f1x.cfg -c "program e6-27_interface.hex verify reset"
