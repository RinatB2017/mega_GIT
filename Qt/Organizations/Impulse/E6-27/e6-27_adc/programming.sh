#!/bin/bash

openocd -f interface/stlink-v1.cfg -f board/stm32vldiscovery.cfg -c "program e6-27_adc.hex verify reset"
