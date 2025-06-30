#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

// Student class
class Student {
public:
    Student(const string& id, const string& name, double score) 
        : id_(id), name_(name), score_(score), currentMatch_(nullptr), preferenceIndex_(0) {}

    const string& getId() const { return id_; }
    double getScore() const { return score_; }
    void addPreference(class College* college) { preferences_.push_back(college); }
    const vector<class College*>& getPreferences() const { return preferences_; }
    class College* getCurrentMatch() const { return currentMatch_; }
    void setCurrentMatch(class College* college) { currentMatch_ = college; }
    
    class College* getNextPreference() {
        if (preferenceIndex_ < preferences_.size()) {
            return preferences_[preferenceIndex_++];
        }
        return nullptr;
    }
    
    void resetPreferences() { preferenceIndex_ = 0; currentMatch_ = nullptr; }

private:
    string id_;
    string name_;
    double score_;
    vector<class College*> preferences_;
    class College* currentMatch_;
    size_t preferenceIndex_;
};

// College class
class College {
public:
    College(const string& id, const string& name, int quota) 
        : id_(id), name_(name), quota_(quota) {}

    const string& getId() const { return id_; }
    int getQuota() const { return quota_; }
    void addPreference(Student* student) { preferences_.push_back(student); }
    const vector<Student*>& getPreferences() const { return preferences_; }
    const vector<Student*>& getCurrentStudents() const { return currentStudents_; }

    bool addStudent(Student* student) {
        if (currentStudents_.size() < static_cast<size_t>(quota_)) {
            currentStudents_.push_back(student);
            student->setCurrentMatch(this);
            return true;
        } else {
            Student* lowest = getLowestRankedStudent();
            if (prefers(student, lowest)) {
                currentStudents_.erase(
                    find(currentStudents_.begin(), currentStudents_.end(), lowest));
                lowest->setCurrentMatch(nullptr);
                currentStudents_.push_back(student);
                student->setCurrentMatch(this);
                return true;
            }
            return false;
        }
    }

    void removeStudent(Student* student) {
        auto it = find_if(currentStudents_.begin(), currentStudents_.end(),
                               [student](const Student* s) { return s->getId() == student->getId(); });
        if (it != currentStudents_.end()) {
            currentStudents_.erase(it);
            student->setCurrentMatch(nullptr);
        }
    }

    bool prefers(Student* s1, Student* s2) const {
        auto it1 = find_if(preferences_.begin(), preferences_.end(),
            [s1](const Student* s) { return s->getId() == s1->getId(); });
        auto it2 = find_if(preferences_.begin(), preferences_.end(),
            [s2](const Student* s) { return s->getId() == s2->getId(); });
        return it1 < it2;
    }

    Student* getLowestRankedStudent() const {
        if (currentStudents_.empty()) return nullptr;
        Student* lowest = currentStudents_[0];
        for (auto student : currentStudents_) {
            if (!prefers(lowest, student)) lowest = student;
        }
        return lowest;
    }

    bool isFull() const { return currentStudents_.size() >= static_cast<size_t>(quota_); }
    void resetStudents() { currentStudents_.clear(); }

private:
    string id_;
    string name_;
    int quota_;
    vector<Student*> preferences_;
    vector<Student*> currentStudents_;
};

// Stable Matching Algorithm class
class StableMatchingAlgorithm {
public:
    StableMatchingAlgorithm(const vector<Student*>& students, 
                           const vector<College*>& colleges, 
                           bool studentOptimal = true)
        : students_(students), colleges_(colleges), studentOptimal_(studentOptimal) {}

    map<string, string> match() {
        resetState();
        return studentOptimal_ ? studentOrientedMatch() : collegeOrientedMatch();
    }

private:
    void resetState() {
        for (auto student : students_) student->resetPreferences();
        for (auto college : colleges_) college->resetStudents();
    }

    map<string, string> studentOrientedMatch() {
        vector<Student*> unmatched = students_;
        while (!unmatched.empty()) {
            Student* student = unmatched.front();
            unmatched.erase(unmatched.begin());
            College* college = student->getNextPreference();
            if (!college) continue;
            if (college->addStudent(student)) {
                for (auto other : students_) {
                    if (other != student && other->getCurrentMatch() == nullptr &&
                        find(unmatched.begin(), unmatched.end(), other) == unmatched.end()) {
                        unmatched.push_back(other);
                    }
                }
            } else {
                unmatched.push_back(student);
            }
        }
        return buildMatching();
    }

    map<string, string> collegeOrientedMatch() {
        vector<College*> unfilled = colleges_;
        map<College*, set<Student*>> invited;
        for (auto college : colleges_) invited[college] = set<Student*>();

        while (!unfilled.empty()) {
            College* college = unfilled.front();
            unfilled.erase(unfilled.begin());
            if (college->isFull()) continue;

            Student* nextStudent = nullptr;
            for (auto student : college->getPreferences()) {
                if (invited[college].find(student) == invited[college].end()) {
                    nextStudent = student;
                    invited[college].insert(student);
                    break;
                }
            }
            if (!nextStudent) continue;

            if (!nextStudent->getCurrentMatch()) {
                college->addStudent(nextStudent);
            } else {
                College* current = nextStudent->getCurrentMatch();
                const auto& prefs = nextStudent->getPreferences();
                auto newIt = find(prefs.begin(), prefs.end(), college);
                auto curIt = find(prefs.begin(), prefs.end(), current);
                if (newIt != prefs.end() && curIt != prefs.end() && newIt < curIt) {
                    current->removeStudent(nextStudent); // Fixed: Remove only the specific student
                    college->addStudent(nextStudent);
                    if (!current->isFull() && 
                        find(unfilled.begin(), unfilled.end(), current) == unfilled.end()) {
                        unfilled.push_back(current);
                    }
                }
            }
            if (!college->isFull()) unfilled.push_back(college);
        }
        return buildMatching();
    }

