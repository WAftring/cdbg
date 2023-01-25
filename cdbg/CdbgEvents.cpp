#include "CdbgEvents.hpp"

STDMETHODIMP CdbgEvents::QueryInterface(
    THIS_
    _In_ REFIID InterfaceId,
    _Out_ PVOID* Interface
)
{
    *Interface = NULL;

    if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
        IsEqualIID(InterfaceId, __uuidof(IDebugEventCallbacks)))
    {
        *Interface = (IDebugEventCallbacks*)this;
        AddRef();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}
STDMETHODIMP_(ULONG) CdbgEvents::AddRef(THIS)
{
    return 1;
}
STDMETHODIMP_(ULONG) CdbgEvents::Release(THIS)
{
    return 0;
}
STDMETHODIMP CdbgEvents::ChangeEngineState(ULONG Flags, ULONG64 Args) { return S_OK; }
STDMETHODIMP CdbgEvents::SessionStatus(ULONG Status)
{
    return S_OK;
}
STDMETHODIMP CdbgEvents::GetInterestMask(PULONG Mask)
{
    *Mask = DEBUG_EVENT_SESSION_STATUS | DEBUG_EVENT_LOAD_MODULE;
    return S_OK;
}

STDMETHODIMP CdbgEvents::Breakpoint(PDEBUG_BREAKPOINT2 bp) { return S_OK; }
STDMETHODIMP CdbgEvents::ChangeDebuggeeState(ULONG Flags, ULONG64 Argument) { return S_OK; }
STDMETHODIMP CdbgEvents::ChangeSymbolState(ULONG Flags, ULONG64 Argument) { return S_OK; }
STDMETHODIMP CdbgEvents::CreateProcessW(ULONG64 ImageFileHandle, ULONG64 Handle,
    ULONG64 BaseOffset,
    ULONG   ModuleSize,
    PCWSTR   ModuleName,
    PCWSTR   ImageName,
    ULONG   CheckSum,
    ULONG   TimeDateStamp,
    ULONG64 InitialThreadHandle,
    ULONG64 ThreadDataOffset,
    ULONG64 StartOffset) {
    return S_OK;
}

STDMETHODIMP CdbgEvents::CreateThread(ULONG64 Handle,
    ULONG64 DataOffset,
    ULONG64 StartOffset) {
    return S_OK;
}

STDMETHODIMP CdbgEvents::Exception(EXCEPTION_RECORD64* Exception, ULONG FirstChance) { return S_OK; }
STDMETHODIMP CdbgEvents::ExitProcess(ULONG ExitCode) { return S_OK; }
STDMETHODIMP CdbgEvents::ExitThread(ULONG ExitCode) { return S_OK; }
STDMETHODIMP CdbgEvents::LoadModule(ULONG64 ImageFileHandle,
    ULONG64 BaseOffset,
    ULONG   ModuleSize,
    PCWSTR   ModuleName,
    PCWSTR   ImageName,
    ULONG   CheckSum,
    ULONG   TimeDateStamp)
{
    puts("Module loaded");
    return S_OK;
}
STDMETHODIMP CdbgEvents::SystemError(ULONG Error, ULONG Level) { return S_OK; }
STDMETHODIMP CdbgEvents::UnloadModule(PCWSTR   ImageBaseName, ULONG64 BaseOffset) { return S_OK; }


