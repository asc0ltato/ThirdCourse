#include <windows.h>
#include <combaseapi.h>
#include <strsafe.h>
#include <initguid.h>
#include "IComTest_h.h"
#include <ComSampleServerGuids.h>
#include <ComSampleServiceGuids.h>
#include <winerror.h>

HRESULT Execute(_In_ const IID &rclsid, _In_ DWORD dwCoInit, _In_ DWORD dwClsContext)
{
    HRESULT hr = CoInitializeEx(NULL, dwCoInit);
    if (SUCCEEDED(hr))
    {
        IComTest *pComTest;
        hr = CoCreateInstance(rclsid,
                              NULL,
                              dwClsContext,
                              IID_PPV_ARGS(&pComTest));
        if (SUCCEEDED(hr))
        {
            LPWSTR pwszWhoAmI;
            hr = pComTest->WhoAmI(&pwszWhoAmI);
            if (SUCCEEDED(hr))
            {
                wprintf(L"%s. Client calling from %s. COM Server running %s.\n", 
                        pwszWhoAmI,
                        (dwCoInit == COINIT_MULTITHREADED) ? L"MTA" : L"STA",
                        (dwClsContext == CLSCTX_INPROC_SERVER) ? L"in-process" : L"out-of-process");
                CoTaskMemFree(pwszWhoAmI);
            }

            pComTest->Release();
        }

        // IZSS
        IZSS* pZss;
        hr = CoCreateInstance(rclsid,
            NULL,
            dwClsContext,
            IID_PPV_ARGS(&pZss));
        if (SUCCEEDED(hr))
        {
            int number = 123;  
            int reversed = 0;
            hr = pZss->reverseNumber(number, &reversed);
            if (SUCCEEDED(hr))
            {
                wprintf(L"IZSS::reverseNumber(%d) returned %d\n", number, reversed);
            }
            else
            {
                wprintf(L"IZSS::reverseNumber failed: 0x%08X\n", hr);
            }
            pZss->Release();
        }
        else
        {
            wprintf(L"QueryInterface for IZSS failed: 0x%08X\n", hr);
        }

        CoUninitialize();
    }

    return hr;
}

int __cdecl main()
{
    Execute(CLSID_CComServerTest,  COINIT_MULTITHREADED,     CLSCTX_INPROC_SERVER);
    Execute(CLSID_CComServerTest,  COINIT_MULTITHREADED,     CLSCTX_LOCAL_SERVER);
    Execute(CLSID_CComServerTest,  COINIT_APARTMENTTHREADED, CLSCTX_INPROC_SERVER);
    Execute(CLSID_CComServerTest,  COINIT_APARTMENTTHREADED, CLSCTX_LOCAL_SERVER);
    Execute(CLSID_CComServiceTest, COINIT_MULTITHREADED,     CLSCTX_LOCAL_SERVER);
    Execute(CLSID_CComServiceTest, COINIT_APARTMENTTHREADED, CLSCTX_LOCAL_SERVER);

    return 0;
}
