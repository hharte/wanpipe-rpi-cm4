# Sangoma Wanpipe Driver for the Raspberry Pi CM4


## Kernel 5.15.56-v7l+


# Overview

The Raspberry Pi Compute Module 4 (CM4) with I/O board provides a single PCIe slot for an expansion card.  This project adds a T1 interface to the Raspberry Pi using a [Sangoma AFT-series card](https://www.sangoma.com/telephony-cards/sd-wan/).  Cards tested include the A101 (single-port T1/E1) and A108 (octal-port T1/E1.)

The Sangoma T1 cards use the [Wanpipe kernel driver](https://wiki.freepbx.org/pages/viewpage.action?pageId=57409587) to provide a [DAHDI](https://wiki.asterisk.org/wiki/display/DAHDI/DAHDI) interface to [Asterisk](https://www.asterisk.org/).  The Wanpipe driver required some small changes to work for the Raspberry Pi CM4, using the 32-bit kernel.  This driver does not currently support the 64-bit kernel on the CM4.


# Challenges


## PCIe Interrupts

The Wanpipe driver follows the older method of probing for PCI devices ([reference](http://www.makelinux.net/ldd3/chp-12-sect-1.shtml)) where it traverses the list of PCI devices in the system, looking for a card.  Using this method, the card’s interrupt was not properly allocated on the Raspberry Pi.  In comparison, this works fine on x86 machines.

The fix is to call `pci_assign_irq()` from the Wanpipe driver.  As the `pci_assign_irq()` symbol is not exported by the Linux kernel, it needs to be called via a function pointer, the address of which is listed in `/proc/kallsyms` on the running Raspberry Pi.

For this reason, the Wanpipe driver must be compiled directly on the Raspberry Pi, and will need to be recompiled whenever the Raspberry Pi’s kernel is updated.


## Unaligned Memory Accesses

The Wanpipe driver makes use of packed data structures in a couple of cases, causing data structures to be unaligned in memory.  This is not much of an issue on x86, as x86 processors handle unaligned memory accesses relatively well.  On ARM, this leads to crashes in various places in the driver.  The least invasive change to solve this was simply not to pack the data structures.  The Wanpipe makes extensive use of [Linux kernel bit operations](https://www.kernel.org/doc/htmldocs/kernel-api/ch02s03.html) which must operate on an aligned `unsigned long` data type.  This is mostly ok for 32-bit ARM, but the driver will need a lot of work if a 64-bit ARM kernel is to be supported.  


## 32-Bit DMA Support

The hardware only supports 32-bit DMA, so a device tree overlay is needed for the CM4 to force 32-bit DMA.  Add the following to `/boot/config.txt`:


```
# Force PCIe config to support 32bit DMA addresses at the expense of
# having to bounce buffers.
dtoverlay=pcie-32bit-dma
```



# Compiling Wanpipe

Compile the Wanpipe driver directly on the Raspberry Pi CM4, using the Setup script provided by Sangoma.


```
# ./Setup dahdi --silent
```


After the driver is compiled and installed by the Setup script, run `wancfg` to configure the T1 spans.  After configuration, run `wanrouter start` to load the drivers.

