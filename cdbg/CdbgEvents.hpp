#pragma once
#include <Windows.h>
#include <DbgEng.h>

#include <stdio.h>

class CdbgEvents : public IDebugEventCallbacksWide
{
public:
    // IUnknown.
    STDMETHOD(QueryInterface)(
        THIS_
        _In_ REFIID InterfaceId,
        _Out_ PVOID* Interface
        );
    STDMETHOD_(ULONG, AddRef)(
        THIS
        );
    STDMETHOD_(ULONG, Release)(
        THIS
        );
    STDMETHOD(ChangeEngineState)(ULONG Flags, ULONG64 Args);
    STDMETHOD(SessionStatus)(ULONG Status);
    STDMETHOD(GetInterestMask)(PULONG Mask);

    STDMETHOD(Breakpoint)(PDEBUG_BREAKPOINT2 bp);
    STDMETHOD(ChangeDebuggeeState)(ULONG Flags, ULONG64 Argument);
    STDMETHOD(ChangeSymbolState)(ULONG Flags, ULONG64 Argument);
    STDMETHOD(CreateProcess)(ULONG64 ImageFileHandle, ULONG64 Handle, 
        ULONG64 BaseOffset, 
        ULONG   ModuleSize, 
        PCWSTR   ModuleName,
        PCWSTR   ImageName,
        ULONG   CheckSum,
        ULONG   TimeDateStamp,
        ULONG64 InitialThreadHandle,
        ULONG64 ThreadDataOffset,
        ULONG64 StartOffset);

    STDMETHOD(CreateThread)(ULONG64 Handle,
        ULONG64 DataOffset,
        ULONG64 StartOffset);
    STDMETHOD(Exception)(THIS_ EXCEPTION_RECORD64* Exception, ULONG FirstChance);
    STDMETHOD(ExitProcess)(ULONG ExitCode);
    STDMETHOD(ExitThread)(ULONG ExitCode);
    STDMETHOD(LoadModule)(ULONG64 ImageFileHandle,
        ULONG64 BaseOffset,
        ULONG   ModuleSize,
        PCWSTR   ModuleName,
        PCWSTR   ImageName,
        ULONG   CheckSum,
        ULONG   TimeDateStamp);
    STDMETHOD(SystemError)(ULONG Error, ULONG Level);
    STDMETHOD(UnloadModule)(PCWSTR ImageBaseName, ULONG64 BaseOffset);
};

