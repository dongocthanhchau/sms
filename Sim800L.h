#ifndef _sim800l_h_
#define _sim800l_h_

bool sim800Init(void);
String getSim800(void);
bool balanceInc(String code);
long balanceCheck(void);
bool smsSend(String num, String msg);

#endif