    map<string, string> buildMatching() {
        map<string, string> matching;
        for (auto student : students_) {
            matching[student->getId()] = student->getCurrentMatch() ? 
                student->getCurrentMatch()->getId() : "";
        }
        return matching;
    }

    vector<Student*> students_;
    vector<College*> colleges_;
    bool studentOptimal_;
};

// Read test data from txt file
bool readTestData(const string& filename, 
                  vector<Student*>& students, 
                  vector<College*>& colleges, 
                  map<string, Student*>& studentMap, 
                  map<string, College*>& collegeMap) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open " << filename << "\n";
        return false;
    }

    string line;
    bool readingStudents = false, readingColleges = false;

    while (getline(inFile, line)) {
        if (line == "STUDENTS") {
            readingStudents = true;
            readingColleges = false;
            continue;
        } else if (line == "COLLEGES") {
            readingStudents = false;
            readingColleges = true;
            continue;
        }

        istringstream iss(line);
        string id, name;
        if (readingStudents) {
            double score;
            iss >> id >> name >> score;
            Student* student = new Student(id, name, score);
            students.push_back(student);
            studentMap[id] = student;
            string cId;
            while (iss >> cId) {
                collegeMap[cId]; // Ensure college exists in map
            }
        } else if (readingColleges) {
            int quota;
            iss >> id >> name >> quota;
            College* college = new College(id, name, quota);
            colleges.push_back(college);
            collegeMap[id] = college;
        }
    }

    inFile.close();

    // Re-read to set student preferences
    inFile.open(filename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open " << filename << "\n";
        return false;
    }

    readingStudents = false;
    int studentIndex = 0;
    while (getline(inFile, line)) {
        if (line == "STUDENTS") {
            readingStudents = true;
            continue;
        } else if (line == "COLLEGES") {
            readingStudents = false;
            continue;
        }

        if (readingStudents) {
            istringstream iss(line);
            string id, name;
            double score;
            iss >> id >> name >> score;
            string cId;
            while (iss >> cId) {
                if (collegeMap.find(cId) != collegeMap.end()) {
                    students[studentIndex]->addPreference(collegeMap[cId]);
                }
            }
            studentIndex++;
        }
    }

    inFile.close();

    // Set college preferences (sorted by score)
    for (auto college : colleges) {
        vector<Student*> prefs = students;
        sort(prefs.begin(), prefs.end(), 
            [](Student* a, Student* b) { return a->getScore() > b->getScore(); });
        for (auto student : prefs) {
            college->addPreference(student);
        }
    }

    return true;
}

// Run matching algorithm and save results
void runTest(const string& dataFile) {
    // Use provided test data instead of generating new data
    // generateTestData(dataFile);

    // Read test data
    vector<Student*> students;
    vector<College*> colleges;
    map<string, Student*> studentMap;
    map<string, College*> collegeMap;

    if (!readTestData(dataFile, students, colleges, studentMap, collegeMap)) {
        cerr << "Failed to read test data\n";
        return;
    }

    // Run algorithm for both modes
    auto runAlgorithm = [&](bool studentOptimal, const string& mode) {
        StableMatchingAlgorithm algo(students, colleges, studentOptimal);
        auto start = chrono::high_resolution_clock::now();
        auto matching = algo.match();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

        // Calculate metrics
        int matched = 0;
        int totalQuota = 0;
        double totalRank = 0;
        double totalScore = 0;
        int matchedStudents = 0;

        for (const auto& [sId, cId] : matching) {
            if (!cId.empty()) {
                matched++;
                Student* student = studentMap[sId];
                auto prefs = student->getPreferences();
                for (size_t i = 0; i < prefs.size(); ++i) {
                    if (prefs[i]->getId() == cId) {
                        totalRank += (i + 1);
                        break;
                    }
                }
                totalScore += student->getScore();
                matchedStudents++;
            }
        }

        for (auto college : colleges) totalQuota += college->getQuota();

        // Output to console
        cout << "\n=== " << mode << " Matching Results ===\n";
        cout << "Matching Rate: " << (matched * 100.0 / students.size()) << "%\n";
        cout << "Quota Usage: " << (matched * 100.0 / totalQuota) << "%\n";
        cout << "Average Satisfaction Rank: " << 
            (matchedStudents ? totalRank / matchedStudents : 0) << "\n";
        cout << "Average Admission Score: " << 
            (matchedStudents ? totalScore / matchedStudents : 0) << "\n";
        cout << "Execution Time: " << duration << " microseconds\n";

        // Output sample matches
        cout << "Sample Matches (first 5):\n";
        int count = 0;
        for (const auto& [sId, cId] : matching) {
            if (count++ >= 5) break;
            cout << sId << " -> " << (cId.empty() ? "Unmatched" : cId) << "\n";
        }

        // Save results to txt file
        ofstream outFile(mode + "_result.txt");
        outFile << mode << " Matching Results\n";
        for (const auto& [sId, cId] : matching) {
            outFile << sId << " -> " << (cId.empty() ? "Unmatched" : cId) << "\n";
        }
        outFile.close();
    };

    // Run both algorithms
    runAlgorithm(true, "Student-Oriented");
    runAlgorithm(false, "College-Oriented");

    // Clean up memory
    for (auto student : students) delete student;
    for (auto college : colleges) delete college;
}

int main() {
    cout << "Running Stable Matching Algorithm Test...\n";
    runTest("test_data.txt");
    return 0;
}