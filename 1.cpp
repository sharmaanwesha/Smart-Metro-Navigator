#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "Graph.h"
#include "User.h"
#include "Utils.h"

using namespace std;

// Display Functions
void displayMainMenu() {
  cout << "\n";
  printLine();
  cout << MAGENTA << setw(55) << "MAIN MENU" << RESET << endl;
  printLine();
  cout << GREEN << "1. " << RESET << "List All Stations\n";
  cout << GREEN << "2. " << RESET << "Show Metro Map\n";
  cout << GREEN << "3. " << RESET << "Get Shortest Distance\n";
  cout << GREEN << "4. " << RESET << "Get Shortest Path (Distance-wise)\n";
  cout << GREEN << "5. " << RESET << "Get Shortest Path (Time-wise)\n";
  cout << GREEN << "6. " << RESET << "Calculate Fare\n";
  cout << GREEN << "7. " << RESET << "Submit Feedback\n";
  cout << GREEN << "8. " << RESET << "Get Complete Route Details\n";
  cout << GREEN << "9. " << RESET << "Exit\n";
  printSmallLine();
}

void displayFeedback() {
  cout << "\n" << YELLOW << "===== FEEDBACK FORM =====" << RESET << endl;
  printSmallLine();

  string name, email, feedback;

  cout << "Enter your name: ";
  getline(cin, name);
  cout << "Enter your email: ";
  getline(cin, email);
  cout << "Your feedback: ";
  getline(cin, feedback);

  cout << "\n"
       << GREEN << "Thank you for your feedback, " << name << "!" << RESET
       << endl;
  cout << GREEN << "We appreciate your input to improve our service." << RESET
       << endl;
  waitForEnter();
}

// Simplified Station Input
bool getStationInput(Graph_M &g, string &st1, string &st2) {
  vector<string> keys = g.getKeys();
  vector<string> codes = g.printCodeList();

  int choice;
  while (true) {
    cout << "\n" << YELLOW << "Select input method:" << RESET << endl;
    cout << GREEN << "1. " << RESET << "Station Serial Number\n";
    cout << GREEN << "2. " << RESET << "Station Code\n";
    cout << GREEN << "3. " << RESET << "Station Name\n";
    cout << GREEN << "4. " << RESET << "Back to Main Menu\n";
    cout << "\n" << YELLOW << "Your choice: " << RESET;

    if (cin >> choice) {
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline
      if (choice >= 1 && choice <= 4) {
        break;
      } else {
        cout << RED << "Invalid choice! Please enter 1-4." << RESET << endl;
      }
    } else {
      clearInputBuffer();
      cout << RED << "Invalid input! Please enter a number." << RESET << endl;
    }
  }

  if (choice == 4)
    return false;

  if (choice == 1) {
    int n1, n2;
    while (true) {
      cout << CYAN << "Enter source station number (1-" << keys.size()
           << "): " << RESET;
      if (cin >> n1 && n1 > 0 && n1 <= (int)keys.size()) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }
      clearInputBuffer();
      cout << RED << "Invalid number! Try again." << RESET << endl;
    }

    while (true) {
      cout << CYAN << "Enter destination station number (1-" << keys.size()
           << "): " << RESET;
      if (cin >> n2 && n2 > 0 && n2 <= (int)keys.size()) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      }
      clearInputBuffer();
      cout << RED << "Invalid number! Try again." << RESET << endl;
    }

    st1 = keys[n1 - 1];
    st2 = keys[n2 - 1];
  } else if (choice == 2) {
    string code1, code2;
    bool found1 = false, found2 = false;

    while (!found1) {
      cout << CYAN << "Enter source station code: " << RESET;
      cin >> code1;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      transform(code1.begin(), code1.end(), code1.begin(), ::toupper);

      for (size_t i = 0; i < codes.size(); i++) {
        if (code1 == codes[i]) {
          st1 = keys[i];
          found1 = true;
          break;
        }
      }
      if (!found1)
        cout << RED << "Invalid code! Try again." << RESET << endl;
    }

    while (!found2) {
      cout << CYAN << "Enter destination station code: " << RESET;
      cin >> code2;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      transform(code2.begin(), code2.end(), code2.begin(), ::toupper);

      for (size_t i = 0; i < codes.size(); i++) {
        if (code2 == codes[i]) {
          st2 = keys[i];
          found2 = true;
          break;
        }
      }
      if (!found2)
        cout << RED << "Invalid code! Try again." << RESET << endl;
    }
  } else if (choice == 3) {
    bool found1 = false, found2 = false;
    clearInputBuffer();

    while (!found1) {
      cout << CYAN
           << "Enter source station name (Smart Search Enabled): " << RESET;
      getline(cin, st1);
      string res = g.findStation(st1);
      if (!res.empty()) {
        st1 = res;
        cout << GREEN << "Selected: " << st1 << RESET << endl;
        found1 = true;
      } else
        cout << RED << "Station not found! Try again." << RESET << endl;
    }

    while (!found2) {
      cout << CYAN << "Enter destination station name (Smart Search Enabled): "
           << RESET;
      getline(cin, st2);
      string res = g.findStation(st2);
      if (!res.empty()) {
        st2 = res;
        cout << GREEN << "Selected: " << st2 << RESET << endl;
        found2 = true;
      } else
        cout << RED << "Station not found! Try again." << RESET << endl;
    }
  }

  return true;
}

