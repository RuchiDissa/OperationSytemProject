#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    int burstTime;
    int arrivalTime;
    int waitingTime;
};

bool compareJobs(const Job& a, const Job& b) {
    return a.arrivalTime < b.arrivalTime;
}

vector<Job> readJobsFromFile(const char* fileName) {
    vector<Job> jobs;
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }

    int burst, arrival;

    while (inputFile >> burst >> arrival) {
        jobs.push_back(Job());
        jobs.back().burstTime = burst;
        jobs.back().arrivalTime = arrival;
        jobs.back().waitingTime = 0;
    }

    inputFile.close();
    return jobs;
}

void simulateFCFS(vector<Job>& jobs) {
    sort(jobs.begin(), jobs.end(), compareJobs);

    int currentTime = 0;

    for (size_t i = 0; i < jobs.size(); ++i) {
        jobs[i].waitingTime = currentTime - jobs[i].arrivalTime;
        currentTime += jobs[i].burstTime;
    }
}

void displayResults(const vector<Job>& jobs) {
    cout << "Job\tWaiting Time\n";
    for (size_t i = 0; i < jobs.size(); ++i) {
        cout << i + 1 << "\t" << jobs[i].waitingTime << "\n";
    }

    double averageWaitingTime = 0.0;
    for (size_t i = 0; i < jobs.size(); ++i) {
        averageWaitingTime += jobs[i].waitingTime;
    }
    averageWaitingTime /= jobs.size();

    cout << "Average Waiting Time: " << averageWaitingTime << "\n";
}

void writeResultsToFile(const char* fileName, const vector<Job>& jobs) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }

    for (size_t i = 0; i < jobs.size(); ++i) {
        outputFile << jobs[i].waitingTime << "\n";
    }

    outputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " -f input.txt -o output.txt" << endl;
        return 1;
    }

    const char* inputFile = 0;
    const char* outputFile = 0;

    for (int i = 1; i < argc; i += 2) {
        if (argv[i][0] == '-' && argv[i][1] == 'f') {
            inputFile = argv[i + 1];
        } else if (argv[i][0] == '-' && argv[i][1] == 'o') {
            outputFile = argv[i + 1];
        }
    }

    if (!inputFile || !outputFile) {
        cerr << "Invalid command line arguments." << endl;
        return 1;
    }

    vector<Job> jobs = readJobsFromFile(inputFile);

    int choice;
    bool preemptiveMode = false;

    do {
        cout << "\nCPU Scheduler Simulator\n";
        cout << "1) Scheduling Method (None)\n";
        cout << "2) Preemptive Mode (Off)\n";
        cout << "3) Show Result\n";
        cout << "4) End Program\n";
        cout << "Option > ";
        cin >> choice;

        switch (choice) {
            case 1:
                // Implement scheduling methods here
                cout << "Scheduling Method: In this mode, the program obtains the scheduling method from the user.\n";
                cout << "1) None: None of the scheduling methods chosen\n";
                cout << "2) First Come, First Served Scheduling\n";
                cout << "3) Shortest-Job-First Scheduling\n";
                cout << "4) Priority Scheduling\n";
                cout << "5) Round-Robin Scheduling (You should also obtain the time quantum value)\n";
                break;
            case 2:
                preemptiveMode = !preemptiveMode;
                cout << "Preemptive Mode: " << (preemptiveMode ? "On" : "Off") << "\n";
                break;
            case 3:
                simulateFCFS(jobs);
                displayResults(jobs);
                writeResultsToFile(outputFile, jobs);
                break;
            case 4:
                cout << "Ending Program.\n";
                break;
            default:
                cout << "Invalid choice. Please choose again.\n";
        }

    } while (choice != 4);

    return 0;
}
