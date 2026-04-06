#ifndef GRAPH_H
#define GRAPH_H

#include "Utils.h"
#include <algorithm>
#include <climits>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// Structure to hold path results cleanly
struct PathResult {
  vector<string> path;
  int cost;
  int interchanges;
};

class Graph_M {
public:
  class Vertex {
  public:
    map<string, int> nbrs;
  };

  map<string, Vertex> vtces;

  bool containsVertex(string vname) { return vtces.find(vname) != vtces.end(); }

  void addVertex(string vname) {
    Vertex vtx;
    vtces[vname] = vtx;
  }

  void addEdge(string vname1, string vname2, int value) {
    if (vtces.find(vname1) == vtces.end() ||
        vtces.find(vname2) == vtces.end()) {
      return;
    }

    Vertex &vtx1 = vtces[vname1];
    Vertex &vtx2 = vtces[vname2];

    if (vtx1.nbrs.find(vname2) != vtx1.nbrs.end()) {
      return;
    }

    vtx1.nbrs[vname2] = value;
    vtx2.nbrs[vname1] = value;
  }

  bool containsEdge(string vname1, string vname2) {
    if (vtces.find(vname1) == vtces.end() ||
        vtces.find(vname2) == vtces.end()) {
      return false;
    }

    Vertex vtx1 = vtces[vname1];
    return vtx1.nbrs.find(vname2) != vtx1.nbrs.end();
  }

  vector<string> getKeys() {
    vector<string> keys;
    for (auto it : vtces) {
      keys.push_back(it.first);
    }
    return keys;
  }

  // Smart Search for Station
  string findStation(string input) {
    if (input.empty())
      return "";

    string upperInput = toUpperCase(input);
    vector<string> keys = getKeys();

    // 1. Exact Match (Case Insensitive)
    for (string key : keys) {
      if (toUpperCase(key) == upperInput)
        return key;
    }

    // 2. Partial Match (Contains)
    for (string key : keys) {
      if (toUpperCase(key).find(upperInput) != string::npos)
        return key;
    }

    return "";
  }

  void displayStations() {
    cout << "\n" << CYAN << "===== ALL METRO STATIONS =====" << RESET << endl;
    printLine();

    vector<string> keys = getKeys();
    for (size_t i = 0; i < keys.size(); i++) {
      cout << GREEN << setw(3) << (i + 1) << ". " << RESET << keys[i] << endl;
    }

    printLine();
    cout << YELLOW << "Total Stations: " << keys.size() << RESET << endl;
  }

  string getStationColor(string stationName) {
    size_t dashPos = stationName.rfind('-');
    if (dashPos == string::npos)
      return RESET;

    string code = stationName.substr(dashPos + 1);
    if (code.length() > 1)
      return GREY; // Interchange

    char lineChar = code[0];
    switch (lineChar) {
    case 'Y':
      return YELLOW;
    case 'B':
      return BLUE;
    case 'R':
      return RED;
    case 'G':
      return GREEN;
    case 'O':
      return ORANGE; // Orange mapped to Red
    case 'P':
      return MAGENTA;
    default:
      return RESET;
    }
  }

  void displayMap() {
    cout << "\n" << CYAN << "===== DELHI METRO MAP =====" << RESET << endl;
    printLine();

    vector<string> keys = getKeys();
    for (size_t i = 0; i < keys.size(); i++) {
      string color = getStationColor(keys[i]);
      cout << "\n" << color << keys[i] << RESET << " =>" << endl;
      Vertex vtx = vtces[keys[i]];

      for (auto it : vtx.nbrs) {
        cout << "    " << GREEN << "-> " << RESET << setw(35) << left
             << it.first << CYAN << it.second << " km" << RESET << endl;
      }
    }

    printLine();
  }

  bool hasPath(string vname1, string vname2, map<string, bool> &processed) {
    if (containsEdge(vname1, vname2)) {
      return true;
    }

    processed[vname1] = true;
    Vertex vtx = vtces[vname1];

    for (auto it : vtx.nbrs) {
      if (processed.find(it.first) == processed.end()) {
        if (hasPath(it.first, vname2, processed)) {
          return true;
        }
      }
    }
    return false;
  }

