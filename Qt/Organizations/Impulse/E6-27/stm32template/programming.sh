#!/bin/bash

openocd -f interface/stlink-v1.cfg -f board/stm32vldiscovery.cfg -c "program stm32template.hex verify reset"
