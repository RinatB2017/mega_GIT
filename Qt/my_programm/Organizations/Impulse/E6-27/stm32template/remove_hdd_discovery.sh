#!/bin/sh

modprobe -r usb-storage && modprobe usb-storage quirks=483:3744:i
