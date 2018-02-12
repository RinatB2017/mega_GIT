Name:           gammaray-qt4
Version:        2.9.0
Release:        1
Summary:        An introspection tool for Qt applications
Source:         gammaray-%{version}.tar.gz
Url:            https://github.com/KDAB/GammaRay
Group:          Development/Tools
License:        GPL-2.0+
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Vendor:         Klaralvdalens Datakonsult AB (KDAB)
Packager:       Klaralvdalens Datakonsult AB (KDAB) <info@kdab.com>
# this package only contains the Qt4 probe, pull in the client from the Qt5 version
Requires:       gammaray = %{version}

BuildRequires: cmake
%if %{defined suse_version}
BuildRequires: libqt4-devel libQtWebKit-devel libkde4-devel
%endif

%if %{defined fedora}
BuildRequires: qt-devel qtwebkit-devel kdelibs-devel udisks2
%if 0%{?fedora} >= 22
BuildRequires: qca
%endif
%endif


%description
GammaRay is a tool for examining the internals of a Qt application
and to some extent also manipulate it. GammaRay uses injection
methods to hook into an application at runtime and provide access to
a wide variety of interesting information. It provides easy ways of
navigating through the complex internal structures you find in some
Qt frameworks, such as QGraphicsView, model/view, QTextDocument,
state machines and more.

This package only contains the Qt4 probe; pull in the client from gammaray-qt5 package.

Authors:
--------
     The GammaRay Team <gammaray-interest@kdab.com>

%define debug_package %{nil}
%global __debug_install_post %{nil}

%package kde4-plugins
Summary:        GammaRay plug-ins to introspect KDE4 applications
Group:          Development/Tools
Requires:       %{name} = %{version}

%description kde4-plugins
Plug-ins for the GammaRay introspection tool to debug KDE4 applications,
such as a KJob tracker.

%prep
%setup -q -n gammaray-%{version}

%build
%if %{defined fedora}
%cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DGAMMARAY_PROBE_ONLY_BUILD=ON
%else
%if "%{_lib}"=="lib64"
cmake . -DLIB_SUFFIX=64 -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DGAMMARAY_PROBE_ONLY_BUILD=ON
%else
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DGAMMARAY_PROBE_ONLY_BUILD=ON
%endif
%endif
%__make %{?_smp_mflags}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%install
%make_install

%clean
%__rm -rf "%{buildroot}"

%files
%defattr(-,root,root)
%{_libdir}/libgammaray_common-*.so.*
%{_libdir}/libgammaray_core-*.so.*
%{_libdir}/libgammaray_ui-*.so.*
%{_libdir}/libgammaray_kitemmodels*
%dir %{_libdir}/gammaray/
%dir %{_libdir}/gammaray/*/
%dir %{_libdir}/gammaray/*/*/
%{_libdir}/gammaray/*/*/libgammaray_widget_export_actions.so
%{_libdir}/gammaray/*/*/gammaray_probe.so
%{_libdir}/gammaray/*/*/gammaray_inprocessui.so
%{_libdir}/gammaray/*/*/gammaray_actioninspector*
%{_libdir}/gammaray/*/*/gammaray_codecbrowser*
%{_libdir}/gammaray/*/*/gammaray_fontbrowser*
%{_libdir}/gammaray/*/*/gammaray_guisupport*
%{_libdir}/gammaray/*/*/gammaray_localeinspector*
%{_libdir}/gammaray/*/*/gammaray_modelinspector*
%{_libdir}/gammaray/*/*/gammaray_objectvisualizer*
%{_libdir}/gammaray/*/*/gammaray_sceneinspector*
%{_libdir}/gammaray/*/*/gammaray_scriptenginedebugger*
%{_libdir}/gammaray/*/*/gammaray_signalmonitor*
%{_libdir}/gammaray/*/*/gammaray_statemachineviewer*
%{_libdir}/gammaray/*/*/gammaray_styleinspector*
%{_libdir}/gammaray/*/*/gammaray_textdocumentinspector*
%{_libdir}/gammaray/*/*/gammaray_timertop*
%{_libdir}/gammaray/*/*/gammaray_widgetinspector*
%exclude %{_libdir}/*.so
%exclude %{_prefix}/share/icons/hicolor/*

%files kde4-plugins
%defattr(-,root,root)
%{_libdir}/gammaray/*/*/gammaray_kjobtracker*

