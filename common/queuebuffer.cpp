#include "queuebuffer.h"


QueueBuffer::QueueBuffer(int size) 
{
    m_max = size;
    m_queue = std::queue<DATAMESSAGE>();
}

QueueBuffer::~QueueBuffer()
{

}

void QueueBuffer::push(DATAMESSAGE message)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    if(m_queue.size() > m_max)
    {
        printf("full m_queue.size() %d\n",m_queue.size()); 
        m_full.wait(lck);
    } 	    
    m_queue.push(message);
    
    //通知消费者可以消费了，消费者得到该通知，就会从等待状态变为阻塞状态，拿到锁后就会执行消费
    m_enpty.notify_all();
}

DATAMESSAGE QueueBuffer::get()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    if(m_queue.size() <= 0) 	
        m_enpty.wait(lck);//缓冲池为空时等待 
	DATAMESSAGE message;
    message = m_queue.front();
    m_queue.pop();
	m_full.notify_all();//唤起full
    return message;
}