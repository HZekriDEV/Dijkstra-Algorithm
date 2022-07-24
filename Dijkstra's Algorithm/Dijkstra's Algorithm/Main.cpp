#include <iostream>
#include "Graph.h"

using namespace std;

// Main entry point of the program. Includes "user interface" in the console
int main()
{
	Graph g;
	bool exit = false;
	char response;
	vector<string> dijkstraPath;
	vector<string>::iterator itr;
	
	cout << "************************************\n"
		<< "* 1] Add vertex                    *\n"
		<< "* 2] Add edge                      *\n"
		<< "* 3] Show adjacency matrix         *\n"
		<< "* 4] Find shortest path (Dijkstra) *\n"
		<< "* 5] Exit                          *\n"
		<< "************************************\n" << endl << endl;

	while (!exit)
	{
		cout << "Please select an option from the menu: ";
		cin >> response;
		cout << endl;

		if ((response != '1') && (response != '2') && (response != '3') && (response != '4') && (response != '5'))
		{
			cout << "Incorrect input. Try again" << endl << endl;
			continue;
		}
			
		string label1;
		string label2;
		unsigned long weight = 0;
		switch (response)
		{
			case '1':
				cout << "Enter the vertex label: ";
				cin >> label1;
				cout << endl;
				g.addVertex(label1);
				break;

			case '2':
				cout << "Enter the first vertex label: ";
				cin >> label1;
				cout << endl;
				cout << "Enter the second vertex label: ";
				cin >> label2;
				cout << endl;
				cout << "Enter the edge weight (positive integer): ";
				cin >> weight;
				cout << endl;
				g.addEdge(label1, label2, weight);
				break;

			case '3':
				g.showAdjacencyMatrix();
				break;

			case '4':
				cout << "Enter the start vertex label: ";
				cin >> label1;
				cout << endl;
				cout << "Enter the end vertex label: ";
				cin >> label2;
				cout << endl;
				cout << "The distance of the shortest path from " << label1 << " to " << label2 << " is " << g.shortestPath(label1, label2, dijkstraPath) << endl;
				cout << "The path is: ";
				itr = dijkstraPath.begin();
				cout << *itr;
				
				itr++;
				while (itr != dijkstraPath.end())
				{
					cout << " -> " << *itr;
					itr++;
				}
				cout << endl << endl;
				break;

			case '5':
				exit = true;
				break;
		}
	}

	return 0;
}