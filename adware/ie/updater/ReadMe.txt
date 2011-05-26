Hi
 
The Program has been made and tested on XP and Windows 7, will work on Vista as well.
 
There are 3 files that need to be added to the setup and all files must go into the installation folder and one file VersionCheck.txt has to go to your Server which will contain the Version Information.. 
 
1) sysserver.ini
Sample File Attached, just change the Values
 
2) VersionCheck.txt has to go to your Server as defined by the ini file above.
Now this file will contain File Version information on every line
File Version is defined as n1,n2,n3,n4 on every line
So The VersionCheck.txt will contain dll file versions as they happen
The BHO Updater will read the last line of the versioncheck.txt and will compare it with the ProductVersion of the BHO File, if they are different the updater will update the BHO
 
-Note : Product Version of BHO is defined in the source code of the BHO in the Resources. There is a attached .png file which shows the version of the dll selected and is 2,2,1,1
So whenever you want to publish a new version you will need to update the version in the source code and add the version to the VersionCheck.txt file onto your Server.
 
3) RegisterService.dll needs to be added to the setup (To the Installation Folder) and Register Server Flag needs to be set in the innoscript file.
 
4) UpdaterSvc.exe needs to be added to the setup (To the Installation Folder).
 
Will send the binaries to vworker, as gmail does not allow binaries.
 
Regards