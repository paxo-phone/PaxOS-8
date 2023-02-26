#ifndef TASK_HPP
#define TASK_HPP

#include "../widgets/gui.hpp" // juste pour les tests
class Gui;

class CallbackClass
{
    public:
    virtual void call() = 0;
};

class ConditionClass
{
    public:
    virtual bool check() = 0;
};

// Gui sera bien évidement déclaré dans les fichiers de Gui

template <class C>

class CallbackClassAuto : public CallbackClass
{
    public:
    CallbackClassAuto(C* object, void (C::*callback)(void)) : object(object), callback(callback) {};
    C* object = nullptr;
    void (C::*callback)(void) = nullptr;
    void call() { (object->*callback)(); };
};

template <class C>

class ConditionClassAuto : public ConditionClass
{
    ConditionClassAuto(C* object, bool (C::*condition)(void)) : object(object), condition(condition) {};
    C* object = nullptr;
    bool (C::* condition)(void);
    bool check() { return (object->*condition)(); };
};

class CallbackGui : public CallbackClass
{
    public:
    CallbackGui(Gui* object, void (Gui::*callback)(void)) : object(object), callback(callback) {};
    Gui* object = nullptr;
    void (Gui::*callback)(void) = nullptr;
    void call() { (object->*callback)(); };
};

class ConditionGui : public ConditionClass
{
    ConditionGui(Gui* object, bool (Gui::*condition)(void)) : object(object), condition(condition) {};
    Gui* object = nullptr;
    bool (Gui::* condition)(void);
    bool check() { return (object->*condition)(); };
};

// FIN POUR GUI

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

class Event
{
    public:
    Event(CallbackClass* callback, ConditionClass* condition) : callback(callback), condition(condition) {};
    CallbackClass* callback;
    ConditionClass* condition;

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
            delete timeouts[i];
            i-=1;
        }
    }
}


void addEventListener(CallbackClass* callback, ConditionClass* condition)
{
    eventHandler.events.push_back(new Event(callback, condition));
}

void removeEventListener(CallbackClass* callback, ConditionClass* condition)
{
    for (uint i = 0; i < eventHandler.events.size(); i++)
    {
        if(eventHandler.events[i]->callback == callback && eventHandler.events[i]->condition == condition)
        {
            delete eventHandler.events[i];
        }
    }
}

bool setTimeout(CallbackClass* callback, uint32_t timeout)
{
    Timeout* ntimeout = new Timeout(callback, timeout);
    eventHandler.timeouts.push_back(ntimeout);
    return ntimeout->id;
}

void removeTimeout(uint32_t id)
{
    for (uint i = 0; i < eventHandler.timeouts.size(); i++)
    {
        if(eventHandler.timeouts[i]->id == id)
        {
            delete eventHandler.timeouts[i];
        }
    }
}

bool setInterval(CallbackClass* callback, uint32_t interval)
{
    Interval* ninterval = new Interval(callback, interval);
    eventHandler.intervals.push_back(ninterval);
    return ninterval->id;
}

void removeInterval(uint32_t id)
{
    for (uint i = 0; i < eventHandler.intervals.size(); i++)
    {
        if(eventHandler.intervals[i]->id == id)
        {
            delete eventHandler.intervals[i];
        }
    }
}

#endif