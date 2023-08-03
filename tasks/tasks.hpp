#ifndef TASK_HPP
#define TASK_HPP

#include "thread.hpp"

#ifdef WIN32
#define uint unsigned int
#endif

class CallbackClass
{
    public:
    virtual void call() = 0;
    virtual void* (*getPtr(void))(void) = 0;
};

class ConditionClass
{
    public:
    virtual bool check() = 0;
    virtual void* (*getPtr(void))(void) = 0;
};

template <class C>

class CallbackMethod : public CallbackClass
{
    public:
    CallbackMethod(C* object, void (C::*callback)(void)) : object(object), callback(callback) {};
    C* object = nullptr;
    void (C::*callback)(void) = nullptr;
    void call() { (object->*callback)(); };
    void* (*getPtr(void))(void) { return (void* (*)()) callback; };
};

template <class C>

class ConditionMethod : public ConditionClass
{
    public:
    ConditionMethod(C* object, bool (C::*condition)(void)) : object(object), condition(condition) {};
    C* object = nullptr;
    bool (C::* condition)(void);
    bool check() { return (object->*condition)(); };
    void* (*getPtr(void))(void) { return (void* (*)()) condition; };
};

template <class C>

class Callback : public CallbackClass
{
    public:
    Callback(void (*callback)(void)) : callback(callback) {};
    void (*callback)(void) = nullptr;
    void call() { (callback)(); };
    void* (*getPtr(void))(void) { return (void* (*)()) callback; };
};

template <class C>

class Condition : public ConditionClass
{
    public:
    Condition(bool (*condition)(void)) : condition(condition) {};
    bool (*condition)(void) = nullptr;
    bool check() { return (condition)(); };
    void* (*getPtr(void))(void) { return (void* (*)()) condition; };
};

class Event;
class Timeout;
class Interval;

class EventHandler
{
    public:
    std::vector<Event*> events;
    std::vector<Timeout*> timeouts;
    std::vector<Interval*> intervals;

    void update();
};

EventHandler eventHandler;
EventHandler graphicalEventHandler;

class Event
{
    public:
    Event(CallbackClass* callback, ConditionClass* condition, bool autoremove) : callback(callback), condition(condition), autoremove(autoremove) {};
    CallbackClass* callback;
    ConditionClass* condition;
    bool autoremove = false;

    ~Event()
    {
        delete callback;
        delete condition;
    }
};

class Timeout
{
    public:
    Timeout(CallbackClass* callback, uint32_t timeout)
    {
        this->callback = callback;
        this->timeout = millis()+timeout;
        for (uint32_t i = 0; i < eventHandler.timeouts.size(); i++) // check for all id
        {
            bool alreadyExisting = false; // false if no event existing with id=i

            for (uint32_t j = 0; j < eventHandler.timeouts.size(); j++) // check all Timeouts existing
            {
                if(i==eventHandler.timeouts[j]->id)
                {
                    alreadyExisting=true;
                    break;
                }
            }
            if(!alreadyExisting)
            {
                this->id = i;
                return;
            }
        }
    }
    CallbackClass* callback;
    uint32_t timeout;
    uint32_t id = 0;

    ~Timeout()
    {
        delete callback;
    }
};

class Interval
{
    public:
    Interval(CallbackClass* callback, uint32_t interval)
    {
        this->callback = callback;
        this->interval = interval;
        for (uint32_t i = 0; i < eventHandler.intervals.size(); i++) // check for all id
        {
            bool alreadyExisting = false; // false if no Interval existing with id=i

            for (uint32_t j = 0; j < eventHandler.intervals.size(); j++) // check all Intervals existing
            {
                if(i==eventHandler.intervals[j]->id)
                {
                    alreadyExisting=true;
                    break;
                }
            }
            if(!alreadyExisting)
            {
                this->id = i;
                return;
            }
        }
    }

    CallbackClass* callback;
    uint32_t interval;
    uint32_t id = 0;
    uint32_t lastExecution = 0;

    ~Interval()
    {
        delete callback;
    }
};

void EventHandler::update()
{
    for(uint32_t i = 0; i < events.size(); i++)
    {
        if(events[i]->condition->check())
        {
            events[i]->callback->call();
            if(events[i]->autoremove)
            {
                delete events[i];
                eventHandler.events.erase(eventHandler.events.begin() + i);
                break;
            }
        }
    }
    for(uint32_t i = 0; i < intervals.size(); i++)
    {
        if(intervals[i]->lastExecution + intervals[i]->interval < millis())
        {
            intervals[i]->callback->call();
            intervals[i]->lastExecution=millis();
        }
    }
    for(uint32_t i = 0; i < timeouts.size(); i++)
    {
        if(timeouts[i]->timeout < millis())
        {
            timeouts[i]->callback->call();
            delete eventHandler.timeouts[i];
            eventHandler.timeouts.erase(eventHandler.timeouts.begin() + i);
            break;
            i-=1;
        }
    }
}


void addEventListener(CallbackClass* callback, ConditionClass* condition, bool autoremove = false, EventHandler* eventHandler = &eventHandler) // OK
{
    print("addEventListener: " + to_string(eventHandler->events.size()));
    eventHandler->events.push_back(new Event(callback, condition, autoremove));
}

/* example:
    addEventListener(new Callback(callbackTest), new ConditionMethod((Gui*) label, &Button::isTouched));
*/

void removeEventListener(CallbackClass* callback, ConditionClass* condition, EventHandler* eventHandler = &eventHandler) // OK
{
    print("removeEventListener: " + to_string(eventHandler->events.size()));
    for (uint i = 0; i < eventHandler->events.size(); i++)
    {
        if(eventHandler->events[i]->callback->getPtr() == callback->getPtr() && eventHandler->events[i]->condition->getPtr() == condition->getPtr())
        {
            delete eventHandler->events[i];
            eventHandler->events.erase(eventHandler->events.begin() + i);
            break;
        }
    }

    delete callback;
    delete condition;
}

uint setTimeout(CallbackClass* callback, uint32_t timeout, EventHandler* eventHandler = &eventHandler) // OK
{
    Timeout* ntimeout = new Timeout(callback, timeout);
    eventHandler->timeouts.push_back(ntimeout);
    return ntimeout->id;
}

void removeTimeout(uint32_t id, EventHandler* eventHandler = &eventHandler) // OK
{
    for (uint i = 0; i < eventHandler->timeouts.size(); i++)
    {
        if(eventHandler->timeouts[i]->id == id)
        {
            delete eventHandler->timeouts[i];
            eventHandler->timeouts.erase(eventHandler->timeouts.begin() + i);
            break;
        }
    }
}

uint setInterval(CallbackClass* callback, uint32_t interval, EventHandler* eventHandler = &eventHandler) // OK
{
    Interval* ninterval = new Interval(callback, interval);
    eventHandler->intervals.push_back(ninterval);
    return ninterval->id;
}

void removeInterval(uint32_t id, EventHandler* eventHandler = &eventHandler) // OK
{
    for (uint i = 0; i < eventHandler->intervals.size(); i++)
    {
        if(eventHandler->intervals[i]->id == id)
        {
            eventHandler->intervals.erase(eventHandler->intervals.begin() + i);
            delete eventHandler->intervals[i];
        }
    }
}

#endif