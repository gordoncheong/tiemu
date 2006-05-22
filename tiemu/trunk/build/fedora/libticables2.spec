%define name	libticables2
%define version %(date +%%Y%%m%%d)
%define release 1
%define my_opt_flags -Os -g -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		LPG (http://lpg.ticalc.org)
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}-%{version}.tar.bz2
Group:		System Environment/Libraries
License:	GPL
BuildRequires:	libusb-devel >= 0.1.10a, glib2-devel >= 2.10.1
Requires:	libusb >= 0.1.10a, glib2 >= 2.10.1
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary:	Library for handling TI link cables
%description
Library for handling TI link cables

%prep
%setup -n libticables

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/etc/udev/rules.d
cat >$RPM_BUILD_ROOT/etc/udev/rules.d/96-libticables.rules <<EOF1
# This file was installed by the libticables2 Fedora package.

# SilverLink
ACTION=="add", SUBSYSTEM=="usb_device", SYSFS{idVendor}=="0451", SYSFS{idProduct}=="e001", RUN+="/lib/udev/libticables-udev.sh \$env{DEVNAME}"
# TI-89 Titanium DirectLink
ACTION=="add", SUBSYSTEM=="usb_device", SYSFS{idVendor}=="0451", SYSFS{idProduct}=="e004", RUN+="/lib/udev/libticables-udev.sh \$env{DEVNAME}"
# TI-84+ DirectLink
ACTION=="add", SUBSYSTEM=="usb_device", SYSFS{idVendor}=="0451", SYSFS{idProduct}=="e008", RUN+="/lib/udev/libticables-udev.sh \$env{DEVNAME}"
EOF1
mkdir -p $RPM_BUILD_ROOT/lib/udev
cat >$RPM_BUILD_ROOT/lib/udev/libticables-udev.sh <<EOF2
#!/bin/sh

# This file was installed by the libticables2 Fedora package.
#
# Sets up newly plugged in USB SilverLink so that the user who owns
# the console according to pam_console can access it from user space
#
# Note that for this script to work, you'll need all of the following:
# a) 96-libticables.rules shipped with the libticables2 Fedora package.
# b) a setup using pam_console creates the respective lock files
#    containing the name of the respective user. You can check for that
#    by executing "echo \`cat /var/{run,lock}/console.lock\`" and 
#    verifying the appropriate user is mentioned somewhere there.
# c) a Linux kernel supporting udev
# d) a recent version of the udev package
#
# Arguments :
# -----------
# \$1=DEVNAME

# New code, using lock files instead of copying /dev/console permissions
# This also works with non-gdm logins (e.g. on a virtual terminal)
# Idea and code from Nalin Dahyabhai <nalin@redhat.com>
if [ -f /var/run/console/console.lock ]
then
    CONSOLEOWNER=\`cat /var/run/console/console.lock\`
elif [ -f /var/run/console.lock ]
then
    CONSOLEOWNER=\`cat /var/run/console.lock\`
elif [ -f /var/lock/console.lock ]
then
    CONSOLEOWNER=\`cat /var/lock/console.lock\`
else
    CONSOLEOWNER=
fi
if [ -n "\$CONSOLEOWNER" ]
then
    chmod 0000 "\$1"
    chown "\$CONSOLEOWNER" "\$1"
    chmod 0600 "\$1"
fi
EOF2
chmod 755 $RPM_BUILD_ROOT/lib/udev/libticables-udev.sh

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/include/tilp2/stdints.h
/usr/include/tilp2/export1.h
/usr/include/tilp2/ticables.h
/usr/include/tilp2/timeout.h
/usr/lib/libticables2.la
/usr/lib/libticables2.so*
/usr/lib/pkgconfig/ticables2.pc
/etc/udev/rules.d/96-libticables.rules
/lib/udev/libticables-udev.sh

%defattr(-,root,root)
%changelog
* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
License now GPL (with exception for TilEm).
Add missing glib2 BuildRequires/Requires.
Now requires libticonv.
Convert hotplug rules to udev rules and add DirectLink DeviceIDs.

* Sat Feb 11 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update setup -n to use the new directory name (libticables, not libticables2).

* Sun Jan 29 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Strip library because -s gets ignored somehow.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Vendor to LPG.

* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update to libticables-2: new package name, updated file list.

* Wed Jun 22 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
SilverLink support (based on Julien Blache's Debian packages and Nalin
Dahyabhai's usbcam script)

* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Copyright to License.
Don't list cabl_def.h twice.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
