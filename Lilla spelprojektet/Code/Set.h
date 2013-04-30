#ifndef SET_H
#define SET_H

#include <vector>
#include "Structure.h"
using namespace std;

template <class T>
class Set
{
public:
	struct Node
	{
		T value;
		Node* parent;
		int rank;
	};

	Set()
	{}

	~Set()
	{
		for(int i = 0; i < (int)sets.size(); i++)
			delete sets.at(i);
	}

	void initSets(T** data, int size)
	{
		if(sets.size() == 0)
		{
			for(int i = 0; i < size*size; i++)
				sets.push_back(new Node);
		}

		//sets.clear();
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				//sets.push_back(new Node);
				MakeSet(j + (i*size), data[i][j]);
			}
		}
	}

	void initSets(vector<T>& data)
	{
		sets.clear();
		sets.resize(data.size() + 5);
		for(int i = 0; i < (int)data.size(); i++)
		{
			sets.push_back(new Node);
			MakeSet(i, data.at(i));
		}
	}

	void MakeSet(int id, T data)
	{
		sets.at(id)->rank = 0;
		sets.at(id)->parent = sets.at(id);
		sets.at(id)->value = data;
	}

	void Union(int X, int Y)
	{
		Node* x = findSet(sets.at(X));
		Node* y = findSet(sets.at(Y));

		if(x->rank >= y->rank)
		{
			if(x->rank == y->rank)
				x->rank++;
			y->parent = x;
		}
		else if(x->rank < y->rank)
			x->parent = y;
	}

	Node* findSet(int x)
	{
		return findSet(sets.at(x));
	}

	Node* findSet(Node* x)
	{
		if(x != x->parent)
			x->parent = findSet(x->parent);
		return x->parent;
	}

private:
	vector<Node*> sets;

};

#endif