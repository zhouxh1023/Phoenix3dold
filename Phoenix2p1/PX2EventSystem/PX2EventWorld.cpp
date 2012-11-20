/*
*
* �ļ�����	��	PX2EventWorld.cpp
*
*/

#include "PX2EventWorld.hpp"
#include "PX2Event.hpp"
#include "PX2EventFactory.hpp"
using namespace PX2;

EventWorld::EventWorld ()
{
	mNowEventList = new0 EventList();
	assertion(mNowEventList!=0, "mNowEventList was not created successfully.");

	mNextEventList = new0 EventList();
	assertion(mNextEventList!=0, "mNextEventList was not created successfully.");
}
//----------------------------------------------------------------------------
EventWorld::~EventWorld ()
{
	// handlers
	mHandlers.clear();

	// events
	EventList::iterator itEvent;
	for (itEvent=mNowEventList->begin();
		itEvent!=mNowEventList->end();
		++itEvent)
	{
		EventFactory::GetInstance().DestoryEvent(*itEvent);
	}
	mNowEventList->clear();

	for (itEvent=mNextEventList->begin();
		itEvent!=mNextEventList->end();
		++itEvent)
	{
		EventFactory::GetInstance().DestoryEvent(*itEvent);
	}
	mNextEventList->clear();

	// list
	if (mNowEventList)
	{
		delete0(mNowEventList);
		mNowEventList = 0;
	}

	if (mNextEventList)
	{
		delete0(mNextEventList);
		mNextEventList = 0;
	}
}
//----------------------------------------------------------------------------
void EventWorld::ComeIn (EventHandler *handler)
{
	assertion(handler!=0, "handler must not be 0.");

	if (!handler)
		return;

	if (handler->IsInWorld(this))
		return;

	handler->Enter(this);

	AddHandler(handler);
}
//----------------------------------------------------------------------------
void EventWorld::GoOut(EventHandler *handler)
{
	assertion(handler!=0, "handler must not be 0.");

	if (!handler)
		return;

	if (!handler->IsInWorld(this))
		return;

	handler->Leave();

	RemoveHandler(handler);
}
//----------------------------------------------------------------------------
void EventWorld::Update (float detalTime)
{
	UpdateEvent(detalTime);
}
//----------------------------------------------------------------------------
void EventWorld::BroadcastingLocalEvent (Event* event)
{
	if (event->IsSystemEvent())
	{
		assertion(false, "event can't be system event.");
	}

	_BroadcastingEvent(event);
}
//----------------------------------------------------------------------------
void EventWorld::BroadcastingNetEvent (Event* event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
bool EventWorld::AddHandler (EventHandler* handler)
{
	mHandlers.push_back(handler);

	return true;
}
//----------------------------------------------------------------------------
void EventWorld::RemoveHandler (EventHandler* handler)
{
	EventHandlerList::iterator it = mHandlers.begin();

	for (; it!=mHandlers.end(); it++)
	{
		if (*it == handler)
		{
			*it = 0;
			mHandlers.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void EventWorld::UpdateEvent (float detalTime)
{
	EventList::iterator itEvent=mNowEventList->begin();
	for (;itEvent!=mNowEventList->end(); ++itEvent)
	{
		(*itEvent)->Update(detalTime);

		// delay
		if ((*itEvent)->IsDoDelay() && !(*itEvent)->IsDelayActted()) 
		{
			continue;
		}

		// ϵͳ��Ϣ
		if ((*itEvent)->IsSystemEvent())
		{
			// û��Ƶ������Ϣ
			switch ((*itEvent)->GetEventType())
			{
			case EVENT_NONE:
				break;
			default:
				break;
			}
		}
		// ��ϵͳ��Ϣ
		else
		{
			EventHandler* receiver = (*itEvent)->GetReceiver();
			if (receiver)
			{ // �н�����
				receiver->Execute(*itEvent);
			}
			else
			{ // û�н����ߣ���ѯ
				EventHandlerList::iterator itHandler;
				for (itHandler=mHandlers.begin();
					itHandler!=mHandlers.end();
					++itHandler)
				{
					if ((*itHandler)->GetChannel().IsListening((*itEvent)->GetChannel()))
						(*itHandler)->Execute(*itEvent);
				}
			}
		}

		// ��Ϣ�����꣬����ɾ��
		EventFactory::GetInstance().DestoryEvent(*itEvent);
		*itEvent = 0;
	}

	// ��û�д������¼���������������б�
	EventList::iterator itEvent1 = mNowEventList->begin();
	for (; itEvent1!=mNowEventList->end(); ++itEvent1)
	{
		if (*itEvent1)
		{
			mNextEventList->push_back(*itEvent1);
		}
	}

	mNowEventList->clear();

	SwapEventList();
}
//----------------------------------------------------------------------------
void EventWorld::_BroadcastingEvent (Event* event)
{
	mNextEventList->push_back(event);
}
//----------------------------------------------------------------------------
void EventWorld::SwapEventList()
{
	EventList* tempEventList = 0;

	tempEventList = mNowEventList;
	mNowEventList = mNextEventList;
	mNextEventList = tempEventList;
}
//----------------------------------------------------------------------------