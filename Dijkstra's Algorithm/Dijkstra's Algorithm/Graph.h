#pragma once
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// Base vertex class. This program uses the adjacency list data structure
class Vertex
{
public:
	Vertex(string l) : label(l) {};
	string label;

private:
	// Each vertex contains a vector of all its adjacent vertices
	vector<Vertex*> adjVertices;
	unsigned long distance = 0;
	Vertex* prev = nullptr;
	friend class Graph;
};

// Base edge class
class Edge
{
public:
	Edge(unsigned long w, pair<Vertex*, Vertex*> p) : weight(w), vertexPair(p) {};
	unsigned long weight;

private:
	pair<Vertex*, Vertex*> vertexPair;
	friend class Graph;
};

class Graph
{
public:
	// Adds a vertex to the graph
	void addVertex(std::string label);

	// Removes a vertex from the graph
	void removeVertex(std::string label);

	// Adds an edge between two vertices as well as assigns a weight to it
	void addEdge(std::string label1, std::string label2, unsigned long weight);

	// Removes the edge between two given vertex labels
	void removeEdge(std::string label1, std::string label2);

	// Calculates the shortest path between two vertices using Dijkstra's Algorithm
	unsigned long shortestPath(std::string startLabel, std::string endLabel, std::vector<std::string>& path);

	// Returns the edge weight between two vertices
	unsigned long getEdgeWeight(Vertex* v1, Vertex* v2);

	// Prints the graph's adjacency matrix to the console
	void showAdjacencyMatrix();

	// Destroys the graph (destructor)
	~Graph();

private:
	// Contains all vertices in the graph
	vector<Vertex*> vertexList;

	// Contains all edges in the graph
	vector<Edge*> edgeList;
};