Steps to be made.
• unzip your delivery.tar.gz into any directory ( 'tar -xvf delivery.tar.gz' ) 
• change dir to delivery 
• takes root prvilileges ('sudo su -' under Debian/Ubuntu) and launch install.sh (./install.sh) or in Ubuntu/Debian sudo ./install.sh 
• reboot your PC 

For no Ubuntu/Debian linux users (and for no Java JDK 7 users).
 
Gatelib uses some java function for message box and exception window.
 If you are not using Ubuntu or Debian, you should check location of java matter, saved in env vars G_JAVA_INCLUDE and G_JAVA_SHARED_OBJECT,
 then update values in following configuaration scripts: 
• [root]/dev/prj/unix/glibenv.sh:
 export G_JAVA_INCLUDE=/usr/lib/jvm/java-7-openjdk-i386/include
 export G_JAVA_SHARED_OBJECT=/usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client/libjvm.so
 

• [root]/delivery/linux/gatelib.sh
 export G_JAVA_INCLUDE=/usr/lib/jvm/java-7-openjdk-i386/include
 export G_JAVA_SHARED_OBJECT=/usr/lib/jvm/java-7-openjdk-i386/jre/lib/i386/client/libjvm.so
 

Package description:
 • /bin constains binaries(.so,.jar) 
• /include/common contains OS common includes 
• /include/unix contains includes specific for unix/linux 
• /lib contains lib(.a) 
• /samples contains template for project under gcc. 
