#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

versiontarget.target = $$VERSION_HEADER
linux {
    versiontarget.commands = ~/bin/auto_inc_version $$VERSION_HEADER
}
win32 {
    versiontarget.commands = C:/bin/auto_inc_version $$VERSION_HEADER
}
versiontarget.depends = Makefile

PRE_TARGETDEPS      += $$VERSION_HEADER
QMAKE_EXTRA_TARGETS += versiontarget

# message(auto_inc_version)
