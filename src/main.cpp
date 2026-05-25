#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Session {
public:
    string moduleCode, roomName, lecturer, type, group;
    int week, day, hour;

    Session(string mCode, string rName, string lect, string t, string g, int w,
            int d, int h) {
        if (w < 1 || w > 52) throw invalid_argument("Invalid week number");
        if (d < 1 || d > 7) throw invalid_argument("Invalid day number");
        if (h < 0 || h > 23) throw invalid_argument("Invalid hour");
        if (mCode.empty()) throw invalid_argument("Module code cannot be empty");

        moduleCode = mCode;
        roomName = rName;
        lecturer = lect;
        type = t;
        group = g;
        week = w;
        day = d;
        hour = h;
    }

    string toCSV() const {
        return moduleCode + "," + roomName + "," + lecturer + "," + type + "," +
               group + "," + to_string(week) + "," + to_string(day) + "," +
               to_string(hour);
    }
};

class Timetable {
private:
    vector<Session> sessions;
    const string dataFile = "data/timetable.csv";

    static int parseIntField(const string& value, const string& fieldName) {
        try {
            size_t pos = 0;
            int parsed = stoi(value, &pos);
            if (pos != value.size()) {
                throw invalid_argument("Invalid " + fieldName);
            }
            return parsed;
        } catch (const exception&) {
            throw invalid_argument("Invalid " + fieldName);
        }
    }

    void loadFromFile() {
        try {
            ifstream file(dataFile);
            if (!file.is_open()) return;

            string line;
            while (getline(file, line)) {
                if (line.empty()) continue;

                stringstream ss(line);
                string moduleCode, roomName, lecturer, type, group;
                string weekStr, dayStr, hourStr;

                getline(ss, moduleCode, ',');
                getline(ss, roomName, ',');
                getline(ss, lecturer, ',');
                getline(ss, type, ',');
                getline(ss, group, ',');
                getline(ss, weekStr, ',');
                getline(ss, dayStr, ',');
                getline(ss, hourStr, ',');

                int week = parseIntField(weekStr, "week number");
                int day = parseIntField(dayStr, "day number");
                int hour = parseIntField(hourStr, "hour");

                sessions.emplace_back(moduleCode, roomName, lecturer, type, group,
                                      week, day, hour);
            }
        } catch (const exception& e) {
            throw runtime_error("Failed to load timetable: " + string(e.what()));
        }
    }

    void saveToFile() const {
        try {
            ofstream file(dataFile, ios::trunc);
            if (!file.is_open())
                throw runtime_error("Cannot open file for writing");

            for (const auto& s : sessions)
                file << s.toCSV() << endl;
        } catch (const exception& e) {
            throw runtime_error("Failed to save timetable: " + string(e.what()));
        }
    }

public:
    Timetable() { loadFromFile(); }
    ~Timetable() {
        try {
            saveToFile();
        } catch (const exception& e) {
            cerr << "Error during save: " << e.what() << endl;
        }
    }

    bool addSession(const Session& s) {
        for (const auto& existing : sessions) {
            if (existing.week == s.week && existing.day == s.day &&
                existing.hour == s.hour) {
                if (existing.roomName == s.roomName)
                    throw invalid_argument("Room conflict detected");
                if (existing.lecturer == s.lecturer)
                    throw invalid_argument("Lecturer conflict detected");
                if (existing.group == s.group)
                    throw invalid_argument("Group conflict detected");
            }
        }
        sessions.push_back(s);
        return true;
    }

    bool editSession(const string& moduleCode) {
        try {
            for (auto& s : sessions) {
                if (s.moduleCode == moduleCode) {
                    string roomName, lecturer, type, group;
                    int week, day, hour;

                    cout << "New Room: ";
                    getline(cin >> ws, roomName);

                    cout << "New Lecturer: ";
                    getline(cin, lecturer);

                    cout << "New Type (Lecture/Lab): ";
                    getline(cin, type);

                    cout << "New Group: ";
                    getline(cin, group);

                    cout << "New Week (1-52): ";
                    if (!(cin >> week) || week < 1 || week > 52)
                        throw invalid_argument("Invalid week number");

                    cout << "New Day (1-7): ";
                    if (!(cin >> day) || day < 1 || day > 7)
                        throw invalid_argument("Invalid day number");

                    cout << "New Hour (0-23): ";
                    if (!(cin >> hour) || hour < 0 || hour > 23)
                        throw invalid_argument("Invalid hour");

                    s = Session(moduleCode, roomName, lecturer, type, group, week,
                                day, hour);
                    return true;
                }
            }
            throw invalid_argument("Module not found");
        } catch (const exception& e) {
            cerr << "Error editing session: " << e.what() << endl;
            return false;
        }
    }

    bool deleteSession(const string& moduleCode) {
        size_t originalSize = sessions.size();
        sessions.erase(remove_if(sessions.begin(), sessions.end(),
                                 [&](const Session& s) {
                                     return s.moduleCode == moduleCode;
                                 }),
                       sessions.end());
        return sessions.size() < originalSize;
    }

