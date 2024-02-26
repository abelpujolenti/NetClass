#include "SocketSelector.h"

bool SocketSelector::Wait()
{
    _mutex.lock();
    bool ready = wait();
    _mutex.unlock();
    return ready;
}

bool SocketSelector::IsReady(TcpListener& listener)
{
    _mutex.lock();
    bool ready = isReady(listener);
    _mutex.unlock();
    return ready;
}

bool SocketSelector::IsReady(TcpSocket& socket)
{
    _mutex.lock();
    bool ready = isReady(socket);
    _mutex.unlock();
    return ready;
}

void SocketSelector::Add(TcpListener& listener)
{
    _mutex.lock();

    add(listener);

    _mutex.unlock();
}

void SocketSelector::Add(TcpSocket& socket)
{
    _mutex.lock();

    add(socket);

    _mutex.unlock();
}

void SocketSelector::Remove(TcpListener& listener)
{
    _mutex.lock();
    
    remove(listener);
    
    _mutex.unlock();
}

void SocketSelector::Remove(TcpSocket& socket)
{
    _mutex.lock();

    remove(socket);

    _mutex.unlock();
}