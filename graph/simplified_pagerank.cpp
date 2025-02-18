/**
* @file
* @brief [Simplified Page Rank Algorithm]
*
* @author [Brian Delaney](https://github.com/BrianTDelaney)
*
* @details
* Note for TA: I referenced my own PageRank project throughout the creation of this
* contribution. All code referenced is my own.
* ---------------------------------------------------------------------------------
* PageRank is an algorithm created by Google to rank the results displayed
* for a given search. For this algorithm, vertices will be web pages and edges
* will be links between them. Essentially one first navigates through an adjacency
* list and assigns each connection (edge) between two pages (vertices) with a 1
* if a link between the two is present and a 0 otherwise. These 1s are then
* divided by the total number of outgoing connections from the origin vertex, so
* a connection's importance is determined by how many links are present in the
* page that the link is from. This initial adjacency list is multiplied by a
* vector with the same amount of vertices as the adjacency list. The result of
* this multiplication then takes the place of the vector. This process is
* repeated however many times a user would choose to have it repeat, called the
* power iteration. In a non-simplified version of this algorithm the process would
* repeat until the adjacency list were to converge.
*/

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

/**
 * @class graph
 * @brief adjacency list graph implementation and pagerank algorithm
 */

class graph {
private:
   /**
    * @brief adjacency list of edges of graph
    */
   std::map<std::string, std::vector<std::string>> adj;
   /**
    * @brief weighted adjacency list
    */
   std::map<std::string, std::vector<std::pair<std::string, float>>> rankMap;
   /**
    * @brief maps vertex names (e.g. b, google, gmail.com, etc) to an index
    */
   std::map<std::string, int> keys;

public:
   /**
    * @brief Function inserts an edge between vertices in the graph
    *
    * @param from any vertex in the graph
    * @param to any vertex in the graph
    */
   void insert(std::string from, std::string to) {
       /// edge is added to the adjacency list
       adj[from].push_back(to);

       /// new vertices are added to the key map
       if (keys.count(from) == 0) {
           keys[from] = keys.size() + 1;
       }
       if (keys.count(to) == 0) {
           keys[to] = keys.size() + 1;
           adj[to] = std::vector<std::string>{};
       }
   }

   /**
    * @brief Function performs the PageRank algorithm on the adjacency list.
    *
    * @param n number of iterations of the algorithm
    *
    * @return string listing each vertex and its calculated weight with a precision
    * of two decimal places
    */
   std::string pageRank(int n) {
       /// for every vertex and its edges
       for (auto row : adj) {
           /// for every vertex linked to by the previous vertex
           for (auto edge : row.second) {
               /// the vertex's out-degree is taken
               float outDegree = (float)adj[row.first].size();

               /// the rank of that vertex is 1 over the out-degree of the vertex that links to it
               /// that rank is then assigned to said vertex in rankMap
               float rank = (float)1 / outDegree;
               rankMap[edge].push_back(make_pair(row.first, rank));
           }
       }

       /// a vector as large as the number of nodes, each element being one over that number
       std::vector<float> ranks(adj.size(), 1.0 / (float)adj.size());

       /// a variable to store the vector "ranks" while performing calculations
       std::vector<float> temp(adj.size(), 0);
       std::string result;
       std::stringstream resultStream;
       std::string website;

       /// for the number of iterations given
       for (int i = 1; i < n; i++) {
           /// for every vertex and its edges
           for (auto row : rankMap) {
               float fraction = 0.0;
               /// for every vertex linked to by the previous vertex
               for (auto edge : row.second) {
                   /// weight is multiplied by corresponding entry in rank vector and added to total for origin vertex
                   fraction += edge.second * ranks[keys[edge.first] - 1];
               }

               /// total updated entry is added to temp variable storing next iteration of the rank vector
               temp[keys[row.first] - 1] = fraction;
           }

           /// rank vector is moved to the next iteration
           ranks = temp;
       }

       std::vector<std::pair<std::string, float>> sortableResult;

       /// each vertex and its rank are added to a vector of pairs for sorting
       for (auto row : adj) {
           sortableResult.push_back(
               make_pair(row.first, ranks[keys[row.first] - 1]));
       }
       sort(sortableResult.begin(), sortableResult.end());

       /// a string is made using a stringstream for better precision control
       for (auto pair : sortableResult){
           resultStream << std::setprecision(2) << std::fixed << pair.first << " " <<
           pair.second << "\n";
       }

       /// return converted string version of result
       std::string stringResult = resultStream.str();
       return stringResult;
   }
};

void tests() {
    std::cout << "Running Test 1...\n";
   graph test1;
   test1.insert("a", "b");
   test1.insert("c", "d");
   test1.insert("e", "f");
   test1.insert("g", "h");
   test1.insert("i", "j");

   std::string expectedOutput = "a 0.00\nb 0.00\nc 0.00\nd 0.00\ne 0.00\nf 0.00\n"
       "g 0.00\nh 0.00\ni 0.00\nj 0.00\n";

   std::string actualOutput = test1.pageRank(3);

//   cout << "Test 1\n" << "Expected:\n" << expectedOutput << "Actual:\n" << actualOutput;

   assert(expectedOutput == actualOutput);
   std::cout << "Test 1 passed!\n\n";

   std::cout << "Running Test 2...\n";
   graph test2;
   test2.insert("a", "b");
   test2.insert("b", "c");
   test2.insert("c", "d");
   test2.insert("d", "a");

   expectedOutput = "a 0.25\nb 0.25\nc 0.25\nd 0.25\n";
   actualOutput = test2.pageRank(3);

//   cout << "\nTest 2\n" << "Expected:\n" << expectedOutput << "Actual:\n" << actualOutput;
   assert(expectedOutput == actualOutput);
   std::cout << "Test 2 passed!\n\n";

   std::cout << "Running Test 3...\n";
   graph test3;
   test3.insert("a", "b");
   test3.insert("a", "c");
   test3.insert("d", "e");
   test3.insert("e", "a");
   test3.insert("e", "b");
   test3.insert("c", "d");
   test3.insert("b", "c");

   expectedOutput = "a 0.12\nb 0.18\nc 0.22\nd 0.23\ne 0.25\n";
   actualOutput = test3.pageRank(30);

//   cout << "\nTest 3\n" << "Expected:\n" << expectedOutput << "Actual:\n" << actualOutput;
   assert(expectedOutput == actualOutput);
   std::cout << "Test 3 passed!\n\n";
}

int main() {
   tests();

   graph userGraph;
   int n, p;
   std::string from, to;

   std::cout << "Enter the number of links between pages:\n";
   std::cin >> n;
   std::cout << "Enter the number of power iterations:\n";
   std::cin >> p;

   for (int i = 0; i < n; i++) {
       std::cout << "Link " << i + 1 << std::endl;
       std::cout << "Enter the web page containing the link:\n";
       std::cin >> from;
       std::cout << "Enter the web page that the link leads to:\n";
       std::cin >> to;
       userGraph.insert(from, to);
   }

   std::cout << "Result:\n" << userGraph.pageRank(p);

}
