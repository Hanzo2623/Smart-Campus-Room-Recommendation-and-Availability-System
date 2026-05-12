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

void clearScreen() { cout << string(1, '\n'); }

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

    string name = readLine("\n  Room Name        : ");
    if (name.empty()) { cout << "  [!] Name cannot be empty.\n"; pause(); return; }

    int cap = readInt("  Number of Users  : ");
    string featInput = readLine("  Features  : (e.g. projector, whiteboard)\n  > ");
    string slot = readLine("  Time Slot : (e.g. 08:00-09:00)\n  > ");

    string err;
    if (!validSlot(slot, err)) { cout << "  [!] " << err << "\n"; pause(); return; }
 
    Room* r = findRoom(name);
    if (r) {
        string conflict;
        if (hasConflict(r->schedules, slot, conflict)) {
            cout << "\n  [!] Conflict with existing slot: " << conflict << "\n";
            pause(); return;
        }
        r->schedules.push_back(slot);
        sort(r->schedules.begin(), r->schedules.end());
        cout << "\n  [OK] Slot '" << slot << "' added to '" << r->name << "'.\n";
    } else {
        rooms.push_back({name, cap, splitComma(featInput), {slot}});
        cout << "\n  [OK] Room '" << name << "' created with slot '" << slot << "'.\n";
    }
    pause();
}
void checkAvailability() {
    clearScreen();
    printLine('=', 58);
    cout << "  CHECK ROOM AVAILABILITY" << endl;
    printLine('=', 58);
 
    cout << "\n  Rooms: ";
    for (size_t i = 0; i < rooms.size(); i++)
        cout << rooms[i].name << (i < rooms.size()-1 ? ", " : "");
    cout << "\n\n";
 
    string name = readLine("  Room Name : ");
    Room* r = findRoom(name);
    if (!r) { cout << "  [!] Room not found.\n"; pause(); return; }
 
    string slot = readLine("  Time Slot : (e.g. 09:00-10:00)\n  > ");
    string err;
    if (!validSlot(slot, err)) { cout << "  [!] " << err << "\n"; pause(); return; }
 
    string conflict;
    bool occupied = hasConflict(r->schedules, slot, conflict);
 
    cout << "\n"; printLine();
    cout << "  Room   : " << r->name << "\n";
    cout << "  Slot   : " << slot << "\n";
    cout << "  Status : " << (occupied ? "OCCUPIED (conflicts with " + conflict + ")" : "AVAILABLE") << "\n";
    printLine();
 
    if (!r->schedules.empty()) {
        cout << "\n  Booked slots: " << joinStrings(r->schedules) << "\n";
    }
    pause();
}
 
void recommendRoom() {
    clearScreen();
    printLine('=', 58);
    cout << "  RECOMMEND BEST ROOM" << endl;
    printLine('=', 58);
 
    int users = readInt("\n  No. of Users   : ");
    string feat = toLower(trim(readLine("  Feature needed : (press Enter to skip)\n  > ")));
    string slot = readLine("  Time Slot      : (e.g. 09:00-10:00)\n  > ");
 
    string err;
    if (!validSlot(slot, err)) { cout << "  [!] " << err << "\n"; pause(); return; }
 
    struct Candidate { string name; int cap, score; vector<string> features; };
    vector<Candidate> list;
 
    for (const Room& r : rooms) {
        string dummy;
        if (hasConflict(r.schedules, slot, dummy)) continue;
        if (r.capacity < users) continue;
        int score = 15 - min((r.capacity - users) / 10, 5);
        for (const string& f : r.features)
            if (!feat.empty() && f == feat) { score += 5; break; }
        list.push_back({r.name, r.capacity, score, r.features});
    }
 
    cout << "\n"; printLine();
 
    if (list.empty()) {
        cout << "  [!] No suitable room found.\n";
        printLine(); pause(); return;
    }
 
    sort(list.begin(), list.end(), [](const Candidate& a, const Candidate& b) {
        return a.score > b.score;
    });
 
    cout << "  TOP RECOMMENDATION\n"; printLine();
    cout << "  Room     : " << list[0].name << "\n";
    cout << "  Capacity : " << list[0].cap << " seats\n";
    cout << "  Score    : " << list[0].score << "\n";
    cout << "  Features : " << joinStrings(list[0].features) << "\n";
 
    if (list.size() > 1) {
        cout << "\n  ALL RANKED MATCHES:\n"; printLine();
        cout << left << setw(5) << "Rank" << setw(18) << "Room"
             << setw(8) << "Score" << "Capacity\n";
        printLine();
        for (size_t i = 0; i < list.size(); i++)
            cout << left << setw(5) << ("#" + to_string(i+1))
                 << setw(18) << list[i].name
                 << setw(8)  << list[i].score
                 << list[i].cap << " seats\n";
    }
    printLine();
    pause();
}
 
void showMenu() {
    clearScreen();
    printLine('=', 58);
    cout << "  SMART CAMPUS ROOM AVAILABILITY SYSTEM\n";
    cout << "  Batangas State University | CC 102\n";
    printLine('=', 58);
    cout << "\n  [1] View Room Summary\n";
    cout << "  [2] Add Room Schedule\n";
    cout << "  [3] Check Room Availability\n";
    cout << "  [4] Recommend Best Room\n";
    cout << "  [0] Exit\n\n";
    printLine('=', 58);
    cout << "  Rooms in system: " << rooms.size() << "\n";
    printLine('=', 58);
    cout << "\n  Choice: ";
}
 
int main() {
    initData();
    int choice;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch (choice) {
            case 1: viewSummary();       break;
            case 2: addSchedule();       break;
            case 3: checkAvailability(); break;
            case 4: recommendRoom();     break;
            case 0:
                cout << "\n  Goodbye!\n\n";
                return 0;
            default:
                cout << "\n  [!] Invalid choice. Enter 0-4.\n";
                pause();
        }
    }
}

    
