
RegisterServiceps.dll: dlldata.obj RegisterService_p.obj RegisterService_i.obj
	link /dll /out:RegisterServiceps.dll /def:RegisterServiceps.def /entry:DllMain dlldata.obj RegisterService_p.obj RegisterService_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del RegisterServiceps.dll
	@del RegisterServiceps.lib
	@del RegisterServiceps.exp
	@del dlldata.obj
	@del RegisterService_p.obj
	@del RegisterService_i.obj
