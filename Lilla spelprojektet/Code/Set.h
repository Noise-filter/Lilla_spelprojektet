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

	void Union(int X, int Y, bool pathCompress, bool rank)
	{
		Node* x = findSet(X, pathCompress);
		Node* y = findSet(Y, pathCompress);

		if(rank)
		{
			if(x->rank >= y->rank)
			{
				if(x->rank == y->rank)
					x->rank++;
				y->parent = x;
			}
			else if(x->rank < y->rank)
				x->parent = y;
		}
		else
		{
			y->parent = x;
			x->parent = x;
		}
	}

	Node* findSet(int x, bool pathCompress)
	{
		if(pathCompress)
			return findSetPathCompress(sets.at(x));
		else
			return findSetNoPathCompress(sets.at(x));
	}

	Node* findSetNoPathCompress(Node* x)
	{
		while(x->parent != x)
			x = x->parent;
		return x;
	}

	Node* findSetPathCompress(Node* x)
	{
		if(x != x->parent)
			x->parent = findSetPathCompress(x->parent->parent);
		return x->parent;
	}

private:
	vector<Node*> sets;

};

#endif