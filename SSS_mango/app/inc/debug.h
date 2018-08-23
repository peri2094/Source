#ifndef DEBUG_H_
#define DEBUG_H_

#include "hw_config.h"

//#define DEBUG_MSG_LEVEL0
//#define DEBUG_MSG_LEVEL1

#define DEBUG_MSG_FUNC_START            (DebugMsg_FunctionStart(__FUNCTION__))
#define DEBUG_MSG_FUNC_END              (DebugMsg_FunctionEnd(__FUNCTION__))
#define DEBUG_MSG_FUNC_NOTIFY           (DebugMsg_FunctionNotify(__FUNCTION__, __LINE__))
#define DEBUG_MSG_FUNC_ERROR            (DebugMsg_FunctionError(__FUNCTION__, __LINE__))

#ifdef USE_FULL_ASSERT
#define DEBUG_MSG_ASSERT                (DebugMsg_Assert(__FUNCTION__, __LINE__))
#else
#define DEBUG_MSG_ASSERT                ((void)0)
#endif /*USE_FULL_ASSERT*/


///////////////////////////////////////////

void assert_failed(uint8_t* file, uint32_t line);
void DebugMsg_FunctionStart(char const* fName_p);
void DebugMsg_FunctionEnd(char const* fName_p);
void DebugMsg_FunctionNotify(char const* fName_p, int lineNum);
void DebugMsg_FunctionError(char const* fName_p, int lineNum);
void DebugMsg_Assert(char const* fName_p, int lineNum);


#endif