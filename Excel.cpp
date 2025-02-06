#include "stdafx.h"
#include <fstream>
#include "excel.h"



// AutoWrap() - Automation helper function...
HRESULT AutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs...) {
    // Begin variable-argument list...
    va_list marker;
    va_start(marker, cArgs);

    if(!pDisp) {
        MessageBox(NULL, "NULL IDispatch passed to AutoWrap()", "Error", 0x10010);
        _exit(0);
    }

    // Variables used...
    DISPPARAMS dp = { NULL, NULL, 0, 0 };
    DISPID dispidNamed = DISPID_PROPERTYPUT;
    DISPID dispID;
    HRESULT hr;
    char buf[200];
    char szName[200];

    
    // Convert down to ANSI
    WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);
    
    // Get DISPID for name passed...
    hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
    if(FAILED(hr)) {
        sprintf(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
        MessageBox(NULL, buf, "AutoWrap()", 0x10010);
        _exit(0);
        return hr;
    }
    
    // Allocate memory for arguments...
    VARIANT *pArgs = new VARIANT[cArgs+1];
    // Extract arguments...
    for(int i=0; i<cArgs; i++) {
        pArgs[i] = va_arg(marker, VARIANT);
    }
    
    // Build DISPPARAMS
    dp.cArgs = cArgs;
    dp.rgvarg = pArgs;
    
    // Handle special-case for property-puts!
    if(autoType & DISPATCH_PROPERTYPUT) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispidNamed;
    }
    
    // Make the call!
    hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
    if(FAILED(hr)) {
        sprintf(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
        MessageBox(NULL, buf, "AutoWrap()", 0x10010);
        _exit(0);
        return hr;
    }
    // End variable-argument section...
    va_end(marker);
    
    delete [] pArgs;
    
    return hr;
}
				
int PasarAExcel(int numCol, int numLin, VARIANT arr) 
{

 // Initialize COM for this thread...
   CoInitialize(NULL);

   // Get CLSID for our server...
   CLSID clsid;
   HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);

   if(FAILED(hr)) {

      ::MessageBox(NULL, "CLSIDFromProgID() failed", "Error", 0x10010);
      return -1;
   }

   // Start server and get IDispatch...
   IDispatch *pXlApp;
   hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void **)&pXlApp);
   if(FAILED(hr)) {
      ::MessageBox(NULL, "Excel not registered properly", "Error", 0x10010);
      return -2;
   }

   // Make it visible (i.e. app.visible = 1)
   {

      VARIANT x;
      x.vt = VT_I4;
      x.lVal = 1;
      AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlApp, L"Visible", 1, x);
   }

   // Get Workbooks collection
   IDispatch *pXlBooks;
   {
      VARIANT result;
      VariantInit(&result);
      AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"Workbooks", 0);
      pXlBooks = result.pdispVal;
   }

   // Call Workbooks.Add() to get a new workbook...
   IDispatch *pXlBook;
   {
      VARIANT result;
      VariantInit(&result);
      AutoWrap(DISPATCH_PROPERTYGET, &result, pXlBooks, L"Add", 0);
      pXlBook = result.pdispVal;
   }


   // Get ActiveSheet object
   IDispatch *pXlSheet;
   {
      VARIANT result;
      VariantInit(&result);
      AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"ActiveSheet", 0);
      pXlSheet = result.pdispVal;
   }

   // Get Range object for the Range A1:...
   CString sEnd;
   sEnd.Format("A1:%c%d",'A'+ numCol - 1, numLin);
	
	BSTR bstr = sEnd.AllocSysString ();

   IDispatch *pXlRange;
   {
      VARIANT parm;
      parm.vt = VT_BSTR;
      parm.bstrVal = ::SysAllocString(bstr);

      VARIANT result;
      VariantInit(&result);
      AutoWrap(DISPATCH_PROPERTYGET, &result, pXlSheet, L"Range", 1, parm);
      VariantClear(&parm);

      pXlRange = result.pdispVal;
   }

   // Set range with our safearray...
   AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlRange, L"Value", 1, arr);

 /*
   // Wait for user...
   ::MessageBox(NULL, "All done.", "Notice", 0x10000);

   // Set .Saved property of workbook to TRUE so we aren't prompted
   // to save when we tell Excel to quit...
   {
      VARIANT x;
      x.vt = VT_I4;
      x.lVal = 1;
      AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlBook, L"Saved", 1, x);
   }

   // Tell Excel to quit (i.e. App.Quit)
   AutoWrap(DISPATCH_METHOD, NULL, pXlApp, L"Quit", 0);
*/
   // Release references...
   pXlRange->Release();
   pXlSheet->Release();
   pXlBook->Release();
   pXlBooks->Release();
   pXlApp->Release();
   
   // Uninitialize COM for this thread...
   CoUninitialize();
   return 0;
}

void PasarACsv(CObArray& p_arrStr)
{
    CFileDialog DlgFile(TRUE,
        ".csv",
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Ficheros CSV (*.csv)|*.csv|Todos los ficheros (*.*)|*.*||");

    if (DlgFile.DoModal() == IDCANCEL) return;

    std::ofstream ofs;
    CString sItem;
    CTime Fec;

    ofs.open(DlgFile.GetPathName(), std::ios::out);
    if (ofs.fail())
    {
        AfxMessageBox("No se puede abrir el fichero:" + DlgFile.GetPathName());
    }

    DoCalcFill(ofs, p_arrStr);

    ofs.close();
    AfxMessageBox("Fichero creado");
}

void 
DoCalcFill(std::ofstream& ofs, CObArray& p_sArr)
{
   
    // hacemos los ciclos desde 1 porque hemos insertado así por similitud al Excel
    for (int ii = 0; ii < p_sArr.GetSize(); ii++)
    {
        CStringArray* sAi = (CStringArray*) p_sArr.GetAt(ii);
        for (int jj = 1; jj < sAi->GetSize(); jj++)
        {
            CString sS = sAi->GetAt(jj);
            if (jj > 1) ofs << ";";
            ofs  << sS ;
        }
        ofs << std::endl;
    }

}

