echo set current gatelib version as current.

VERSION=2.1

GFOLDER=/opt/gatelib/$VERSION
GCURRENT=/opt/gatelib/current

ln -s $GFOLDER/bin/libglib_compatibility.so  /usr/lib/libglib_compatibility.so
ln -s $GFOLDER/bin/libglib_compatibilityD.so /usr/lib/libglib_compatibilityD.so
ln -s $GFOLDER $GCURRENT

cp uninstall.sh $GFOLDER/.. #/opt/gatelib


 
