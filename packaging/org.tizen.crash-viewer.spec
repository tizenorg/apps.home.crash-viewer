#sbs-git:slp/pkgs/c/crash-viewer crash-viewer 0.1.0 c498a247c76aafb7abd882762ec5f4fcea7f62e8
Name:       org.tizen.crash-viewer
Summary:    Crash-viewer (EFL)
Version: 0.1.1
Release:    3
Group:      misc
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(appcore-efl)
BuildRequires:  pkgconfig(ecore-input)
BuildRequires:  pkgconfig(ecore-x)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(evas)
BuildRequires:  pkgconfig(edje)
BuildRequires:  pkgconfig(utilX)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(libcurl)
BuildRequires:  pkgconfig(iniparser)
BuildRequires:  pkgconfig(efl-assist)
BuildRequires:  cmake
BuildRequires:  edje-tools
BuildRequires:  gettext-devel
Requires(post): sys-assert

%description
crash-viewer

%prep
%setup -q

%build
%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS+=" -DTIZEN_ENGINEER_MODE"
%endif
export CFLAGS+=" -Werror"
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE.APLv2 %{buildroot}/usr/share/license/%{name}

%post

%postun


%files
%manifest org.tizen.crash-viewer.manifest
%defattr(-,root,root,-)
/etc/smack/accesses.d/org.tizen.crash-viewer.efl
/usr/share/packages/org.tizen.crash-viewer.xml
/usr/apps/org.tizen.crash-viewer/bin/crash-viewer
/usr/apps/org.tizen.crash-viewer/res/edje/crash-viewer.edj
/usr/apps/org.tizen.crash-viewer/res/locale/*/LC_MESSAGES/crash-viewer.mo
/usr/share/license/%{name}
