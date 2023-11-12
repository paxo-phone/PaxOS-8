#ifndef TASK_HPP
#define TASK_HPP

#include "thread.hpp"
#include <vector>

class CallbackClass
{
    public:
    virtual void call() = 0;
    virtual void* (*getPtr(void))(void) = 0;
    virtual ~CallbackClass() {};
};

class ConditionClass
{
    public:
    virtual bool check() = 0;
    virtual void* (*getPtr(void))(void) = 0;
    virtual ~ConditionClass() {};
};

template <class C>

class CallbackMethod : public CallbackClass
{
    public:
    CallbackMethod(C* object, void (C::*callback)(void)) : object(object), callback(callback) {};
    C* object = nullptr;
    void (C::*callback)(void) = nullptr;
    void call() { (object->*callback)(); };
    //void* (*getPtr(void))(void) { return (void* (*)()) callback; };
    void* (*getPtr(void))(void) { return nullptr; };
};

template <class C>

class ConditionMethod : public ConditionClass
{
    public:
    ConditionMethod(C* object, bool (C::*condition)(void)) : object(object), condition(condition) {};
    C* object = nullptr;
    bool (C::* condition)(void);
    bool check() { return (object->*condition)(); };
    //void* (*getPtr(void))(void) { return (void* (*)()) condition; };
    void* (*getPtr(void))(void) { return nullptr; };
};

template <class C>

class Callback : public CallbackClass
{
    public:
    Callback(void (*callback)(void)) : callback(callback) {};
    void (*callback)(void) = nullptr;
    void call() { (callback)(); };
    //void* (*getPtr(void))(void) { return (void* (*)()) callback; };
    void* (*getPtr(void))(void) { return nullptr; };
};

template <class C>

class Condition : public ConditionClass
{
    public:
    Condition(bool (*condition)(void)) : condition(condition) {};
    bool (*condition)(void) = nullptr;
    bool check() { return (condition)(); };
    //void* (*getPtr(void))(void) { return (void* (*)()) condition; };
    void* (*getPtr(void))(void) { return nullptr; };
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

extern EventHandler eventHandler;
extern EventHandler graphicalEventHandler;

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
    Timeout(CallbackClass* callback, uint32_t timeout);
    CallbackClass* callback;
    uint64_t timeout;
    uint32_t id = 0;

    ~Timeout()
    {
        delete callback;
    }
};

class Interval
{
    public:
    Interval(CallbackClass* callback, uint32_t interval);

    CallbackClass* callback;
    uint32_t interval;
    uint32_t id = 0;
    uint64_t lastExecution = 0;

    ~Interval()
    {
        delete callback;
    }
};

void addEventListener(CallbackClass* callback, ConditionClass* condition, bool autoremove = false, EventHandler* pEventHandler = &eventHandler);

/* example:
    addEventListener(new Callback(callbackTest), new ConditionMethod((Gui*) label, &Button::isTouched));
*/

void removeEventListener(CallbackClass* callback, ConditionClass* condition, EventHandler* pEventHandler = &eventHandler);

uint16_t setTimeout(CallbackClass* callback, uint32_t timeout, EventHandler* pEventHandler = &eventHandler);

void removeTimeout(uint32_t id, EventHandler* pEventHandler = &eventHandler);

/// Add a reccruring task to pEventHandler, returns its id in pEventHandler.
uint16_t setInterval(CallbackClass* callback, uint32_t interval, EventHandler* pEventHandler = &eventHandler);


/// Remove a reccuring task from pEventHandler with its id.
void removeInterval(uint32_t id, EventHandler* pEventHandler = &eventHandler);

#endif
