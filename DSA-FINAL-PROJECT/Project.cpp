#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

struct Room {
    string name;
    int capacity;
    vector<string> features;
    vector<string> schedules;
};

vector<Room> rooms;

void printLine(char c = '-', int w = 58) { cout << string(w, c) << endl; }

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void clearScreen() { cout << string(3, '\n'); }

string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t");
    size_t b = s.find_last_not_of(" \t");
    return (a == string::npos) ? "" : s.substr(a, b - a + 1);
}

string toLower(const string& s) {
    string r = s;
    for (char& c : r) c = tolower(c);
    return r;
}

vector<string> splitComma(const string& s) {
    vector<string> res;
    stringstream ss(s);
    string item;
    while (getline(ss, item, ',')) {
        string t = trim(item);
        if (!t.empty()) res.push_back(toLower(t));
    }
    return res;
}

string joinStrings(const vector<string>& v) {
    string r;
    for (size_t i = 0; i < v.size(); i++) {
        r += v[i];
        if (i < v.size() - 1) r += ", ";
    }
    return r;
}

int parseTime(const string& t) {
    if (t.size() != 5 || t[2] != ':') return -1;
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (t[i] < '0' || t[i] > '9') return -1;
    }
    int h = stoi(t.substr(0, 2)), m = stoi(t.substr(3, 2));
    return (h > 23 || m > 59) ? -1 : h * 60 + m;
}

bool validSlot(const string& slot, string& err) {
    if (slot.size() != 11 || slot[5] != '-') {
        err = "Use format HH:MM-HH:MM (e.g. 08:00-09:00).";
        return false;
    }
    int s = parseTime(slot.substr(0, 5)), e = parseTime(slot.substr(6));
    if (s == -1 || e == -1) { err = "Invalid time values."; return false; }
    if (s >= e) { err = "Start must be before end time."; return false; }
    return true;
}

bool hasConflict(const vector<string>& slots, const string& newSlot, string& found) {
    int ns = parseTime(newSlot.substr(0, 5)), ne = parseTime(newSlot.substr(6));
    for (const string& s : slots) {
        int es = parseTime(s.substr(0, 5)), ee = parseTime(s.substr(6));
        if (ns < ee && ne > es) { found = s; return true; }
    }
    return false;
}

Room* findRoom(const string& name) {
    string n = toLower(trim(name));
    for (Room& r : rooms)
        if (toLower(r.name) == n) return &r;
    return nullptr;
}

string readLine(const string& prompt) {
    cout << prompt;
    string s; getline(cin, s);
    return trim(s);
}

int readInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v > 0) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Enter a valid positive number.\n";
    }
}

void initData() {
    rooms = {
        {"Room 101",    30,  {"projector","whiteboard","air conditioning"},          {"08:00-09:00","10:00-11:00"}},
        {"Room 202",    50,  {"projector","sound system","air conditioning"},        {"09:00-10:00","13:00-14:00"}},
        {"Lab 301",     40,  {"computers","projector","air conditioning"},           {"07:00-08:00","11:00-12:00"}},
        {"Conference A",20,  {"whiteboard","tv screen","air conditioning"},          {"14:00-15:00"}},
        {"Seminar Hall",100, {"projector","sound system","whiteboard","air conditioning"},{"15:00-17:00"}}
    };
}

void viewSummary() {
    clearScreen();
    printLine('=', 58);
    cout << "  ROOM SUMMARY  (" << rooms.size() << " rooms)" << endl;
    printLine('=', 58);
    for (const Room& r : rooms) {
        cout << "\n  Room     : " << r.name << "\n";
        cout << "  Capacity : " << r.capacity << " seats\n";
        cout << "  Features : " << joinStrings(r.features) << "\n";
        cout << "  Schedule : " << (r.schedules.empty() ? "None" : joinStrings(r.schedules)) << "\n";
        printLine();
    }
    pause();
}

void addSchedule() {
    clearScreen();
    printLine('=', 58);
    cout << "  ADD ROOM SCHEDULE" << endl;
    printLine('=', 58);

    string name = readLine("\n  Room Name : ");
    if (name.empty()) { cout << "  [!] Name cannot be empty.\n"; pause(); return; }

    int cap = readInt("  Capacity  : ");
    string featInput = readLine("  Features  : (e.g. projector, whiteboard)\n  > ");
    string slot = readLine("  Time Slot : (e.g. 08:00-09:00)\n  > ");

    