  // Optimized Dijkstra using Priority Queue
  int dijkstra(string src, string dst) {
    if (src == dst)
      return 0;

    priority_queue<pair<int, string>, vector<pair<int, string>>,
                   greater<pair<int, string>>>
        pq;
    map<string, int> dist;

    for (auto it : vtces) {
      dist[it.first] = INT_MAX;
    }

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
      string u = pq.top().second;
      int d = pq.top().first;
      pq.pop();

      if (d > dist[u])
        continue;
      if (u == dst)
        return d;

      for (auto &neighbor : vtces[u].nbrs) {
        string v = neighbor.first;
        int weight = neighbor.second;

        if (dist[u] + weight < dist[v]) {
          dist[v] = dist[u] + weight;
          pq.push({dist[v], v});
        }
      }
    }
    return -1; // Should not happen if path exists
  }

  // Get Shortest Path (Distance)
  PathResult getShortestPath(string src, string dst, bool timeBased = false) {
    PathResult result;
    result.cost = 0;
    result.interchanges = 0;

    if (src == dst) {
      result.path.push_back(src);
      return result;
    }

    // Dijkstra with path reconstruction
    map<string, int> dist;
    map<string, string> parent;
    priority_queue<pair<int, string>, vector<pair<int, string>>,
                   greater<pair<int, string>>>
        pq;

    for (auto it : vtces) {
      dist[it.first] = INT_MAX;
    }

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
      string u = pq.top().second;
      int d = pq.top().first;
      pq.pop();

      if (d > dist[u])
        continue;
      if (u == dst)
        break;

      for (auto &neighbor : vtces[u].nbrs) {
        string v = neighbor.first;
        int weight = neighbor.second;

        // If timeBased, calculate time (assuming avg speed 40km/h -> 1.5 min
        // per km)
        if (timeBased) {
          weight = (weight * 60) / 40;
        }

        if (dist[u] + weight < dist[v]) {
          dist[v] = dist[u] + weight;
          parent[v] = u;
          pq.push({dist[v], v});
        }
      }
    }

    // Reconstruct path
    if (dist[dst] == INT_MAX)
      return result; // No path

    result.cost = dist[dst];
    string curr = dst;
    while (curr != src) {
      result.path.push_back(curr);
      curr = parent[curr];
    }
    result.path.push_back(src);
    reverse(result.path.begin(), result.path.end());

    // Calculate Interchanges
    // Logic: Check if line changes between stations
    // Assuming station format "Name-LineCode" (e.g., "Rajiv Chowk-BY")
    for (size_t i = 1; i < result.path.size() - 1; i++) {
      string prev = result.path[i - 1];
      string currSt = result.path[i];
      string next = result.path[i + 1];

      // Extract line codes
      size_t pDash = prev.find('-');
      size_t cDash = currSt.find('-');
      size_t nDash = next.find('-');

      if (pDash != string::npos && cDash != string::npos &&
          nDash != string::npos) {
        string pLine = prev.substr(pDash + 1);
        string cLine = currSt.substr(cDash + 1);
        string nLine = next.substr(nDash + 1);

        // If current station has multiple lines (interchange station)
        if (cLine.length() > 1) {
          // Simple heuristic: if the line 'flow' changes
          // This is a bit tricky without explicit line mapping, but let's try:
          // If the edge incoming line != edge outgoing line
          // But we don't have edge lines stored, only station lines.
          // Let's stick to the original logic: if previous line != next line
          // But we need to be careful about multi-line stations.

          // Simplified: If the common char between prev and curr is NOT the
          // common char between curr and next Actually, let's just count it as
          // interchange if the station is an interchange station (len > 1) AND
          // it's not the start or end. This is an approximation.

          // Better approximation from original code:
          // It checked if prevLine != nextLine.
          // But here we have full line strings.
          // Let's assume interchange if we switch "primary" lines.

          // Let's use the original logic's spirit:
          // If we are at an interchange station, we *might* be changing lines.
          result.interchanges++;
        }
      }
    }

    // Refine interchanges: The original code had specific logic.
    // Let's implement a cleaner version:
    // Iterate through path. If we are at a station with >1 line code, check if
    // we entered on one line and left on another. Since we don't store which
    // line the EDGE belongs to, we have to guess based on neighbors. For now,
    // let's stick to a simple counter for stations with multiple lines
    // (excluding src/dst).
    result.interchanges = 0;
    for (size_t i = 1; i < result.path.size() - 1; ++i) {
      string name = result.path[i];
      size_t dash = name.find('-');
      if (dash != string::npos) {
        string code = name.substr(dash + 1);
        if (code.length() > 1)
          result.interchanges++;
      }
    }

    return result;
  }

  vector<string> printCodeList() {
    cout << "\n" << CYAN << "===== STATION CODES =====" << RESET << endl;
    printLine();

    vector<string> keys = getKeys();
    vector<string> codes;

    for (string key : keys) {
      vector<string> words;
      string word = "";

      for (size_t i = 0; i <= key.size(); i++) {
        if (i == key.size() || key[i] == ' ') {
          if (!word.empty()) {
            words.push_back(word);
            word = "";
          }
        } else {
          word += key[i];
        }
      }

      string code = "";
      if (words.size() == 1) {
        code = words[0].substr(0, min((size_t)2, words[0].length()));
      } else {
        for (size_t i = 0; i < words.size(); i++) {
          code += words[i].substr(0, 1);
        }
      }

      transform(code.begin(), code.end(), code.begin(), ::toupper);
      codes.push_back(code);
    }

    for (size_t i = 0; i < codes.size(); i++) {
      cout << GREEN << setw(3) << (i + 1) << ". " << RESET << setw(35) << left
           << keys[i] << YELLOW << codes[i] << RESET << endl;
    }

    printLine();
    return codes;
  }
};

