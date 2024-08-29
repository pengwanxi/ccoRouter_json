#include "timerlist.h"
#include "stddef.h"
#include "time.h"
#include <algorithm>
 #include <unistd.h>


TimerList::TimerList()
{

}

TimerList::~TimerList()
{
    m_timerlist.clear();
}

int64_t TimerList::addTimerNode(TimerCallback_t run, unsigned int  intervalTime, bool isRepeat,void *userdata)
{
    TimerNode timer(run, intervalTime, isRepeat,userdata); 
    m_timerlist.push_back(timer); 
    m_timerlist.sort(); 
    return reinterpret_cast<int64_t>(run);
}


void TimerList::timerProcess() // 处理timer
{
    while(true)
    {
        auto it = m_timerlist.begin();
        uint32_t sleeptime = 0;
        while (it != m_timerlist.end())
        {
            TimerNode &timer = *it;
            struct timespec current_time;
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            uint32_t currentTime = (uint32_t)(current_time.tv_sec * 1000);
            currentTime +=  (uint32_t)(current_time.tv_nsec / 1000000);
            
            if (currentTime < timer.m_nextRunTime) 
            {
                sleeptime = timer.m_nextRunTime-currentTime;         
                break; 
            }
            timer.m_runFun(timer.m_userdata);
            if (timer.m_isRepeat) //判断是不是要重复执行
            {
                timer.updateNextTimerTime(); //是重复执行，要更新下次执行的时间
                ++it;
            }
            else
            {
                it = m_timerlist.erase(it); //不是重复执行的，删除就行
            }
            m_timerlist.sort(); //处理后对list进行排序
            sleeptime = 10;
            break;
        }
        usleep(sleeptime*1000);
    }
}

void TimerList::deleteTimerNode( int64_t timerID )
{
    auto timer = std::find(m_timerlist.begin(), m_timerlist.end(), timerID); //根据timerID查找是不是有存在，这个和上面重载 == 符号相关
    if(timer != m_timerlist.end()) //找到这个节点，并且不是end
    {
        m_timerlist.erase(timer);//删除这个节点
    }
}

uint32_t TimerList::getCurrentTimeMs()
{
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    uint32_t temp = (uint32_t)(current_time.tv_sec * 1000);
    temp +=  (uint32_t)(current_time.tv_nsec / 1000000);
    return temp;
}

