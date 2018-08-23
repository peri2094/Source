#include "debug.h"

void assert_failed(uint8_t* file, uint32_t line)
{
  printf("Wrong parameters value : file %s on line %d\r\n", file, line);
  while(1){}
}

void DebugMsg_FunctionStart(char const* fName_p)
{
#ifdef  DEBUG_MSG_LEVEL0
  printf("%s() S\n", fNAME_p);
#endif
}

void DebugMsg_FunctionEnd(char const* fName_p)
{
#ifdef  DEBUG_MSG_LEVEL0
  printf("%s() E\n", fNAME_p);
#endif
}

void DebugMsg_FunctionNotify(char const* fName_p, int lineNum)
{
#ifdef  DEBUG_MSG_LEVEL0
  printf("%s() %d\n", fNAME_p, lineNum);
#endif
}

void DebugMsg_FunctionError(char const* fName_p, int lineNum)
{
#ifdef  DEBUG_MSG_LEVEL0
  printf("[ERROR] %s() %d\n", fNAME_p, lineNum);
#endif
}

#ifdef USE_FULL_ASSERT
void DebugMsg_Assert(char const* fName_p, int lineNum)
{
  printf("!!! ASSERT FAILED !!! %s() %d\n",fName_p, lineNum);
}

#endif