// Initialize Graph
inline void createMetroMap(Graph_M &g) {
  g.addVertex("Noida Sector 62-B");
  g.addVertex("Botanical Garden-B");
  g.addVertex("Yamuna Bank-B");
  g.addVertex("Rajiv Chowk-BY");
  g.addVertex("Vaishali-B");
  g.addVertex("Moti Nagar-B");
  g.addVertex("Janak Puri West-BO");
  g.addVertex("Dwarka Sector 21-B");
  g.addVertex("Huda City Centre-Y");
  g.addVertex("Saket-Y");
  g.addVertex("Vishwavidyalaya-Y");
  g.addVertex("Chandni Chowk-Y");
  g.addVertex("New Delhi-YO");
  g.addVertex("AIIMS-Y");
  g.addVertex("Shivaji Stadium-O");
  g.addVertex("DDS Campus-O");
  g.addVertex("IGI Airport-O");
  g.addVertex("Rajouri Garden-BP");
  g.addVertex("Netaji Subhash Place-PR");
  g.addVertex("Punjabi Bagh West-P");

  g.addEdge("Noida Sector 62-B", "Botanical Garden-B", 8);
  g.addEdge("Botanical Garden-B", "Yamuna Bank-B", 10);
  g.addEdge("Yamuna Bank-B", "Vaishali-B", 8);
  g.addEdge("Yamuna Bank-B", "Rajiv Chowk-BY", 6);
  g.addEdge("Rajiv Chowk-BY", "Moti Nagar-B", 9);
  g.addEdge("Moti Nagar-B", "Janak Puri West-BO", 7);
  g.addEdge("Janak Puri West-BO", "Dwarka Sector 21-B", 6);
  g.addEdge("Huda City Centre-Y", "Saket-Y", 15);
  g.addEdge("Saket-Y", "AIIMS-Y", 6);
  g.addEdge("AIIMS-Y", "Rajiv Chowk-BY", 7);
  g.addEdge("Rajiv Chowk-BY", "New Delhi-YO", 1);
  g.addEdge("New Delhi-YO", "Chandni Chowk-Y", 2);
  g.addEdge("Chandni Chowk-Y", "Vishwavidyalaya-Y", 5);
  g.addEdge("New Delhi-YO", "Shivaji Stadium-O", 2);
  g.addEdge("Shivaji Stadium-O", "DDS Campus-O", 7);
  g.addEdge("DDS Campus-O", "IGI Airport-O", 8);
  g.addEdge("Moti Nagar-B", "Rajouri Garden-BP", 2);
  g.addEdge("Punjabi Bagh West-P", "Rajouri Garden-BP", 2);
  g.addEdge("Punjabi Bagh West-P", "Netaji Subhash Place-PR", 3);
}

#endif
