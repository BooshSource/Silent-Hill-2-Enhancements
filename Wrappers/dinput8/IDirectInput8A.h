#pragma once

class m_IDirectInput8A : public IDirectInput8A, public AddressLookupTableDinput8Object
{
private:
	IDirectInput8A *ProxyInterface;

public:
	m_IDirectInput8A(IDirectInput8A *aOriginal) : ProxyInterface(aOriginal)
	{
		Logging::LogDebug() << "Creating device " << __FUNCTION__ << "(" << this << ")";

		ProxyAddressLookupTableDinput8.SaveAddress(this, ProxyInterface);
	}
	~m_IDirectInput8A()
	{
		Logging::LogDebug() << __FUNCTION__ << "(" << this << ")" << " deleting device!";

		ProxyAddressLookupTableDinput8.DeleteAddress(this);
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirectInput8A methods ***/
	STDMETHOD(CreateDevice)(THIS_ REFGUID, LPDIRECTINPUTDEVICE8A *, LPUNKNOWN);
	STDMETHOD(EnumDevices)(THIS_ DWORD, LPDIENUMDEVICESCALLBACKA, LPVOID, DWORD);
	STDMETHOD(GetDeviceStatus)(THIS_ REFGUID);
	STDMETHOD(RunControlPanel)(THIS_ HWND, DWORD);
	STDMETHOD(Initialize)(THIS_ HINSTANCE, DWORD);
	STDMETHOD(FindDevice)(THIS_ REFGUID, LPCSTR, LPGUID);
	STDMETHOD(EnumDevicesBySemantics)(THIS_ LPCSTR, LPDIACTIONFORMATA, LPDIENUMDEVICESBYSEMANTICSCBA, LPVOID, DWORD);
	STDMETHOD(ConfigureDevices)(THIS_ LPDICONFIGUREDEVICESCALLBACK, LPDICONFIGUREDEVICESPARAMSA, DWORD, LPVOID);
};
