export VERSION=2.1
echo installing gatelib.compatibility.$VERSION

export GFOLDER=/opt/gatelib/$VERSION
export GCURRENT=/opt/gatelib/current

echo "Version folder $GFOLDER" 

echo "creatings folder"
mkdir -p $GFOLDER

cp -r ./include $GFOLDER
cp -r ./lib $GFOLDER
cp -r ./samples $GFOLDER
cp -r ./bin $GFOLDER

echo "settings permission for all files and folder"
find $GFOLDER -type d -links 2 -exec chmod -v 755 {} \;
find $GFOLDER -type f -exec chmod -v 444 {} \;

echo copying env var bash file 
cp -f gatelib.sh /etc/profile.d
chmod -v -R 555 /etc/profile.d/gatelib.sh

echo "copying installation files"
cp -f usethisversion.sh $GFOLDER
cp -f ReadMe.txt        $GFOLDER
cp -f uninstall.sh      $GFOLDER

echo "setting permission in access for all"
find $GFOLDER/bin -type f -exec chmod -v 555 {} \;

ln -s $GFOLDER/bin/libglib_compatibility.so  /usr/lib/libglib_compatibility.so
ln -s $GFOLDER/bin/libglib_compatibilityD.so /usr/lib/libglib_compatibilityD.so
ln -s $GFOLDER $GCURRENT



 
