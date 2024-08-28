/*
    This queue structure was implemented to be used in two different ways in the program:
        1 - To handle compound words present in the input text. For example, "New York"
        2 - To open multiple input files.
*/ 
#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <iostream>
#include <string>

struct ListNode
{
    std::string *info;
    ListNode* next;

    ListNode(const std::string& str) : info(new std::string(str)), next(nullptr) {}
    ~ListNode() { delete info; }
};

class Queue
{
public:
    Queue();
    ~Queue();

    bool search(const std::string& str) const;
    void enqueue(const std::string& str);
    void dequeue();
    void cleanup();

    ListNode* getStart() const { return start; } // Inline getter for start
    ListNode* getEnd() const { return end; } // Inline getter for end

private:
    ListNode* start;
    ListNode* end;
};

#endif
