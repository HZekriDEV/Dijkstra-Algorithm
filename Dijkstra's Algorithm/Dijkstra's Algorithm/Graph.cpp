#include "Graph.h"

void Graph::addVertex(std::string label)
{
	// Checking if the vertex already exists
	// If so, an exception is thrown
	for (Vertex* v : vertexList)
	{
		if (label == v->label)
		{
			cout << "Vertex already exists" << endl << endl;
			return;
		}
	}

	// Allocate memory for new vertex and add it to the graph's vertex list
	Vertex* v = new Vertex(label);
	vertexList.push_back(v);
}
void Graph::removeVertex(std::string label)
{
	vector<Vertex*>::iterator itr;
	// Iterating through the graphs vertex list to find the vertex that is to be deleted
	Vertex* vertex = nullptr;
	for (itr = vertexList.begin(); itr != vertexList.end(); ++itr)
	{
		// If we find the label, set the current node to "vertex" and break the loop
		if (label == (*itr)->label)
		{
			vertex = *itr;
			break;
		}
	}

	// If vertex is null, we have not found the vertex in the graph, so an exception is thrown
	if (vertex == nullptr)
	{
		cout << "Vertex does not exist" << endl << endl;
		return;
	}

	vector<Vertex*>::iterator i;
	vector<Vertex*>::iterator j;
	// For each adjacent vertex, we remove the vertex we want to delete from its adjacent vertices list
	for (i = vertex->adjVertices.begin(); i != vertex->adjVertices.end(); ++i)
	{
		for (j = (*i)->adjVertices.begin(); j != (*i)->adjVertices.end(); ++j)
		{
			if ((*j)->label == vertex->label)
			{
				(*i)->adjVertices.erase(j);
				break;
			}
		}
	}

	vector<Edge*>::iterator eItr;
	vector<Vertex*>::iterator vItr;
	// This block of code searches for every edge object that contains the vertex
	// we want to remove. It does this by searching the graph's edge list for
	// any occurrence of the vertex we want to delete. The edge is stored in a
	// temporary edge object e and deleted after each loop.
	for (vItr = vertex->adjVertices.begin(); vItr != vertex->adjVertices.end(); vItr++)
	{
		Edge* e = nullptr;
		for (eItr = edgeList.begin(); eItr != edgeList.end(); ++eItr)
		{
			if (((*eItr)->vertexPair.first->label == vertex->label || (*eItr)->vertexPair.second->label == vertex->label))
			{
				e = *eItr;
				edgeList.erase(eItr);
				break;
			}
		}
		delete e;
	}

	// Clearing all adjacent vertices from the vertex's list of adjacent vertices
	vertex->adjVertices.clear();

	// Deleting the vertex from the graphs list of vertices
	vertexList.erase(itr);

	// deleting the vertex object from memory
	delete vertex;
}
void Graph::addEdge(std::string label1, std::string label2, unsigned long weight)
{
	// If both input vertices are the same, throw an exception
	if (label1 == label2)
	{
		cout << "A vertex cannot have an edge to itself" << endl << endl;
		return;
	}

	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;
	// Search through the graph's vertex list for the vertices we want to create an edge between
	// The vertices are stored in v1 and v2
	for (Vertex* i : vertexList)
	{
		if (label1 == i->label)
			v1 = i;
		else if (label2 == i->label)
			v2 = i;
	}

	// If one or both vertices are not found in the vertex list, throw an exception
	if (v1 == nullptr || v2 == nullptr)
	{
		cout << "One or more labels do not correspond to existing vertices" << endl << endl;
		return;
	}

	// If v2 is already in the adjacennt vertex list of v1, there already exists an
	// edge between the two vertices. An exception is thrown
	for (Vertex* i : v1->adjVertices)
	{
		if (i->label == v2->label)
		{
			cout << "An edge already exists between these two vertices" << endl << endl;
			return;
		}
	}

	// Allocate memory for the new edge and add it to the graph's edge list
	Edge* e = new Edge(weight, pair<Vertex*, Vertex*>(v1, v2));
	edgeList.push_back(e);

	// Add each vertex to the other vertex's adjVertices list
	(v1->adjVertices).push_back(v2);
	(v2->adjVertices).push_back(v1);
}
void Graph::removeEdge(std::string label1, std::string label2)
{
	vector<Vertex*>::iterator vItr;
	vector<Edge*>::iterator eItr;

	Edge* e = nullptr;
	Vertex* v1 = nullptr;
	Vertex* v2 = nullptr;
	// Search through the graph's vertex list for the vertices we want to create an edge between
	// The vertices are stored in v1 and v2
	for (Vertex* i : vertexList)
	{
		if (label1 == i->label)
			v1 = i;
		else if (label2 == i->label)
			v2 = i;
	}

	// If one or both vertices are not found in the vertex list, throw an exception
	if (v1 == nullptr || v2 == nullptr)
	{
		cout << "One or more labels do not correspond to existing vertices" << endl << endl;
		return;
	}

	// Iterating through the graphs list of edges. When the edge we are looking for is
	// found, we set the variable e to it and erase it from the graph's edge list.
	for (eItr = edgeList.begin(); eItr != edgeList.end(); ++eItr)
	{
		if (((*eItr)->vertexPair.first->label == v1->label || (*eItr)->vertexPair.first->label == v2->label)
			&& ((*eItr)->vertexPair.second->label == v1->label || (*eItr)->vertexPair.second->label == v2->label))
		{
			e = *eItr;
			edgeList.erase(eItr);
			break;
		}
	}

	// Erases v2 from the adjacent vertex list of v1
	for (vItr = v1->adjVertices.begin(); vItr != v1->adjVertices.end(); ++vItr)
	{
		if ((*vItr)->label == v2->label)
		{
			(v1->adjVertices).erase(vItr);
			break;
		}
	}

	// Erases v1 from the adjacent vertex list of v2
	for (vItr = v2->adjVertices.begin(); vItr != v2->adjVertices.end(); ++vItr)
	{
		if ((*vItr)->label == v1->label)
		{
			(v2->adjVertices).erase(vItr);
			break;
		}
	}

	// Dealocate the memory for e
	delete (e);
}
unsigned long Graph::shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string>& path)
{
	// Min heap based priority queue to keep track of the vertices with the minimum distance
	priority_queue<pair<unsigned long, Vertex*>, vector<pair<unsigned long, Vertex*>>, greater<pair<unsigned long, Vertex*>>> pq;

	// Will contain the destination vertex (endLabel)
	Vertex* destination = nullptr;

	// Will contain the final distance to be returned
	unsigned long distance = 0;

	// If the graphs list of vertices is empty, throw an exception
	if (vertexList.empty())
	{
		cout << "Graph is empty" << endl << endl;
		exit(-1);
	}

	// Iterating throught the graph's vertex list. The distance for the starting vertex is
	// set to 0 while the distance is set to infinity(LONG_MAX) for the rest of the vertices
	for (Vertex* v : vertexList)
	{
		if (v->label == startLabel)
			v->distance = 0;
		else
		{
			v->distance = 2147483647L;
		}
		// Each vertex and their distance is pushed into the priority queue
		pq.push(pair<unsigned long, Vertex*>(v->distance, v));

		// The pointer to the previous vertex is set to nullptr for all vertices
		v->prev = nullptr;
	}

	// Main Dijkstra loop
	// Loops until the top of the priority queue is the destination vertex
	// pq.top().second represents the current vertex
	while (pq.top().second->label != endLabel)
	{
		// Iterating through the current vertex's list of adjacent vertices
		for (Vertex* u : pq.top().second->adjVertices)
		{
			// Updating the distances of the vertices adjacent to the current vertex
			if (pq.top().second->distance + getEdgeWeight(pq.top().second, u) < u->distance)
			{
				// If dist(current) + weight(current, u) < dist(u),
				// set dist(u) to dist(current) + weight(current, u)
				u->distance = pq.top().second->distance + getEdgeWeight(pq.top().second, u);

				// The rest of this if statement is basically a method of finding and updating the distance value of u in 
				// the priority queue
				priority_queue<pair<unsigned long, Vertex*>, vector<pair<unsigned long, Vertex*>>, greater<pair<unsigned long, Vertex*>>> copy;

				copy = pq;
				while (!pq.empty())
					pq.pop();

				while (!copy.empty())
				{
					pq.push(pair<unsigned long, Vertex*>(copy.top().second->distance, copy.top().second));
					copy.pop();
				}

				// Setting u's prev pointer to the current vertex
				u->prev = pq.top().second;
			}
		}

		// Pop the current vertex from the priority queue
		pq.pop();
	}

	// Setting the destination and distance variables
	destination = pq.top().second;
	distance = pq.top().second->distance;

	// Iterating backwards through the shortest path using the prev
	// pointers and pushing each vertex label into the input path vector
	for (Vertex* i = destination; i != nullptr; i = i->prev)
	{
		path.push_back(i->label);
	}

	// Reversing the path vector in order to get the correct sequence
	// of vertex labels (startLabel -> endLabel)
	reverse(path.begin(), path.end());

	// Return the shortest path's distance
	return distance;
}

