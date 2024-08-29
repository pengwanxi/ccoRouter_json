#ifndef _QUEUEBUFFER_H_
#define _QUEUEBUFFER_H_

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>




typedef struct{
    int src;
    std::string topic;
    std::string message;
}DATAMESSAGE;

class QueueBuffer
{

public:
    QueueBuffer(int size = 100);
    ~QueueBuffer();

    void push(DATAMESSAGE message);
    DATAMESSAGE get();


private:
    std::queue<DATAMESSAGE> m_queue;
    unsigned int m_max;     


    std::mutex m_mutex;
    std::condition_variable m_full;//如果某时刻缓冲池为满，那么就让full等待
    std::condition_variable m_enpty;//如果某时刻缓冲池为空，那么就让empty等待


};



#endif 