#include "timernode.h"
#include "time.h"

TimerNode::TimerNode(TimerCallback_t run, unsigned int inter, bool isRep,void *userdata)
{
    m_runFun = run;
    m_intervalTime = inter;
    m_isRepeat = isRep;
    m_userdata = userdata;

    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    uint32_t temp = (uint32_t)(current_time.tv_sec * 1000);
    temp +=  (current_time.tv_nsec / 1000000);
    
    m_nextRunTime = temp+inter;
    
}

void TimerNode::updateNextTimerTime()
{
    m_nextRunTime += m_intervalTime;
}

bool TimerNode::operator<(const TimerNode &timerNode) const
{
    return m_nextRunTime < timerNode.m_nextRunTime;
}
bool TimerNode::operator==(int64_t TimerNodeID) const
{
    return reinterpret_cast<int64_t>(m_runFun) == TimerNodeID;
}