void Graph::showAdjacencyMatrix()
{
	// Printing the labels for the columns
	cout << "  ";
	for (vector<Vertex*>::iterator col = vertexList.begin(); col != vertexList.end(); ++col)
		cout << (*col)->label << " ";

	cout << endl;
	// Printing the labels for the rows
	for (vector<Vertex*>::iterator row = vertexList.begin(); row != vertexList.end(); ++row)
	{
		cout << (*row)->label << "|";
		// Filling each row with 1 if there is an edge and 0 if there is no edge
		for (vector<Vertex*>::iterator col = vertexList.begin(); col != vertexList.end(); ++col)
		{
			if (getEdgeWeight(*row, *col) == -1)
				cout << 0 << "|";
			else
				cout << 1 << "|";

		}
		cout << endl;
	}
	cout << endl;
}

unsigned long Graph::getEdgeWeight(Vertex* v1, Vertex* v2)
{
	// Iterates throught the graph's edge list. If the edge we want is found,
	// its weight is returned
	for (Edge* e : edgeList)
	{
		if ((e->vertexPair.first == v1 || e->vertexPair.first == v2) && (e->vertexPair.second == v1 || e->vertexPair.second == v2))
			return e->weight;
	}

	// If the edge is not found, an exception is thrown
	return -1;
}

// Deletes all vertices and edges of the graph
// (The vertices and edges are the only objects which are manually allocated using "new")
Graph::~Graph()
{
	for (Edge* e : edgeList)
		delete e;
	for (Vertex* v : vertexList)
		delete v;
}