    void searchSessions(const string& keyword) const {
        cout << "\nSearch results for: " << keyword << endl;
        bool found = false;
        for (const auto& s : sessions) {
            if (s.moduleCode == keyword || s.roomName == keyword ||
                s.lecturer == keyword || s.group == keyword) {
                cout << setw(10) << s.moduleCode << " | " << setw(10) << s.roomName
                     << " | " << setw(20) << s.lecturer << " | " << setw(8) << s.type
                     << " | " << setw(8) << s.group << " | Week " << setw(2) << s.week
                     << " Day " << setw(1) << s.day << " Hour " << setw(2) << s.hour
                     << endl;
                found = true;
            }
        }
        if (!found) cout << "No matches found.\n";
    }

    void exportTimetable(int week) const {
        if (week < 1 || week > 52)
            throw invalid_argument("Invalid week number");

        string filename = "Week_" + to_string(week) + "_Timetable.csv";
        ofstream file(filename);
        if (!file.is_open())
            throw runtime_error("Cannot create export file");

        for (const auto& s : sessions) {
            if (s.week == week)
                file << s.toCSV() << endl;
        }
        cout << "Timetable exported to " << filename << endl;
    }

    void viewTimetable(int week) const {
        cout << "\n--- Timetable for Week " << week << " ---\n";
        bool found = false;
        for (const auto& s : sessions) {
            if (s.week == week) {
                cout << setw(10) << s.moduleCode << " | " << setw(8) << s.type
                     << " | " << setw(10) << s.roomName << " | " << setw(20)
                     << s.lecturer << " | Group: " << setw(8) << s.group
                     << " | Day: " << setw(1) << s.day << " Hour: " << setw(2)
                     << s.hour << endl;
                found = true;
            }
        }
        if (!found) cout << "No sessions found for week " << week << endl;
    }
};

void adminMenu(Timetable& t) {
    int choice;
    do {
        cout << "\n=== Admin Menu ===\n"
             << "1. Add Session\n"
             << "2. Edit Session\n"
             << "3. Delete Session\n"
             << "4. View Timetable\n"
             << "5. Search Sessions\n"
             << "0. Logout\n"
             << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    string moduleCode, roomName, lecturer, type, group;
                    int week, day, hour;

                    cout << "Module Code: ";
                    cin >> moduleCode;
                    cout << "Room: ";
                    cin >> roomName;
                    cout << "Lecturer: ";
                    cin.ignore();
                    getline(cin, lecturer);
                    cout << "Type (Lecture/Lab): ";
                    cin >> type;
                    cout << "Group: ";
                    cin >> group;
                    cout << "Week (1-52): ";
                    cin >> week;
                    cout << "Day (1-7): ";
                    cin >> day;
                    cout << "Hour (0-23): ";
                    cin >> hour;

                    t.addSession(Session(moduleCode, roomName, lecturer, type, group,
                                         week, day, hour));
                    break;
                }
                case 2: {
                    string moduleCode;
                    cout << "Enter Module Code to Edit: ";
                    cin >> moduleCode;
                    t.editSession(moduleCode);
                    break;
                }
                case 3: {
                    string moduleCode;
                    cout << "Enter Module Code to Delete: ";
                    cin >> moduleCode;
                    if (t.deleteSession(moduleCode))
                        cout << "Session deleted successfully.\n";
                    else
                        cout << "Session not found.\n";
                    break;
                }
                case 4: {
                    int week;
                    cout << "Week Number (1-52): ";
                    cin >> week;
                    t.viewTimetable(week);
                    break;
                }
                case 5: {
                    string keyword;
                    cout << "Search by module, room, lecturer, or group: ";
                    cin >> keyword;
                    t.searchSessions(keyword);
                    break;
                }
                case 0:
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } while (choice != 0);
}

void studentMenu(Timetable& t) {
    int choice;
    do {
        cout << "\n=== Student Menu ===\n"
             << "1. View Timetable\n"
             << "2. Search Timetable\n"
             << "3. Export Timetable\n"
             << "0. Logout\n"
             << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    int week;
                    cout << "Enter Week Number (1-52): ";
                    cin >> week;
                    t.viewTimetable(week);
                    break;
                }
                case 2: {
                    string keyword;
                    cout << "Search by module, room, lecturer, or group: ";
                    cin >> keyword;
                    t.searchSessions(keyword);
                    break;
                }
                case 3: {
                    int week;
                    cout << "Enter Week Number to Export (1-52): ";
                    cin >> week;
                    t.exportTimetable(week);
                    break;
                }
                case 0:
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } while (choice != 0);
}

int main() {
    try {
        Timetable timetable;
        int userType;
        cout << "=== NTU Timetabling System ===\n";

        do {
            cout << "\nSelect User Type:\n"
                 << "1. Admin\n"
                 << "2. Student\n"
                 << "0. Exit\n"
                 << "Choice: ";

            if (!(cin >> userType)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (userType) {
                case 1:
                    adminMenu(timetable);
                    break;
                case 2:
                    studentMenu(timetable);
                    break;
                case 0:
                    cout << "Exiting system.\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (userType != 0);

        return 0;
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
}
