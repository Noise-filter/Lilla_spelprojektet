#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <string>
using namespace std;

/*
	Enkel länkad lista med first och last pekare.
*/
template<class T>
class ListNode
{
public:
	T value;
	ListNode<T> *next;
	ListNode<T>(T value){ this->value = value; this->next = NULL; }
	~ListNode<T>() {}
};

template<class T>
class List
{
private:
	ListNode<T> *first;
	ListNode<T> *last;
	int nrOfListNodes;

public:
	List()
	{
		first = NULL;
		last = NULL;
		nrOfListNodes = 0;
	}

	List(const List<T>& list)
	{
		nrOfListNodes = 0;
		for(int i = 0; i < list.size(); i++)
			insertLast(list.elementAt(i));
	}

	~List()
	{
		ListNode<T> *walker = first;
		ListNode<T> *garbage = walker;
		for(int i = 0; i < nrOfListNodes; i++)
		{
			garbage = walker;
			walker = walker->next;
			delete garbage;
		}
	}

	List<T>& operator=(const List<T>& list)
	{
		ListNode<T> *walker = first;
		for(int i = 0; i < nrOfListNodes; i++)
		{
			ListNode<T> *garbage = walker;
			walker = walker->next;
			delete garbage;
		}

		nrOfListNodes = 0;
		for(int i = 0; i < list.size(); i++)
			insertLast(list.elementAt(i));

		return *this;
	}

	/*void insertAt(int pos, T value)
	{
		if(pos >= 0)
		{
			if(pos == 0)
				insertFirst(value);
			else if(pos >= nrOfListNodes)
				insertLast(value);
			else
			{
				ListNode<T> *newListNode = new ListNode<T>(value);
				ListNode<T> *walker = first;
				for(int i = 0; i < pos-1; i++)
					walker = walker->next;

				newListNode->next = walker->next;
				walker->next = newListNode;
				nrOfListNodes++;
			}
		}
	}*/

	void insertLast(T value)
	{
		ListNode<T> *newListNode = new ListNode<T>(value);
		if(nrOfListNodes == 0)
		{
			newListNode->next = first;
			first = newListNode;
			last = first;
		}
		else
		{
			last->next = newListNode;
			last = newListNode;
		}
		nrOfListNodes++;
	}

	/*
	void insertFirst(T value)
	{
		ListNode<T> *newListNode = new ListNode<T>(value);
		
		nrOfListNodes++;
	}*/

	T elementAt(int pos) const throw(...)
	{
		if(pos < 0 || pos >= nrOfListNodes)
			throw string("Out of Bounds");

		ListNode<T> *walker = first;
		for(int i = 0; i < pos; i++)
			walker = walker->next;

		return walker->value;
	}

	void removeFirst()
	{
		if(nrOfListNodes == 0)
		{
			return;
		}

		ListNode<T> *walker = first->next;
		delete first;
		first = walker;
		nrOfListNodes--;
	}

	void remove(int num)
	{
		ListNode<T>* temp;
		ListNode<T>* walker = first;
		for(int i = 0; i < num; i++)
		{
			temp = walker;
			walker = walker->next;
			delete temp;
		}
		nrOfListNodes -= num;
		first = walker;
	}

	/*
	int removeLast() throw(...)
	{
		int value = -1;
		if(nrOfListNodes == 0)
		{
			throw string("Empty List");
		}

		ListNode<T> *walker = first;
		for(int i = 0; i < nrOfListNodes-1; i++)
			walker = walker->next;

		value = walker->value;
		walker->next = NULL;
		nrOfListNodes--;

		delete walker;
		return value;
	}

	int removeAt(int pos) throw(...)
	{
		int value = -1;
		
		if(pos < 0 || pos >= nrOfListNodes)
		{
			throw string("Out of Bound");
		}

		if(pos == 0)
			value = removeFirst();
		else
		{
			ListNode<T> *walker = first;
			for(int i = 0; i < pos-1; i++)
				walker = walker->next;

			ListNode<T> *garbage = walker;
			garbage = garbage->next;
			value = garbage->value;
			walker->next = garbage->next;
			delete garbage;
			nrOfListNodes--;
		}
		return value;
	}
	*/

	int size() const
	{
		return nrOfListNodes;
	}

	ListNode<T>* getFirst()
	{
		return first;
	}

};

#endif