#ifndef _TIMERNODE_H_
#define _TIMERNODE_H_

#include <stdint.h>


typedef void (*TimerCallback_t)(void* userData);

class TimerNode
{
public:
    TimerNode(TimerCallback_t run, unsigned int inte, bool isRep,void *userdata);
    void updateNextTimerTime();
    bool operator<(const TimerNode &timerNode) const;
    bool operator==(int64_t TimerNodeID) const;

public:
    TimerCallback_t m_runFun; // 存储要执行的任务函数（把函数转为function对象，用shared_ptr进行管理）
    uint64_t m_intervalTime; //要执行的时间间隔，
    uint64_t m_nextRunTime; //下次要执行的时间
    void * m_userdata; //信息
    bool m_isRepeat; //是否要重复执行，还是只执行一次
};



#endif