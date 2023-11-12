#ifndef TASK_CPP
#define TASK_CPP

#include "thread.hpp"
#include "tasks.hpp"
#include "../interface/interface.hpp"
#include <vector>

EventHandler eventHandler;
EventHandler graphicalEventHandler;

Timeout::Timeout(CallbackClass* callback, uint32_t timeout)
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

Interval::Interval(CallbackClass* callback, uint32_t interval)
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


void addEventListener(CallbackClass* callback, ConditionClass* condition, bool autoremove, EventHandler* pEventHandler) // OK
{
    //print("addEventListener: " + std::to_string(pEventHandler->events.size()));
    pEventHandler->events.push_back(new Event(callback, condition, autoremove));
}

/* example:
    addEventListener(new Callback(callbackTest), new ConditionMethod((Gui*) label, &Button::isTouched));
*/

void removeEventListener(CallbackClass* callback, ConditionClass* condition, EventHandler* pEventHandler) // OK
{
    //print("removeEventListener: " + std::to_string(pEventHandler->events.size()));
    for (uint16_t i = 0; i < pEventHandler->events.size(); i++)
    {
        if(pEventHandler->events[i]->callback->getPtr() == callback->getPtr() && pEventHandler->events[i]->condition->getPtr() == condition->getPtr())
        {
            delete pEventHandler->events[i];
            pEventHandler->events.erase(pEventHandler->events.begin() + i);
            break;
        }
    }

    delete callback;
    delete condition;
}

uint16_t setTimeout(CallbackClass* callback, uint32_t timeout, EventHandler* pEventHandler) // OK
{
    Timeout* ntimeout = new Timeout(callback, timeout);
    pEventHandler->timeouts.push_back(ntimeout);
    return ntimeout->id;
}

void removeTimeout(uint32_t id, EventHandler* pEventHandler) // OK
{
    for (uint16_t i = 0; i < pEventHandler->timeouts.size(); i++)
    {
        if(pEventHandler->timeouts[i]->id == id)
        {
            delete pEventHandler->timeouts[i];
            pEventHandler->timeouts.erase(pEventHandler->timeouts.begin() + i);
            break;
        }
    }
}

/// Add a reccruring task to pEventHandler, returns its id in pEventHandler.
uint16_t setInterval(CallbackClass* callback, uint32_t interval, EventHandler* pEventHandler) // OK
{
    Interval* ninterval = new Interval(callback, interval);
    pEventHandler->intervals.push_back(ninterval);
    return ninterval->id;
}


/// Remove a reccuring task from pEventHandler with its id.
void removeInterval(uint32_t id, EventHandler* pEventHandler) // OK
{
    for (uint16_t i = 0; i < pEventHandler->intervals.size(); i++)
    {
        if(pEventHandler->intervals[i]->id == id)
        {
            pEventHandler->intervals.erase(pEventHandler->intervals.begin() + i);
            delete pEventHandler->intervals[i];
        }
    }
}

#endif
