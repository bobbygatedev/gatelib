export VERSION=2.1
echo uninstalling gatelib.compatibility.$VERSION

export GFOLDER=/opt/gatelib/$VERSION
export GCURRENT=/opt/gatelib/current

echo removing version files
rm -f -r $GCURRENT
rm -f -r $GFOLDER
rm -f /usr/lib/libglib_compatibility.so
rm -f /usr/lib/libglib_compatibilityD.so

echo removing env var init script from /etc/profile.d 
rm -f /etc/profile.d/gatelib.sh