int main() {
  Graph_M g;
  createMetroMap(g);

  displayAnimatedHeader();
  waitForEnter();

  unordered_map<string, string> users;
  bool authenticated = false;

  // Authentication Loop
  while (!authenticated) {
    displayAuthMenu();
    int authChoice;

    while (true) {
      cout << YELLOW << "Enter your choice (1-3): " << RESET;
      if (cin >> authChoice && authChoice >= 1 && authChoice <= 3) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      } else {
        clearInputBuffer();
        cout << RED << "Invalid choice! Please enter 1, 2, or 3." << RESET
             << endl;
      }
    }

    if (authChoice == 1) {
      signUp(users);
      waitForEnter();
    } else if (authChoice == 2) {
      authenticated = signIn(users);
      if (!authenticated)
        waitForEnter();
    } else if (authChoice == 3) {
      cout << "\n"
           << GREEN << "Thank you for visiting Delhi Metro System. Goodbye!"
           << RESET << endl;
      return 0;
    }
  }

  // Wait before showing main menu
  waitForEnter();

  // Main Menu Loop
  while (true) {
    displayMainMenu();
    int choice;

    while (true) {
      cout << YELLOW << "Enter your choice (1-9): " << RESET;
      if (cin >> choice && choice >= 1 && choice <= 9) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        break;
      } else {
        clearInputBuffer();
        cout << RED << "Invalid choice! Please enter a number between 1-9."
             << RESET << endl;
      }
    }

    if (choice == 1) {
      g.displayStations();
      waitForEnter();
    } else if (choice == 2) {
      g.displayMap();
      waitForEnter();
    } else if ((choice >= 3 && choice <= 6) || choice == 8) {
      string st1 = "", st2 = "";
      if (!getStationInput(g, st1, st2))
        continue;

      map<string, bool> processed;

      if (!g.containsVertex(st1) || !g.containsVertex(st2) ||
          !g.hasPath(st1, st2, processed)) {
        cout << RED << "\nInvalid stations or no path exists!" << RESET << endl;
      } else {
        if (choice == 3) {
          PathResult res = g.getShortestPath(st1, st2);
          cout << "\n";
          printSmallLine();
          cout << CYAN << "Source: " << RESET << st1 << endl;
          cout << CYAN << "Destination: " << RESET << st2 << endl;
          cout << YELLOW << "Shortest Distance: " << GREEN << res.cost << " km"
               << RESET << endl;
          printSmallLine();
        } else if (choice == 4) {
          PathResult res = g.getShortestPath(st1, st2);
          cout << "\n"
               << CYAN << "===== SHORTEST PATH (DISTANCE) =====" << RESET
               << endl;
          printLine();
          cout << YELLOW << "Source: " << RESET << st1 << endl;
          cout << YELLOW << "Destination: " << RESET << st2 << endl;
          cout << YELLOW << "Distance: " << GREEN << res.cost << " km" << RESET
               << endl;
          cout << YELLOW << "Interchanges: " << GREEN << res.interchanges
               << RESET << endl;
          printSmallLine();

          cout << GREEN << "START => " << RESET << res.path[0] << endl;
          for (size_t i = 1; i < res.path.size(); i++) {
            cout << "  " << CYAN << "v " << RESET << res.path[i] << endl;
          }
          cout << GREEN << "END" << RESET << endl;
          printLine();
        } else if (choice == 5) {
          PathResult res = g.getShortestPath(st1, st2, true);
          cout << "\n"
               << CYAN << "===== SHORTEST PATH (TIME) =====" << RESET << endl;
          printLine();
          cout << YELLOW << "Source: " << RESET << st1 << endl;
          cout << YELLOW << "Destination: " << RESET << st2 << endl;
          cout << YELLOW << "Time: " << GREEN << res.cost << " minutes" << RESET
               << endl;
          cout << YELLOW << "Interchanges: " << GREEN << res.interchanges
               << RESET << endl;
          printSmallLine();

          cout << GREEN << "START => " << RESET << res.path[0] << endl;
          for (size_t i = 1; i < res.path.size(); i++) {
            cout << "  " << CYAN << "v " << RESET << res.path[i] << endl;
          }
          cout << GREEN << "END" << RESET << endl;
          printLine();
        } else if (choice == 6) {
          PathResult res = g.getShortestPath(st1, st2);
          int fare = res.cost * 3;
          if (res.cost > 0 && fare < 20)
            fare = 20;
          if (fare > 70)
            fare = 70;
          if (res.cost == 0)
            fare = 0;

          cout << "\n"
               << CYAN << "===== FARE CALCULATION =====" << RESET << endl;
          printLine();
          cout << YELLOW << "Source: " << RESET << st1 << endl;
          cout << YELLOW << "Destination: " << RESET << st2 << endl;
          cout << YELLOW << "Distance: " << GREEN << res.cost << " km" << RESET
               << endl;
          cout << YELLOW << "Fare: " << GREEN << "Rs. " << fare << RESET
               << endl;
          printLine();
        } else if (choice == 8) {
          // Complete Route Details
          PathResult distRes = g.getShortestPath(st1, st2, false);
          PathResult timeRes = g.getShortestPath(st1, st2, true);

          int fare = distRes.cost * 3;
          if (distRes.cost > 0 && fare < 20)
            fare = 20;
          if (fare > 70)
            fare = 70;
          if (distRes.cost == 0)
            fare = 0;

          cout << "\n"
               << CYAN << "===== COMPLETE ROUTE DETAILS =====" << RESET << endl;
          printLine();
          cout << YELLOW << "Source: " << RESET << st1 << endl;
          cout << YELLOW << "Destination: " << RESET << st2 << endl;
          printSmallLine();

          cout << MAGENTA << "1. Distance Analysis:" << RESET << endl;
          cout << "   Shortest Distance: " << GREEN << distRes.cost << " km"
               << RESET << endl;
          cout << "   Interchanges: " << GREEN << distRes.interchanges << RESET
               << endl;

          cout << "\n" << MAGENTA << "2. Time Analysis:" << RESET << endl;
          cout << "   Estimated Time: " << GREEN << timeRes.cost << " minutes"
               << RESET << endl;

          cout << "\n" << MAGENTA << "3. Fare Analysis:" << RESET << endl;
          cout << "   Total Fare: " << GREEN << "Rs. " << fare << RESET << endl;

          printSmallLine();
          cout << MAGENTA << "Recommended Path (Shortest Distance):" << RESET
               << endl;
          cout << GREEN << "START => " << RESET << distRes.path[0] << endl;
          for (size_t i = 1; i < distRes.path.size(); i++) {
            cout << "  " << CYAN << "v " << RESET << distRes.path[i] << endl;
          }
          cout << GREEN << "END" << RESET << endl;
          printLine();
        }
      }
      waitForEnter();
    } else if (choice == 7) {
      displayFeedback();
    } else if (choice == 9) {
      cout << "\n"
           << GREEN << "Thank you for using Delhi Metro System!" << RESET
           << endl;
      cout << GREEN << "Have a safe journey!" << RESET << endl;
      cout << endl;
      return 0;
    }
  }

  return 0;
}