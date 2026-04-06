#ifndef UTILS_H
#define UTILS_H

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

#include <chrono>
#include <thread>

// ANSI Color Codes
#define RESET "\033[0m"
#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define GREY "\033[1;30m"
#define ORANGE "\033[38;5;214m"

// Utility Functions
inline void slowPrint(string text, int delayMs = 30) {
  for (char c : text) {
    cout << c << flush;
    this_thread::sleep_for(chrono::milliseconds(delayMs));
  }
  cout << endl;
}

inline void displayAnimatedHeader() {
  cout << "\n";
  cout << CYAN;
  slowPrint("  ____  _____ _     _   _ ___   __  __ _____ _____ ____   ___  ",
            2);
  slowPrint(
      " |  _ \\| ____| |   | | | |_ _| |  \\/  | ____|_   _|  _ \\ / _ \\ ", 2);
  slowPrint(" | | | |  _| | |   | |_| || |  | |\\/| |  _|   | | | |_) | | | |",
            2);
  slowPrint(" | |_| | |___| |___|  _  || |  | |  | | |___  | | |  _ <| |_| |",
            2);
  slowPrint(
      " |____/|_____|_____|_| |_|___| |_|  |_|_____| |_| |_| \\_\\\\___/ ", 2);
  slowPrint("                                                               ",
            2);
  slowPrint("              S   Y   S   T   E   M              ", 10);
  cout << RESET << endl;
}

inline void printLine() {}

inline void printSmallLine() {}

// Robust pause function
inline void waitForEnter() {
  cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
  cin.get();
}

// Helper to clear buffer if needed before waiting (use this if you suspect
// leftover garbage)
inline void clearInputBuffer() {
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper to convert string to uppercase
inline string toUpperCase(string s) {
  string res = s;
  transform(res.begin(), res.end(), res.begin(), ::toupper);
  return res;
}

#endif
