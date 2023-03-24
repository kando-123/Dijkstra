/** @file */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "methods.h"

int main(int argc, char** argv)
{
    my_graph Graph;
    std::string localisation;
    message result;
    unsigned choice, ref;
    std::string serialisationName;

    std::cout << "Choose what action should be performed: " << std::endl
        << "1 = read a graph from a file," << std::endl
        << "2 = print the graph," << std::endl
        << "3 = perform the Dijkstra's algorithm for the graph," << std::endl
        << "4 = serialise the vector of distances into a text file," << std::endl
        << "5 = delete the graph" << std::endl
        << "0 = finish." << std::endl << std::endl;
    do {
        std::cout << "Your choice: ";
        std::cin >> choice;
        
        switch (choice)
        {
        case 1:
            if (!(Graph.empty()))
            {
                std::cout << "The graph is not empty. New data must not be read." << std::endl;
                break;
            }
            std::cout << "Please input the path to the source file: ";
            std::cin >> localisation;
            Graph.setSourceName(localisation);
            result = Graph.readGraph();
            if (result == message::allCorrect)
                std::cout << "The graph has been read properly." << std::endl;
            else if (result == message::noFile)
                std::cerr << "Incorrect file name!" << std::endl;
            else if (result == message::wrongData)
                std::cerr << "Incorrect data!" << std::endl;
            else if (result == message::nonEmptyGraph)
                std::cerr << "Attempt of reading new data into a nonempty graph." << std::endl;
            break;
        case 2:
            Graph.printGraph();
            break;
        case 3:
            if (Graph.empty())
            {
                std::cout << "The graph is empty!" << std::endl;
                break;
            }
            std::cout << "The reference vertex: "; std::cin >> ref;
            result = Graph.Dijkstra(ref);
            if (result == message::wrongData)
                std::cout << "The number exceeds the graph order." << std::endl;
            else
                Graph.DijkstraResults();
            break;
        case 4:
            if (Graph.empty())
            {
                std::cout << "The graph is empty!" << std::endl;
                break;
            }
            std::cout << std::endl << "Please give the name of the file. (It may include spaces.)" << std::endl
                << "It will be saved in the folder \'Serialisation\'; the extention \'.txt\' will be added automatically." << std::endl;
            getchar();
            std::getline(std::cin, serialisationName);
            serialisationName = "Serialisation/" + serialisationName + ".txt";
            result = Graph.DijkstraSerialise(serialisationName);
            if (result == message::allCorrect)
                std::cout << "The serialisation has succeeded." << std::endl;
            else
                std::cout << "The serialisation has not succeeded." << std::endl;
            break;
        case 5:
            Graph.clearGraph();
            std::cout << "The graph has been deleted." << std::endl;
            break;
        }
        std::cout << std::endl;
    } while (choice);
    Graph.clearGraph();
    return 0;
}
