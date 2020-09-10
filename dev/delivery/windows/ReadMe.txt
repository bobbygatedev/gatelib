26/02/2015 glib_compatibility 2.1.000

Description:
Introduction of multithread(g::mthread) functionalities.

Installation Instructions(Windows):

unzip your .zip into a your target directory 
• launch GLibInstaller.exe ( will set automatically env vars and Visual studio 2005 config ) 
• reboot your PC 
• if your Visual Studio is compatible with 2005 project format you could generate a project using these settings:
	Include dirs $(G_INCLUDE)\common;$(G_INCLUDE)\Win32 
	Library dirs $(G_LIB) 
	Libraries:
		debug(static) glib_compatibilityd.lib 
		debug(dll) glib_compatibilityDllD.lib 
		relaese(dll) glib_compatibilityDll.lib 
		release(static) glib_compatibilityd.lib 

Package description: 
• /bin            constains binaries(.dll,.jar) 
• /include/common contains includes 
• /include/Win32  contains includes 
• /lib            contains lib(s) 
• /samples        contains template for project under Visual Studio 2005 and gcc. 

