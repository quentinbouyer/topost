%define _topdir /root/topost

%define name topost
%define release 0%{?dist}
%define version 1.1
%define buildroot %{_topdir}/BUILD/

BuildRoot: %{buildroot}
Summary: topost
Name: %{name}
Version: %{version}
Release: %{release}
Group: System
License: GPL
Packager: quentin.bouyer@gmail.com
Requires: glib2

%description
topost show r/w/iops done by client on all ost for one oss

%build
cd %{_sourcedir};
make

%install
rm -fR $RPM_BUILD_ROOT;
mkdir -p $RPM_BUILD_ROOT/usr/bin;
mkdir -p $RPM_BUILD_ROOT/etc;
mkdir -p $RPM_BUILD_ROOT/usr/share/man/man1;
cd %{_sourcedir};
cp topost $RPM_BUILD_ROOT/usr/bin;
cp topost.conf $RPM_BUILD_ROOT/etc;
cp topost.1 $RPM_BUILD_ROOT/usr/share/man/man1;
cp topost.conf.1 $RPM_BUILD_ROOT/usr/share/man/man1;
%clean
rm -rf RPM_BUILD_ROOT/
%files
%attr(755,root,root)
/usr/bin/topost
/usr/share/man/man1/topost.1.gz
/usr/share/man/man1/topost.conf.1.gz
/etc/topost.conf
%dir
