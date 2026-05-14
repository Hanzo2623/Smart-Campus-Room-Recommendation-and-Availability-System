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
void initData() {
    rooms = {
        {"Room 101",     30,  {"projector","whiteboard","air conditioning"},                {"08:00-09:00","10:00-11:00"}},
        {"Room 202",     50,  {"projector","sound system","air conditioning"},              {"09:00-10:00","13:00-14:00"}},
        {"Lab 301",      40,  {"computers","projector","air conditioning"},                 {"07:00-08:00","11:00-12:00"}},
        {"Conference A", 20,  {"whiteboard","tv screen","air conditioning"},                {"14:00-15:00"}},
        {"Seminar Hall", 100, {"projector","sound system","whiteboard","air conditioning"}, {"15:00-17:00"}}
    };
}
void showMenu() {
    cout << "\n";
    cout << string(58, '=') << "\n";
    cout << "  SMART CAMPUS ROOM AVAILABILITY AND RECOMMENDATION SYSTEM\n";
    cout << "  Batangas State University | CC 102\n";
    cout << string(58, '=') << "\n";
    cout << "\n  [1] View Room Summary\n";
    cout << "  [2] Add Room Schedule\n";
    cout << "  [3] Check Room Availability\n";
    cout << "  [4] Recommend Best Room\n";
    cout << "  [0] Exit\n\n";
    cout << string(58, '=') << "\n";
    cout << "  Rooms in system: " << rooms.size() << "\n";
    cout << string(58, '=') << "\n";
    cout << "\n  Choice: ";
}
void viewSummary() {
    cout << "\n";
    cout << string(58, '=') << "\n";
    cout << "  ROOM SUMMARY  (" << rooms.size() << " rooms)\n";
    cout << string(58, '=') << "\n";

    for (const Room& r : rooms) {
        cout << "\n  Room     : " << r.name << "\n";
        cout << "  Capacity : " << r.capacity << " seats\n";
        cout << "  Features : " << joinStrings(r.features) << "\n";
        cout << "  Schedule : "
             << (r.schedules.empty() ? "None" : joinStrings(r.schedules))
             << "\n";
        cout << string(58, '-') << "\n";
    }

    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void addSchedule() {
    cout << "\n";
    cout << string(58, '=') << "\n";
    cout << "  ADD ROOM SCHEDULE\n";
    cout << string(58, '=') << "\n";

    cout << "\n  Room Name  : ";
    string name;
    getline(cin, name);
    name = trim(name);
    if (name.empty()) {
        cout << "  [!] Name cannot be empty.\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    int cap;
    while (true) {
        cout << "  Number of Users  : ";
        if (cin >> cap && cap > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Enter a valid positive number.\n";
    }
    cout << "  Features   : (e.g. projector, whiteboard)\n  > ";
    string featInput;
    getline(cin, featInput);

    cout << "  Time Slot : (e.g. 08:00-09:00)\n  > ";
    string slot;
    getline(cin, slot);
    slot = trim(slot);

    string err;
    if (!validSlot(slot, err)) {
        cout << "  [!] " << err << "\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    Room* r = findRoom(name);
    if (r) {
        string conflict;
        if (hasConflict(r->schedules, slot, conflict)) {
            cout << "\n  [!] Conflict with existing slot: " << conflict << "\n";
            cout << "\n  Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return;
        }
        r->schedules.push_back(slot);
        sort(r->schedules.begin(), r->schedules.end());
        cout << "\n  [OK] Slot '" << slot << "' added to '" << r->name << "'.\n";
    } else {
        rooms.push_back({name, cap, splitComma(featInput), {slot}});
        cout << "\n  [OK] Room '" << name << "' created with slot '" << slot << "'.\n";
    }
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void checkAvailability() {
    cout << "\n";
    cout << string(58, '=') << "\n";
    cout << "  CHECK ROOM AVAILABILITY\n";
    cout << string(58, '=') << "\n";

    cout << "\n  Rooms: ";
    for (size_t i = 0; i < rooms.size(); i++)
        cout << rooms[i].name << (i < rooms.size() - 1 ? ", " : "");
    cout << "\n\n";
    cout << "  Room Name : ";
    string name;
    getline(cin, name);
    name = trim(name);
    Room* r = findRoom(name);
    if (!r) {
        cout << "  [!] Room not found.\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    cout << "  Time Slot : (e.g. 09:00-10:00)\n  > ";
    string slot;
    getline(cin, slot);
    slot = trim(slot);

    string err;
    if (!validSlot(slot, err)) {
        cout << "  [!] " << err << "\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    string conflict;
    bool occupied = hasConflict(r->schedules, slot, conflict);

    cout << "\n";
    cout << string(58, '-') << "\n";
    cout << "  Room   : " << r->name << "\n";
    cout << "  Slot   : " << slot << "\n";
    cout << "  Status : "
         << (occupied ? "OCCUPIED (conflicts with " + conflict + ")" : "AVAILABLE")
         << "\n";
    cout << string(58, '-') << "\n";

    if (!r->schedules.empty())
        cout << "\n  Booked slots: " << joinStrings(r->schedules) << "\n";

    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
void recommendRoom() {
    cout << "\n";
    cout << string(58, '=') << "\n";
    cout << "  RECOMMEND BEST ROOM\n";
    cout << string(58, '=') << "\n";
    int users;
    while (true) {
        cout << "\n  No. of Users   : ";
        if (cin >> users && users > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Enter a valid positive number.\n";
    }
    cout << "  Feature needed : (press Enter to skip)\n  > ";
    string feat;
    getline(cin, feat);
    feat = toLower(trim(feat));

    cout << "  Time Slot      : (e.g. 09:00-10:00)\n  > ";
    string slot;
    getline(cin, slot);
    slot = trim(slot);
    string err;
    if (!validSlot(slot, err)) {
        cout << "  [!] " << err << "\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
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
    cout << "\n";
    cout << string(58, '-') << "\n";

    if (list.empty()) {
        cout << "  [!] No suitable room found.\n";
        cout << string(58, '-') << "\n";
        cout << "\n  Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    sort(list.begin(), list.end(), [](const Candidate& a, const Candidate& b) {
        return a.score > b.score;
    });
    cout << "  TOP RECOMMENDATION\n";
    cout << string(58, '-') << "\n";
    cout << "  Room     : " << list[0].name << "\n";
    cout << "  Capacity : " << list[0].cap << " seats\n";
    cout << "  Score    : " << list[0].score << "\n";
    cout << "  Features : " << joinStrings(list[0].features) << "\n";
    if (list.size() > 1) {
        cout << "\n  ALL RANKED MATCHES:\n";
        cout << string(58, '-') << "\n";
        cout << left << setw(5) << "Rank" << setw(18) << "Room"
             << setw(8) << "Score" << "Capacity\n";
        cout << string(58, '-') << "\n";
        for (size_t i = 0; i < list.size(); i++)
            cout << left << setw(5)  << ("#" + to_string(i + 1))
                         << setw(18) << list[i].name
                         << setw(8)  << list[i].score
                         << list[i].cap << " seats\n";
    }
    cout << string(58, '-') << "\n";
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
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
                cout << "\n  Press Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
        }
    }
}
