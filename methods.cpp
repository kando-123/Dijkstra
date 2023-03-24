/** @file */

#include <iostream>
#include <fstream>
#include <filesystem>
#include "methods.h"

my_graph::my_graph()
{
    order = size = 0;
    reference = undefined;
    sourceName = "";
}

my_graph::~my_graph()
{
    clearGraph();
}

void my_graph::setSourceName(const std::string& newSourceName)
{
    sourceName = newSourceName;
}

message my_graph::readGraph()
{
    if (!(adjacencyList.empty()))
        return message::nonEmptyGraph;
    std::fstream source;
    source.open(sourceName, std::ios::in);
    if (!source.good())
    {
        source.close();
        return message::noFile;
    }
    if (!(source >> order >> size) || order == 0)
    {
        source.close();
        return message::wrongData;
    }
    adjacencyList.resize(order);
    unsigned initial;
    element node;
    for (unsigned i = 0; i < size; ++i)
    {
        if (!(source >> initial >> node.terminal >> node.weight))
        {
            source.close();
            adjacencyList.clear();
            order = size = 0;
            return message::wrongData;
        }
        if (initial >= order || node.terminal >= order || node.weight < 0)
        {
            source.close();
            adjacencyList.clear();
            order = size = 0;
            return message::wrongData;
        }
        adjacencyList.at(initial).push_back(node);
    }
    source.close();
    distance.resize(order);
    previous.resize(order);
    return message::allCorrect;
}

bool my_graph::empty()
{
    return !(bool(order));
}

void my_graph::clearGraph()
{
    adjacencyList.clear();
    order = size = 0;
    distance.clear();
    previous.clear();
    reference = undefined;
    sourceName = "";
}

void my_graph::printGraph()
{
    if (adjacencyList.empty())
    {
        std::cout << "The graph is empty!" << std::endl;
        return;
    }
    std::cout << "graph order (number of vertices): " << order << std::endl
        << "graph size (number of edges): " << size << std::endl;
    std::list<element>::iterator it, stop;
    for (unsigned i = 0; i < order; ++i)
    {
        stop = adjacencyList[i].end();
        for (it = adjacencyList[i].begin(); it != stop; ++it)\
            std::cout << i << "-" << it->terminal << " : " << it->weight << std::endl;
    }
    std::cout << "=====" << std::endl;
}

message my_graph::Dijkstra(unsigned vertex)
{
    if (vertex >= order)
        return message::wrongData;
    reference = vertex;
    std::vector<bool> checked(order);
    unsigned i, j, nearest;
    for (i = 0; i < order; ++i)
    {
        checked[i] = false;
        distance[i] = infinity;
        previous[i] = undefined;
    }
    distance[reference] = 0;
    for (i = 0; i < order; ++i)
    {
        nearest = undefined;
        for (j = 0; j < order; ++j)
        {
            if (checked[j])
                continue;
            if (nearest == undefined)
                nearest = j;
            if (distance[j] < distance[nearest])
                nearest = j;
        }
        checked[nearest] = true;

        std::list<element>::iterator neighbour = adjacencyList[nearest].begin(),
            stop = adjacencyList[nearest].end();
        while (neighbour != stop)
        {
            if (!(checked[neighbour->terminal]))
            {
                if (distance[neighbour->terminal] > distance[nearest] + neighbour->weight)
                {
                    distance[neighbour->terminal] = distance[nearest] + neighbour->weight;
                    previous[neighbour->terminal] = nearest;
                }
            }
            ++neighbour;
        }
    }
    return message::allCorrect;
}

void my_graph::DijkstraResults()
{
    if (reference == undefined)
    {
        std::cout << "The Dijkstra\'s algorithm has not been performed on this graph." << std::endl;
        return;
    }
    unsigned i; signed j;
    std::cout << std::endl;
    std::cout << "The arrays resulting from the Dijkstra\'s algorithm:" << std::endl;
    std::cout << "vertex:   ";
    for (i = 0; i < order; ++i)
        std::cout << i << "\t";
    std::cout << std::endl << "distance: ";
    for (i = 0; i < order; ++i)
        if (distance[i] == infinity)
            std::cout << "inf." << "\t";
        else
            std::cout << distance[i] << "\t";
    std::cout << std::endl << "previous: ";
    for (i = 0; i < order; ++i)
        if (previous[i] == undefined)
            std::cout << "undf." << "\t";
        else
            std::cout << previous[i] << "\t";
    std::cout << std::endl << std::endl;
    std::cout << "Particular paths:" << std::endl;
    std::vector<int> path(order);
    int precursor;
    for (i = 0; i < order; ++i)
    {
        if (i == reference)
            continue;
        std::cout << "path " << reference << "-" << i << ": ";
        path[0] = i;
        j = 0;
        do {
            precursor = previous[path[j]];
            path[++j] = precursor;
        } while (path[j] != reference && path[j] != undefined);
        if (path[j] == undefined)
            std::cout << "no path";
        else
        {
            while (j >= 0)
            {
                std::cout << path[j--];
                if (j < 0)
                    std::cout << "; ";
                else
                    std::cout << ", ";
            }
            std::cout << "cost: " << distance[i];
        }
        std::cout << std::endl;
    }
    std::cout << "=====" << std::endl;
}

message my_graph::DijkstraSerialise(std::string& name)
{
    std::fstream file;
    file.open(name, std::fstream::out);
    if (!file.good())
    {
        file.close();
        if (!(std::filesystem::create_directory("Serialisation")))
        {
            file.close();
            return message::noFile;
        }
        file.open(name, std::fstream::out);
        if (!file.good())
        {
            file.close();
            return message::noFile;
        }
    }
    if (reference != undefined)
        for (unsigned i = 0; i < order; ++i)
        {
            if (distance[i] == infinity)
                file << "infinity ";
            else
                file << distance[i] << " ";
        }
    file.close();
    return message::allCorrect;
}