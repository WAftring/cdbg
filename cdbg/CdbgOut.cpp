#include "CdbgOut.hpp"
#include "conwriter.h"
STDMETHODIMP
CdbgOut::QueryInterface(
    THIS_
    _In_ REFIID InterfaceId,
    _Out_ PVOID* Interface
)
{
    *Interface = NULL;

    if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
        IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
    {
        *Interface = (IDebugOutputCallbacks*)this;
        AddRef();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

STDMETHODIMP_(ULONG)
CdbgOut::AddRef(
    THIS
)
{
    // This class is designed to be static so
    // there's no true refcount.
    return 1;
}

STDMETHODIMP_(ULONG)
CdbgOut::Release(
    THIS
)
{
    // This class is designed to be static so
    // there's no true refcount.
    return 0;
}

STDMETHODIMP
CdbgOut::Output(
    THIS_
    _In_ ULONG Mask,
    _In_ PCWSTR Text
)
{
    // TODO(will): This should eventually be more complex
    UNREFERENCED_PARAMETER(Mask);
    CwWrite(Text);
    return S_OK;
}