<div align="center">

# NTU Timetabling System

### Console app for managing university session schedules

[![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Standard Library](https://img.shields.io/badge/IO-CSV%20files-2d3748?style=for-the-badge)]()
[![Platform](https://img.shields.io/badge/platform-terminal-64748b?style=for-the-badge)]()

<br/>

[![Roles](https://img.shields.io/badge/roles-Admin%20%7C%20Student-22c55e?style=flat-square)]()
[![Persistence](https://img.shields.io/badge/persistence-CSV-3b82f6?style=flat-square)]()
[![Conflicts](https://img.shields.io/badge/conflict-room%20%7C%20lecturer%20%7C%20group-f59e0b?style=flat-square)]()

<br/>

[![GitHub last commit](https://img.shields.io/github/last-commit/hackerbotfz/Terminal-University-timetable-system?style=flat-square&logo=github)](https://github.com/hackerbotfz/Terminal-University-timetable-system/commits)
[![GitHub repo size](https://img.shields.io/github/repo-size/hackerbotfz/Terminal-University-timetable-system?style=flat-square&logo=github)](https://github.com/hackerbotfz/Terminal-University-timetable-system)
[![GitHub stars](https://img.shields.io/github/stars/hackerbotfz/Terminal-University-timetable-system?style=flat-square&logo=github)](https://github.com/hackerbotfz/Terminal-University-timetable-system/stargazers)

<br/>

**[Faiz Lawan](https://github.com/hackerbotfz)**

</div>

---

**NTU Timetabling System** is a C++ command-line application for scheduling university teaching sessions. Admins can add, edit, and delete sessions with conflict checks; students can browse, search, and export weekly timetables. Data persists in `data/timetable.csv`.

## Overview

| Detail | Value |
|--------|--------|
| **Interface** | Terminal menu (Admin / Student) |
| **Sessions** | Module, room, lecturer, type, group, week, day, hour |
| **Storage** | CSV load on startup, save on exit |
| **Validation** | Room, lecturer, and group clash detection |
| **Export** | Per-week CSV files (`Week_N_Timetable.csv`) |

## Architecture

```mermaid
flowchart LR
    Main[main.cpp] --> Timetable[Timetable class]
    Timetable --> Session[Session model]
    Timetable --> CSV[data/timetable.csv]
    Main --> Admin[Admin menu]
    Main --> Student[Student menu]
```

`Session` holds one timetable entry. `Timetable` loads and saves the CSV store, enforces scheduling conflicts, and powers search, view, and export operations.

## Tech stack

C++17 · Standard library (`fstream`, `vector`, `algorithm`)

## Build & run

```bash
g++ -std=c++17 -o timetable src/main.cpp
cp data/timetable.example.csv data/timetable.csv
./timetable
```

On Windows (MinGW or MSVC):

```bash
g++ -std=c++17 -o timetable.exe src/main.cpp
copy data\timetable.example.csv data\timetable.csv
timetable.exe
```

## Repository

```
uni_timetable/
├── src/
│   └── main.cpp
├── data/
│   └── timetable.example.csv
├── .gitignore
├── LICENSE
└── README.md
```

## License

© Faiz Lawan. See [LICENSE](LICENSE).
