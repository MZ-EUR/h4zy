// dllmain.h : Declaration of module class.

class CAdwareModule : public CAtlDllModuleT< CAdwareModule >
{
public :
	DECLARE_LIBID(LIBID_AdwareLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ADWARE, "{D2A47394-D6E9-49E8-BDBA-0FDEF574DE8C}")
};

extern class CAdwareModule _AtlModule;
