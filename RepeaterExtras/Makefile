# Makefile-include
#
# Build the repeater extras software on Linux
#

include settings.mk

all:	DCSGateway/dcsgateway DExtraGateway/dextragateway ParrotController/parrotcontroller

DExtraGateway/dextragateway:	Common/Common.a
		make -C DExtraGateway

DCSGateway/dcsgateway:	Common/Common.a
		make -C DCSGateway

ParrotController/parrotcontroller:	Common/Common.a
		make -C ParrotController

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C Data install
		make -C DCSGateway install
		make -C DExtraGateway install
		make -C ParrotController install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C DCSGateway clean
		make -C DExtraGateway clean
		make -C ParrotController clean

include package.mk
