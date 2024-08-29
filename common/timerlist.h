#ifndef _TIMERLIST_H_
#define _TIMERLIST_H_

#include "timernode.h"
#include <list>


class TimerList
{
public:
    TimerList();
    ~TimerList();

    int64_t addTimerNode(TimerCallback_t run, unsigned int  intervalTime, bool isRepeat,void *userdata);
    void timerProcess();
    void deleteTimerNode(int64_t timerID);
    uint32_t  getCurrentTimeMs();
private:
    std::list<TimerNode> m_timerlist;
};
#endif