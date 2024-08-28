#include "Queue.h"

Queue::Queue() : start(nullptr), end(nullptr) { }

Queue::~Queue()
{
    cleanup();
}

bool Queue::search(const std::string& str) const
{
    ListNode* list;
    for(list = start; list != nullptr; list = list->next)
    {
        if (*(list->info) == str)
            return true;
    }
    return false;
}

void Queue::enqueue(const std::string& str)
{
    ListNode* node = new ListNode(str);

    if (end)
    {
        end->next = node;
    }
    else
    {
        start = node;
    }
    end = node;
}

void Queue::dequeue()
{
    if (!start)
        return;

    ListNode* temp = start;
    start = start->next;

    if (!start)
        end = nullptr;

    delete temp;
}

void Queue::cleanup()
{
    while (start)
    {
        dequeue();
    }
}
