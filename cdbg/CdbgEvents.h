#pragma once
#include <Windows.h>
#include <DbgEng.h>
class CdbgEvents : public IDebugEventCallbacks
{
    ULONG m_lastStatus;
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
    BOOL EngineRunning() { return m_lastStatus == DEBUG_SESSION_ACTIVE; }
    STDMETHOD(ChangeEngineState)(ULONG Flags, ULONG64 Args) { return S_OK; }
    STDMETHOD(SessionStatus)(ULONG Status)
    {
        m_lastStatus = Status;
        return S_OK;
    }
    STDMETHOD(GetInterestMask)(PULONG Mask)
    {
        *Mask =  DEBUG_EVENT_SESSION_STATUS;
        return S_OK;
    }

    STDMETHOD(Breakpoint)(PDEBUG_BREAKPOINT bp) { return S_OK; }
    STDMETHOD(ChangeDebuggeeState)(ULONG Flags, ULONG64 Argument) { return S_OK; }
    STDMETHOD(ChangeSymbolState)(ULONG Flags, ULONG64 Argument) { return S_OK; }
    STDMETHOD(CreateProcess)(ULONG64 ImageFileHandle, ULONG64 Handle, 
        ULONG64 BaseOffset, 
        ULONG   ModuleSize, 
        PCSTR   ModuleName,
        PCSTR   ImageName,
        ULONG   CheckSum,
        ULONG   TimeDateStamp,
        ULONG64 InitialThreadHandle,
        ULONG64 ThreadDataOffset,
        ULONG64 StartOffset) { return S_OK; }

        STDMETHOD(CreateThread)( ULONG64 Handle,
             ULONG64 DataOffset,
             ULONG64 StartOffset) {
            return S_OK;
        }
        STDMETHOD(Exception)(PEXCEPTION_RECORD64 _Exception,  LONG FirstChance) { return S_OK; }
        STDMETHOD(ExitProcess)(ULONG ExitCode) { return S_OK; }
        STDMETHOD(ExitThread)(ULONG ExitCode) { return S_OK; }
        STDMETHOD(LoadModule)(           ULONG64 ImageFileHandle,
                       ULONG64 BaseOffset,
                       ULONG   ModuleSize,
             PCSTR   ModuleName,
             PCSTR   ImageName,
                       ULONG   CheckSum,
                       ULONG   TimeDateStamp) 
        {
            return S_OK;
        }
        STDMETHOD(SystemError)(ULONG Error, ULONG Level) { return S_OK; }
        STDMETHOD(UnloadModule)(PCSTR   ImageBaseName, ULONG64 BaseOffset) { return S_OK; }
};