%changelog
* Wed Feb 07 2018 Allen Winter <allen.winter@kdab.com> 2.9.0
  2.9.0 final
* Tue Sep 05 2017 Allen Winter <allen.winter@kdab.com> 2.8.1
  2.8.1 bugfix release
* Wed Jun 07 2017 Allen Winter <allen.winter@kdab.com> 2.8.0
  2.8.0 final
* Tue Feb 14 2017 Allen Winter <allen.winter@kdab.com> 2.7.0
  2.7.0 final
* Thu Sep 29 2016 Allen Winter <allen.winter@kdab.com> 2.6.0
  2.6.0 final
* Wed Jul 27 2016 Allen Winter <allen.winter@kdab.com> 2.5.1
  2.5.1 bugfix release
* Fri Jul 08 2016 Allen Winter <allen.winter@kdab.com> 2.5.0
  2.5.0 final
* Fri Mar 11 2016 Allen Winter <allen.winter@kdab.com> 2.4.1
  2.4.1 final
* Tue Dec 22 2015 Allen Winter <allen.winter@kdab.com> 2.4.0
  2.4.0 final
* Sun Dec 20 2015 Allen Winter <allen.winter@kdab.com> 2.3.99
  2.4.0 release candidate 1
* Fri Jul 10 2015 Allen Winter <allen.winter@kdab.com> 2.3.0
  2.3.0 final
* Tue Jan 27 2015 Allen Winter <allen.winter@kdab.com> 2.2.1
  2.2.1 final
* Fri Oct 31 2014 Allen Winter <allen.winter@kdab.com> 2.2.0
  2.2.0 final
* Thu Oct 23 2014 Allen Winter <allen.winter@kdab.com> 2.1.2
  2.1.2 final
* Sat Aug 30 2014 Allen Winter <allen.winter@kdab.com> 2.1.1
  2.1.1 final
* Fri Jun 27 2014 Allen Winter <allen.winter@kdab.com> 2.1.0
  2.1.0 final
* Thu Jun 26 2014 Allen Winter <allen.winter@kdab.com> 2.0.98
  2.1.0 release candidate 1
* Fri Apr 18 2014 Allen Winter <allen.winter@kdab.com> 2.0.2
  2.0.2 final
* Fri Feb 28 2014 Allen Winter <allen.winter@kdab.com> 2.0.1
  2.0.1 final
* Fri Jan 17 2014 Allen Winter <allen.winter@kdab.com> 2.0.0
  2.0.0 final
* Sat Jan 11 2014 Allen Winter <allen.winter@kdab.com> 1.9.96
  Second 2.0 beta release
* Fri Dec 20 2013 Allen Winter <allen.winter@kdab.com> 1.9.95
  First 2.0 beta release
* Thu Oct 03 2013 Allen Winter <allen.winter@kdab.com> 1.3.2
  Second 1.3 bugfix release
* Tue Apr 30 2013 Allen Winter <allen.winter@kdab.com> 1.3.1
  First 1.3 bugfix release
* Sun Jan 27 2013 Allen Winter <allen.winter@kdab.com> 1.3.0
  Update to latest release
* Wed Jun 27 2012 Allen Winter <allen.winter@kdab.com> 1.1.98
- Rename version to use all integers
* Tue Jun 26 2012 Allen Winter <allen.winter@kdab.com> 1.2beta
- Update to beta release
* Wed Jun 20 2012 Allen Winter <allen.winter@kdab.com> 1.2prebeta
- Update to prebeta release
* Tue Jan 10 2012 Kevin Ottens <kevin.ottens@kdab.com> 1.1.0
- Update to latest release
* Sun Jan  8 2012 Kevin Ottens <kevin.ottens@kdab.com> 1.0.98.20120108
- Grabbing a new snapshot
* Wed Dec 21 2011 Kevin Ottens <kevin.ottens@kdab.com> 1.0.1
- First packaging
