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

void displayResults(const vector<Job>& jobs, const char* method, int timeQuantum = -1) {
    cout << "Scheduling Method: " << method;
    if (timeQuantum != -1) {
        cout << " – time_quantum=" << timeQuantum;
    }
    cout << "\nProcess Waiting Times:\n";

    for (size_t i = 0; i < jobs.size(); ++i) {
        cout << "P" << i + 1 << ": " << jobs[i].waitingTime << " ms\n";
    }

    double averageWaitingTime = 0.0;
    for (size_t i = 0; i < jobs.size(); ++i) {
        averageWaitingTime += jobs[i].waitingTime;
    }
    averageWaitingTime /= jobs.size();

    cout << "Average Waiting Time: " << averageWaitingTime << " ms\n";
}

void simulateFCFS(vector<Job>& jobs) {
    sort(jobs.begin(), jobs.end(), compareJobs);

    int currentTime = 0;

    for (size_t i = 0; i < jobs.size(); ++i) {
        jobs[i].waitingTime = currentTime - jobs[i].arrivalTime;
        currentTime += jobs[i].burstTime;
    }

    displayResults(jobs, "First Come First Served");
}

void simulateSJF(vector<Job>& jobs, bool preemptive) {
    sort(jobs.begin(), jobs.end(), compareJobs);

    int currentTime = 0;
    for (size_t i = 0; i < jobs.size(); ++i) {
        jobs[i].waitingTime = currentTime - jobs[i].arrivalTime;
        currentTime += jobs[i].burstTime;
    }

    if (preemptive) {
        displayResults(jobs, "Shortest Job First – Preemptive");
    } else {
        displayResults(jobs, "Shortest Job First – Non-Preemptive");
    }
}

void simulatePriority(vector<Job>& jobs, bool preemptive) {
    sort(jobs.begin(), jobs.end(), compareJobs);

    int currentTime = 0;
    for (size_t i = 0; i < jobs.size(); ++i) {
        jobs[i].waitingTime = currentTime - jobs[i].arrivalTime;
        currentTime += jobs[i].burstTime;
    }

    if (preemptive) {
        displayResults(jobs, "Priority Scheduling – Preemptive");
    } else {
        displayResults(jobs, "Priority Scheduling – Non-Preemptive");
    }
}

void simulateRoundRobin(vector<Job>& jobs, int timeQuantum) {
    int currentTime = 0;
    size_t currentIndex = 0;

    while (!jobs.empty()) {
        Job& currentJob = jobs[currentIndex];
        if (currentJob.burstTime > timeQuantum) {
            currentTime += timeQuantum;
            currentJob.burstTime -= timeQuantum;
            jobs.push_back(currentJob);
        } else {
            currentTime += currentJob.burstTime;
            currentJob.waitingTime = currentTime - currentJob.arrivalTime - currentJob.burstTime;

            displayResults(jobs, "Round Robin Scheduling", timeQuantum);

            jobs.erase(jobs.begin() + currentIndex);
            currentIndex--;
        }

        currentIndex = (currentIndex + 1) % jobs.size();
    }
}

void writeResultsToFile(const char* fileName, const vector<Job>& jobs, double averageWaitingTime) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(1);
    }

    for (size_t i = 0; i < jobs.size(); ++i) {
        outputFile << jobs[i].waitingTime << "\n";
    }

    outputFile << "\n===== Additional Statistics =====\n";
    outputFile << "Average Waiting Time for All Jobs: " << averageWaitingTime << " ms\n";
    outputFile << "Waiting Time for Each Job:\n";
    for (size_t i = 0; i < jobs.size(); ++i) {
        outputFile << "Job " << i + 1 << ": " << jobs[i].waitingTime << " ms\n";
    }

    outputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " -f input.txt -o output.txt" << endl;
        return 1;
    }

    const char* inputFile = nullptr;
    const char* outputFile = nullptr;

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
        cout << "1) Scheduling Method\n";
        cout << "2) Preemptive Mode (Off)\n";
        cout << "3) Show Result\n";
        cout << "4) End Program\n";
        cout << "Option > ";
        cin >> choice;

        switch (choice) {
            case 1:
                int schedulingMethod;
                cout << "Scheduling Method: In this mode, the program obtains the scheduling method from the user.\n";
                cout << "1) None: None of the scheduling methods chosen\n";
                cout << "2) First Come, First Served Scheduling\n";
                cout << "3) Shortest-Job-First Scheduling\n";
                cout << "4) Priority Scheduling\n";
                cout << "5) Round-Robin Scheduling\n";
                cout << "Choose scheduling method (1-5): ";
                cin >> schedulingMethod;

                switch (schedulingMethod) {
                    case 1:
                        // None
                        break;
                    case 2:
                        // FCFS
                        simulateFCFS(jobs);
                        break;
                    case 3:
                        // SJF
                        int sjfChoice;
                        cout << "1) Shortest-Job-First Non-Preemptive\n";
                        cout << "2) Shortest-Job-First Preemptive\n";
                        cout << "Choose SJF scheduling type (1 or 2): ";
                        cin >> sjfChoice;

                        if (sjfChoice == 1) {
                            simulateSJF(jobs, false);
                        } else if (sjfChoice == 2) {
                            simulateSJF(jobs, true);
                        } else {
                            cout << "Invalid SJF scheduling choice.\n";
                        }

                        break;
                    case 4:
                        // Priority Scheduling
                        int priorityChoice;
                        cout << "1) Priority Non-Preemptive\n";
                        cout << "2) Priority Preemptive\n";
                        cout << "Choose Priority scheduling type (1 or 2): ";
                        cin >> priorityChoice;

                        if (priorityChoice == 1) {
                            simulatePriority(jobs, false);
                        } else if (priorityChoice == 2) {
                            simulatePriority(jobs, true);
                        } else {
                            cout << "Invalid Priority scheduling choice.\n";
                        }

                        break;
                    case 5:
                        // Round-Robin Scheduling
                        int timeQuantum;
                        cout << "Enter time quantum for Round-Robin Scheduling: ";
                        cin >> timeQuantum;
                        simulateRoundRobin(jobs, timeQuantum);
                        break;
                    default:
                        cout << "Invalid scheduling method choice.\n";
                        break;
                }

                break;
            case 2:
                preemptiveMode = !preemptiveMode;
                cout << "Preemptive Mode: " << (preemptiveMode ? "On" : "Off") << "\n";
                break;
            case 3:
                // Display results
                break;
            case 4:
                // End Program
                break;
            default:
                cout << "Invalid choice. Please choose again.\n";
        }

    } while (choice != 4);

    return 0;
}

