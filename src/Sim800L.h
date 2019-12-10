#ifndef _sim800l__h_
#define _sim800l__h_

bool sim800Init(void);
String getSim800(void);
bool balanceInc(String code);
long balanceCheck(void);
bool smsSend(String num, String msg);

#endif
