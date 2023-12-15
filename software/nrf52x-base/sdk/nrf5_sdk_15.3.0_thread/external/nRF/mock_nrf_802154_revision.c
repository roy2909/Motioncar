/*lint -e???? -save */
/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "mock_nrf_802154_revision.h"

typedef struct _CMOCK_nrf_802154_revision_init_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int CallOrder;

} CMOCK_nrf_802154_revision_init_CALL_INSTANCE;

typedef struct _CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  bool ReturnVal;
  int CallOrder;

} CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE;

static struct mock_nrf_802154_revisionInstance
{
  int nrf_802154_revision_init_IgnoreBool;
  CMOCK_nrf_802154_revision_init_CALLBACK nrf_802154_revision_init_CallbackFunctionPointer;
  int nrf_802154_revision_init_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE nrf_802154_revision_init_CallInstance;
  int nrf_802154_revision_has_phyend_event_IgnoreBool;
  bool nrf_802154_revision_has_phyend_event_FinalReturn;
  CMOCK_nrf_802154_revision_has_phyend_event_CALLBACK nrf_802154_revision_has_phyend_event_CallbackFunctionPointer;
  int nrf_802154_revision_has_phyend_event_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE nrf_802154_revision_has_phyend_event_CallInstance;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void mock_nrf_802154_revision_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.nrf_802154_revision_init_CallInstance, cmock_line, "Function 'nrf_802154_revision_init' called less times than expected.");
  if (Mock.nrf_802154_revision_init_CallbackFunctionPointer != NULL)
    Mock.nrf_802154_revision_init_CallInstance = CMOCK_GUTS_NONE;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.nrf_802154_revision_has_phyend_event_CallInstance, cmock_line, "Function 'nrf_802154_revision_has_phyend_event' called less times than expected.");
  if (Mock.nrf_802154_revision_has_phyend_event_CallbackFunctionPointer != NULL)
    Mock.nrf_802154_revision_has_phyend_event_CallInstance = CMOCK_GUTS_NONE;
}

void mock_nrf_802154_revision_Init(void)
{
  mock_nrf_802154_revision_Destroy();
}

void mock_nrf_802154_revision_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  Mock.nrf_802154_revision_init_CallbackFunctionPointer = NULL;
  Mock.nrf_802154_revision_init_CallbackCalls = 0;
  Mock.nrf_802154_revision_has_phyend_event_CallbackFunctionPointer = NULL;
  Mock.nrf_802154_revision_has_phyend_event_CallbackCalls = 0;
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

void nrf_802154_revision_init(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_nrf_802154_revision_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_nrf_802154_revision_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.nrf_802154_revision_init_CallInstance);
  Mock.nrf_802154_revision_init_CallInstance = CMock_Guts_MemNext(Mock.nrf_802154_revision_init_CallInstance);
  if (Mock.nrf_802154_revision_init_IgnoreBool)
  {
    return;
  }
  if (Mock.nrf_802154_revision_init_CallbackFunctionPointer != NULL)
  {
    Mock.nrf_802154_revision_init_CallbackFunctionPointer(Mock.nrf_802154_revision_init_CallbackCalls++);
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'nrf_802154_revision_init' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'nrf_802154_revision_init' called earlier than expected.");
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'nrf_802154_revision_init' called later than expected.");
}

void nrf_802154_revision_init_CMockIgnore(void)
{
  Mock.nrf_802154_revision_init_IgnoreBool = (int)1;
}

void nrf_802154_revision_init_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_nrf_802154_revision_init_CALL_INSTANCE));
  CMOCK_nrf_802154_revision_init_CALL_INSTANCE* cmock_call_instance = (CMOCK_nrf_802154_revision_init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.nrf_802154_revision_init_CallInstance = CMock_Guts_MemChain(Mock.nrf_802154_revision_init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
}

void nrf_802154_revision_init_StubWithCallback(CMOCK_nrf_802154_revision_init_CALLBACK Callback)
{
  Mock.nrf_802154_revision_init_CallbackFunctionPointer = Callback;
}

bool nrf_802154_revision_has_phyend_event(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE* cmock_call_instance = (CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.nrf_802154_revision_has_phyend_event_CallInstance);
  Mock.nrf_802154_revision_has_phyend_event_CallInstance = CMock_Guts_MemNext(Mock.nrf_802154_revision_has_phyend_event_CallInstance);
  if (Mock.nrf_802154_revision_has_phyend_event_IgnoreBool)
  {
    if (cmock_call_instance == NULL)
      return Mock.nrf_802154_revision_has_phyend_event_FinalReturn;
    Mock.nrf_802154_revision_has_phyend_event_FinalReturn = cmock_call_instance->ReturnVal;
    return cmock_call_instance->ReturnVal;
  }
  if (Mock.nrf_802154_revision_has_phyend_event_CallbackFunctionPointer != NULL)
  {
    return Mock.nrf_802154_revision_has_phyend_event_CallbackFunctionPointer(Mock.nrf_802154_revision_has_phyend_event_CallbackCalls++);
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'nrf_802154_revision_has_phyend_event' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'nrf_802154_revision_has_phyend_event' called earlier than expected.");
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, "Function 'nrf_802154_revision_has_phyend_event' called later than expected.");
  return cmock_call_instance->ReturnVal;
}

void nrf_802154_revision_has_phyend_event_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE));
  CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE* cmock_call_instance = (CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.nrf_802154_revision_has_phyend_event_CallInstance = CMock_Guts_MemChain(Mock.nrf_802154_revision_has_phyend_event_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
  Mock.nrf_802154_revision_has_phyend_event_IgnoreBool = (int)1;
}

void nrf_802154_revision_has_phyend_event_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, bool cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE));
  CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE* cmock_call_instance = (CMOCK_nrf_802154_revision_has_phyend_event_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.nrf_802154_revision_has_phyend_event_CallInstance = CMock_Guts_MemChain(Mock.nrf_802154_revision_has_phyend_event_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void nrf_802154_revision_has_phyend_event_StubWithCallback(CMOCK_nrf_802154_revision_has_phyend_event_CALLBACK Callback)
{
  Mock.nrf_802154_revision_has_phyend_event_CallbackFunctionPointer = Callback;
}

/* lint -restore */