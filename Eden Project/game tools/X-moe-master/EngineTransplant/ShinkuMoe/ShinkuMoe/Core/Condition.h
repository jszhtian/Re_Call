#pragma once

class Condition
{
public:
    Condition();
    ~Condition();
   
    void Set();
    void Wait();
    
private:
    #ifndef _WIN32
    void* Mutex;
    #endif
    void* Event;
};
