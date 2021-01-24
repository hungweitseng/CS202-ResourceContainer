#
# By default, the build is done against the running linux kernel source.
# To build against a different kernel source tree, set SYSSRC:
#
#    make SYSSRC=/path/to/kernel/source
CONFIG_MODULE_SIG=n

ifdef SYSSRC
 KERNEL_SOURCES	 = $(SYSSRC) $(SYSSRC)/src
else
 KERNEL_UNAME	:= $(shell uname -r)
 KERNEL_SOURCES	 = /lib/modules/$(KERNEL_UNAME)/build
endif


default: modules
.PHONY: default
install: modules_install
	cp resource_container-blacklist.conf /etc/modprobe.d
#	cp 80-resource_container.rules /etc/udev/rules.d
	mkdir -p /usr/local/include/resource_container/
	cp include/resource_container.h /usr/local/include/resource_container/
	update-initramfs -u
#	cp resource_container.ko /lib/modules/$(KERNEL_UNAME)/

.PHONY: install


.PHONY:


%::
	$(MAKE) -C $(KERNEL_SOURCES) \
        M=$$PWD $@
