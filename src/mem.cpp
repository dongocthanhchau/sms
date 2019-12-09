//#include <String.h>
//#define STACK_SIZE 127// 127, 511, 1023, 2047, 4095, ...
//
//static unsigned int cNum=0;
//struct sms
//{
//    String pNum;
//    bool smsStt;
//    
//    void addData(String _pNum, bool _smsStt)
//    {
//        pNum = _pNum;
//        smsStt = _smsStt;    
//    }
//
//};
//sms stack[STACK_SIZE];
//
//void saveData(String pNumber, bool smsStatus)
//{
//    stack[(++cNum&STACK_SIZE)].addData(pNumber, smsStatus);
//}
//bool loadData(String pNumber)
//{
//    unsigned int i=-1;
//    while (pNumber.indexOf(stack[(cNum-(++i))&STACK_SIZE].pNum)<0);
//    return stack[(cNum-i)&STACK_SIZE].smsStt;
//}
