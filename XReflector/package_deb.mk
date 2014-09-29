# Debian Binary Package Builder, copy to package.mk 
# to use package target. (Need to get VERSION and ARCH for file naming)

package:	install
		rm -rf debian$(BINDIR)
		mkdir -p debian$(BINDIR)
		cp $(BINDIR)/ircddbgateway debian$(BINDIR)
		cp $(BINDIR)/ircddbgatewayd debian$(BINDIR)
		cp $(BINDIR)/remotecontrol debian$(BINDIR)
		cp $(BINDIR)/starnetserver debian$(BINDIR)
		cp $(BINDIR)/starnetserverd debian$(BINDIR)
		cp $(BINDIR)/timercontrol debian$(BINDIR)
		cp $(BINDIR)/timercontrold debian$(BINDIR)
		cp $(BINDIR)/timeserver debian$(BINDIR)
		cp $(BINDIR)/timeserverd debian$(BINDIR)
		cp $(BINDIR)/xreflector debian$(BINDIR)
		cp $(BINDIR)/xreflectord debian$(BINDIR)

		dpkg-deb --build debian
		mv debian.deb ircDDBGateway-$(VERSION)-armel.deb
