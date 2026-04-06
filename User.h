#ifndef USER_H
#define USER_H

#include "Utils.h"
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// Password Validation
inline bool isPasswordStrong(const string &password) {
  if (password.length() < 8)
    return false;

  bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
  for (char c : password) {
    if (isupper(c))
      hasUpper = true;
    else if (islower(c))
      hasLower = true;
    else if (isdigit(c))
      hasDigit = true;
    else if (ispunct(c))
      hasSpecial = true;
  }
  return hasUpper && hasLower && hasDigit && hasSpecial;
}

// Authentication Functions
inline void displayAuthMenu() {
  cout << "\n";
  printLine();
  cout << MAGENTA << setw(55) << "DELHI METRO SYSTEM" << RESET << endl;
  printLine();
  cout << GREEN << "1. " << RESET << "Sign Up (New User)\n";
  cout << GREEN << "2. " << RESET << "Sign In (Existing User)\n";
  cout << GREEN << "3. " << RESET << "Exit\n";
  printSmallLine();
}

inline void signUp(unordered_map<string, string> &users) {
  string username, password;

  cout << "\n" << CYAN << "===== SIGN UP =====" << RESET << "\n\n";

  // Clear buffer before getline if we came from a menu selection
  // Clear buffer before getline if we came from a menu selection

  cout << "Enter username: ";
  getline(cin, username);

  if (users.find(username) != users.end()) {
    cout << RED << "\nUsername already exists! Please try a different username."
         << RESET << "\n";
    // waitForEnter(); // Let main loop handle the pause
    return;
  }

  while (true) {
    cout << "Enter password: ";
    getline(cin, password);

    if (!isPasswordStrong(password)) {
      cout << RED << "\nWeak password! Password must have:\n" << RESET;
      cout << "  - At least 8 characters\n";
      cout << "  - One uppercase letter\n";
      cout << "  - One lowercase letter\n";
      cout << "  - One digit\n";
      cout << "  - One special character (!@#$%^&*)\n\n";
    } else {
      break;
    }
  }

  users[username] = password;
  cout << GREEN << "\nSign-up successful! You can now sign in." << RESET
       << "\n";
}

inline bool signIn(const unordered_map<string, string> &users) {
  string username, password;

  cout << "\n" << CYAN << "===== SIGN IN =====" << RESET << "\n\n";

  cout << "Enter username: ";
  getline(cin, username);

  if (users.find(username) == users.end()) {
    cout << RED << "\nUsername not found! Please sign up first." << RESET
         << "\n";
    return false;
  }

  int attempts = 3;
  while (attempts > 0) {
    cout << "Enter password: ";
    getline(cin, password);

    if (users.at(username) == password) {
      cout << GREEN << "\nSign-in successful! Welcome, " << username << "!"
           << RESET << "\n";
      return true;
    } else {
      attempts--;
      if (attempts > 0) {
        cout << RED << "Incorrect password! " << attempts
             << " attempt(s) remaining.\n"
             << RESET;
      } else {
        cout << RED << "\nToo many failed attempts. Returning to main menu."
             << RESET << "\n";
      }
    }
  }
  return false;
}

#endif
