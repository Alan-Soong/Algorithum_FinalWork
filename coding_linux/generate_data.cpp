#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;

// Generate test data and save to txt file
void generateTestData(const string& filename) {
    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> scoreDist(400, 750);
    uniform_int_distribution<> quotaDist(5, 15);

    // Data scale
    const int numStudents = 50;
    const int numColleges = 5;

    // Generate students and colleges
    vector<pair<string, double>> students; // id, score
    vector<string> collegeIds;
    vector<pair<string, int>> colleges; // id, quota
    vector<vector<string>> studentPrefs(numStudents);

    // Generate students
    for (int i = 0; i < numStudents; ++i) {
        students.emplace_back("S" + to_string(i), scoreDist(gen));
    }

    // Generate colleges
    for (int i = 0; i < numColleges; ++i) {
        collegeIds.push_back("C" + to_string(i));
        colleges.emplace_back("C" + to_string(i), quotaDist(gen));
    }

    // Generate student preferences
    for (int i = 0; i < numStudents; ++i) {
        studentPrefs[i] = collegeIds;
        shuffle(studentPrefs[i].begin(), studentPrefs[i].end(), gen);
    }

    // Write to txt file
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot open " << filename << " for writing\n";
        return;
    }

    outFile << "STUDENTS\n";
    for (const auto& [id, score] : students) {
        outFile << id << " Student" << id.substr(1) << " " << score;
        for (const auto& cId : studentPrefs[stoi(id.substr(1))]) {
            outFile << " " << cId;
        }
        outFile << "\n";
    }

    outFile << "COLLEGES\n";
    for (const auto& [id, quota] : colleges) {
        outFile << id << " College" << id.substr(1) << " " << quota << "\n";
    }

    outFile.close();
    cout << "Test data generated and saved to " << filename << "\n";
}

int main() {
    cout << "Generating test data...\n";
    generateTestData("test_data.txt");
    return 0;
}