# Smart Campus Room Availability And Recommendation System
**Created By: Drancel Rosales, CS-1203**
\
*Final Project for the Course CC 102 | Data Structures and Algorithms*

---

## 📖 Project Overview

The Smart Campus Room Availability System is a lightweight, high-performance C++ console application designed to manage room bookings, track scheduled time slots, and recommend the best available room across a university campus.

Under the hood, the system is powered by C++ STL vectors and struct-based data modeling, ensuring efficient memory management and rapid CRUD (Create, Read, Update, Delete) operations without relying on any external libraries or databases.

---

## 🔄 How the System Works

The application provides four core operations that mirror a real-world room booking workflow:

1. **View Room Summary**: Browse all registered rooms, their capacities, features, and existing time slot bookings.
2. **Add Room Schedule**: Register a new room or append a new time slot to an existing room — with automatic conflict detection.
3. **Check Availability**: Query any room for a specific time window and instantly see if it's free or occupied.
4. **Recommend Best Room**: Input your headcount, required feature, and preferred time slot — the system scores and ranks all qualifying rooms for you.

---

## ✨ Features

* **Room Summary View**: Displays all rooms with their capacity, available features, and currently booked time slots at a glance.
* **Add Room Schedule**: Creates a new room entry or appends a time slot to an existing room. Prevents double-booking through overlap detection.
* **Conflict Detection**: Compares time intervals with precision — partial overlaps are caught, not just exact matches.
* **Smart Room Recommendation**: Filters rooms by headcount and requested feature, then scores each candidate and returns a ranked list of the best fits.
* **Availability Check**: Enter a room name and time slot to instantly determine if it is free or occupied, including which existing slot caused the conflict.
* **Robust Input Validation**: All time slots are validated against the `HH:MM-HH:MM` format. Malformed inputs, invalid times, and reversed ranges are rejected with clear error messages.

---

## 🧠 Technical Deep Dive: Why Structs and Vectors?

Rather than relying on complex class hierarchies or external data structures, this system uses a simple `Room` struct stored in a `std::vector`. This approach achieves:

* **Fast Lookup**: Linear search over a small room list is efficient and easy to maintain.
* **Dynamic Scheduling**: Each room carries its own `vector<string>` of time slots, making it easy to append, sort, and validate bookings independently.
* **Conflict Resolution**: Time slots are parsed into integer minutes and compared numerically, enabling reliable overlap detection for any two intervals.

---

## 🛠️ Technical Details

* **Language**: `C++` (Standard C++11 or higher)
* **Architecture**: Procedural programming with struct-based data modeling
* **Data Structure**: `std::vector<Room>` with nested `std::vector<string>` for schedules
* **Libraries Used**: `<iostream>`, `<string>`, `<vector>`, `<algorithm>`, `<iomanip>`, `<sstream>`, `<limits>`

---

## 🏫 Preloaded Rooms

The system comes with 5 rooms initialized at startup:

| Room | Capacity | Features |
|------|----------|----------|
| Room 101 | 30 seats | Projector, Whiteboard, Air Conditioning |
| Room 202 | 50 seats | Projector, Sound System, Air Conditioning |
| Lab 301 | 40 seats | Computers, Projector, Air Conditioning |
| Conference A | 20 seats | Whiteboard, TV Screen, Air Conditioning |
| Seminar Hall | 100 seats | Projector, Sound System, Whiteboard, Air Conditioning |

---

## 🧩 Core Data Structure

```cpp
struct Room {
    string name;              // Room identifier (e.g. "Lab 301")
    int capacity;             // Maximum number of occupants
    vector<string> features;  // Available amenities
    vector<string> schedules; // Booked time slots in HH:MM-HH:MM format
};
```

---

## 📄 Sample Output

```text
══════════════════════════════════════════════════════
  SMART CAMPUS ROOM AVAILABILITY SYSTEM
  Batangas State University | CC 102
══════════════════════════════════════════════════════

  [1] View Room Summary
  [2] Add Room Schedule
  [3] Check Room Availability
  [4] Recommend Best Room
  [0] Exit

══════════════════════════════════════════════════════
  Rooms in system: 5
──────────────────────────────────────────────────────
  Room     : Lab 301
  Capacity : 40 seats
  Features : computers, projector, air conditioning
  Schedule : 07:00-08:00, 11:00-12:00
──────────────────────────────────────────────────────
```

---

## 🚀 How to Compile and Run

If you are using a standard GCC/MinGW terminal, compile and run the application with:

```bash
# 1. Compile the program
g++ smart_campus_system.cpp -o SmartCampus

# 2. Run the executable
./SmartCampus        # Linux / macOS
SmartCampus.exe      # Windows
```

To specify the C++ standard explicitly:

```bash
g++ -std=c++17 smart_campus_system.cpp -o SmartCampus
```

---

## ⏰ Time Slot Format

All time slots must follow the format `HH:MM-HH:MM` using a 24-hour clock. The start time must always be strictly before the end time.

| Input | Status |
|-------|--------|
| `08:00-09:30` | ✅ Valid |
| `13:00-14:00` | ✅ Valid |
| `09:00-08:00` | ❌ Invalid — start is after end |
| `25:00-26:00` | ❌ Invalid — out of range |

---

## 🎮 Controls

* **[1]** View Room Summary — see all rooms, features, and current schedules.
* **[2]** Add Room Schedule — register a new room or add a time slot to an existing one.
* **[3]** Check Room Availability — verify if a room is free for your desired time window.
* **[4]** Recommend Best Room — get a ranked list of rooms that fit your needs.
* **[0]** Exit — close the application.
