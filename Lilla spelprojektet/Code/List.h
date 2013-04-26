#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <string>
using namespace std;

/*
	Enkel länkad lista med first och last pekare.
*/
template<class T>
	class Node
	{
	public:
		T value;
		Node<T> *next;
		Node<T>(T value){ this->value = value; this->next = NULL; }
		~Node<T>() {}
	};

template<class T>
class List
{
private:
	Node<T> *first;
	Node<T> *last;
	int nrOfNodes;

public:
	List()
	{
		first = NULL;
		last = NULL;
		nrOfNodes = 0;
	}

	List(const List<T>& list)
	{
		nrOfNodes = 0;
		for(int i = 0; i < list.size(); i++)
			insertLast(list.elementAt(i));
	}

	~List()
	{
		Node<T> *walker = first;
		Node<T> *garbage = walker;
		for(int i = 0; i < nrOfNodes; i++)
		{
			garbage = walker;
			walker = walker->next;
			delete garbage;
		}
	}

	List<T>& operator=(const List<T>& list)
	{
		Node<T> *walker = first;
		for(int i = 0; i < nrOfNodes; i++)
		{
			Node<T> *garbage = walker;
			walker = walker->next;
			delete garbage;
		}

		nrOfNodes = 0;
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
			else if(pos >= nrOfNodes)
				insertLast(value);
			else
			{
				Node<T> *newNode = new Node<T>(value);
				Node<T> *walker = first;
				for(int i = 0; i < pos-1; i++)
					walker = walker->next;

				newNode->next = walker->next;
				walker->next = newNode;
				nrOfNodes++;
			}
		}
	}*/

	void insertLast(T value)
	{
		Node<T> *newNode = new Node<T>(value);
		if(nrOfNodes == 0)
		{
			newNode->next = first;
			first = newNode;
			last = first;
		}
		else
		{
			last->next = newNode;
			last = newNode;
		}
		nrOfNodes++;
	}

	/*
	void insertFirst(T value)
	{
		Node<T> *newNode = new Node<T>(value);
		
		nrOfNodes++;
	}*/

	T elementAt(int pos) const throw(...)
	{
		if(pos < 0 || pos >= nrOfNodes)
			throw string("Out of Bounds");

		Node<T> *walker = first;
		for(int i = 0; i < pos; i++)
			walker = walker->next;

		return walker->value;
	}

	void removeFirst()
	{
		if(nrOfNodes == 0)
		{
			return;
		}

		Node<T> *walker = first->next;
		delete first;
		first = walker;
		nrOfNodes--;
	}

	/*
	int removeLast() throw(...)
	{
		int value = -1;
		if(nrOfNodes == 0)
		{
			throw string("Empty List");
		}

		Node<T> *walker = first;
		for(int i = 0; i < nrOfNodes-1; i++)
			walker = walker->next;

		value = walker->value;
		walker->next = NULL;
		nrOfNodes--;

		delete walker;
		return value;
	}

	int removeAt(int pos) throw(...)
	{
		int value = -1;
		
		if(pos < 0 || pos >= nrOfNodes)
		{
			throw string("Out of Bound");
		}

		if(pos == 0)
			value = removeFirst();
		else
		{
			Node<T> *walker = first;
			for(int i = 0; i < pos-1; i++)
				walker = walker->next;

			Node<T> *garbage = walker;
			garbage = garbage->next;
			value = garbage->value;
			walker->next = garbage->next;
			delete garbage;
			nrOfNodes--;
		}
		return value;
	}
	*/

	int size() const
	{
		return nrOfNodes;
	}

	Node<T>* getFirst()
	{
		return first;
	}

};

#endif