
if ( "`uname -n`" == barrand-centos7.lal.in2p3.fr || "`uname -n`" == barrand-centos7 ) then
  setenv QT_XKB_CONFIG_ROOT /usr/share/X11/xkb
endif

if ( "`uname | grep CYGWIN`" != "" ) then
  set Qt_home=/cygdrive/c/Qt/5.12.3/msvc2017_64
  setenv PATH "${Qt_home}/bin:${PATH}"
endif
