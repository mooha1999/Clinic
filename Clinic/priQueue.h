#pragma once
#include "priNode.h"

// This class impelements the priority queue as a sorted list (Linked List)
// The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
    priNode<T> *head;

public:
    priQueue() : head(nullptr) {}

    ~priQueue()
    {
    }

    // insert the new node in its correct position according to its priority
    void enqueue(const T &data, int priority)
    {
        priNode<T> *newNode = new priNode<T>(data, priority);

        if (head == nullptr || priority > head->getPri())
        {

            newNode->setNext(head);
            head = newNode;
            return;
        }

        priNode<T> *current = head;
        while (current->getNext() && priority <= current->getNext()->getPri())
        {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
    }

    bool dequeue(T &topEntry, int &pri)
    {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T> *temp = head;
        head = head->getNext();
        delete temp;
        return true;
    }

    bool peek(T &topEntry, int &pri)
    {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        pri = head->getPri();
        return true;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    int getSize() const
    {
        int size = 0;
        priNode<T> *current = head;
        while (current)
        {
            size++;
            current = current->getNext();
        }
        return size;
    }

	priQueue<T> clone() const
	{
		priQueue<T> newQueue;
		priNode<T>* current = head;
		while (current)
		{
			int i = current->getPri();
			newQueue.enqueue(current->getItem(i), current->getPri());
			current = current->getNext();
		}
		return newQueue;
	}
};
