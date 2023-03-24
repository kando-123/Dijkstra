/** @file */

#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <cstring>
#include <list>
#include <vector>

enum class message { allCorrect, wrongData, noFile, nonEmptyGraph };

class my_graph
{
    const int undefined = -1;
    const double infinity = (double)((unsigned long long)(-1));
    struct element { unsigned terminal; double weight; };

    unsigned order, size;
    std::string sourceName;
    std::vector<std::list<element>> adjacencyList;
    std::vector<double> distance;
    std::vector<int> previous;
    unsigned reference;

public:
    /**
    Sets the data members to initial values.
    */
    my_graph();
    /**
    Calls clearGraph().
    */
    ~my_graph();
    /**
    Sets source name.
    @param newSourceName - the path to the source file
    */
    void setSourceName(const std::string&);
    /**
    Reads a grap from the file whose path was saved in the object by means of method setSourceName().
    @return Information about the execution:
        allCorrect - if the graph was read correctly,
        noFile - if the file does not exist,
        nonEmptyGraph - if some data had already been read,
        wrongData - if the file exists but the discription of the graph is incorrect,
    In all cases apart from allCorrect, the reading process is broken and the adjacency list is immediately deleted if has been created.
    */
    message readGraph();
    /**
    Returns information whether the graph is empty.
    @return true if the graph is empty, otherwise false
    */
    bool empty();
    /**
    Prints out the graph or information that no graph has been read in.
    */
    void printGraph();
public:
    /**
    Deletes all data and sets the initial values.
    */
    void clearGraph();
    /**
    Performs the Dijkstra algorithm for the graph, i.e. seeks the shortest paths from the reference vertex to all other vertices.
    @param vertex The reference vertex.
    */
    message Dijkstra(unsigned);
    /**
    Prints out the results from the Dijkstra algorithm or information that there are no results to print.
    */
    void DijkstraResults();
    /**
    Serialises the vector of distances into a file.
    @param name the name of the file the vector should be serialised into
    */
    message DijkstraSerialise(std::string&);
};

#endif // !METHODS_H