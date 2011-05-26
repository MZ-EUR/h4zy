
UpdaterSvcps.dll: dlldata.obj UpdaterSvc_p.obj UpdaterSvc_i.obj
	link /dll /out:UpdaterSvcps.dll /def:UpdaterSvcps.def /entry:DllMain dlldata.obj UpdaterSvc_p.obj UpdaterSvc_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del UpdaterSvcps.dll
	@del UpdaterSvcps.lib
	@del UpdaterSvcps.exp
	@del dlldata.obj
	@del UpdaterSvc_p.obj
	@del UpdaterSvc_i.obj
