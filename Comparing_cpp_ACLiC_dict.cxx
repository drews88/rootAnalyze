// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME CcLfIUsersfIswinsfIDownloadsfISchoolsP23mI24fICernfIanalyzeRootfIComparing_cpp_ACLiC_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "C:/Users/swins/Downloads/School 23-24/Cern/analyzeRoot/Comparing.cpp"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_Comparing_cpp_ACLiC_dict_Impl() {
    static const char* headers[] = {
"Comparing.cpp",
nullptr
    };
    static const char* includePaths[] = {
"C:/root_v6.32.06/include",
"C:/root_v6.32.06/include/",
"C:/Users/swins/Downloads/School 23-24/Cern/analyzeRoot/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Comparing_cpp_ACLiC_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
)DICTFWDDCLS"
R"DICTFWDDCLS(#pragma clang diagnostic ignored "-Wignored-attributes"
)DICTFWDDCLS"
R"DICTFWDDCLS(#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
)DICTFWDDCLS"
R"DICTFWDDCLS(extern int __Cling_AutoLoading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Comparing_cpp_ACLiC_dict dictionary payload"

#ifndef R__ACLIC_ROOTMAP
  #define R__ACLIC_ROOTMAP 1
#endif
#ifndef __ACLIC__
  #define __ACLIC__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "Comparing.cpp"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"getData", payloadCode, "@",
"main", payloadCode, "@",
"makeText", payloadCode, "@",
"runAnalysis", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Comparing_cpp_ACLiC_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Comparing_cpp_ACLiC_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Comparing_cpp_ACLiC_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Comparing_cpp_ACLiC_dict() {
  TriggerDictionaryInitialization_Comparing_cpp_ACLiC_dict_Impl();
}