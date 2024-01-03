#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int burstTime;
    int arrivalTime;
    int priority;
    int waitingTime;
};

// Function to read input from a file
vector<Process> readInputFromFile(const string& fileName) {
    ifstream inputFile(fileName.c_str());

    vector<Process> processes;
    int burst, arrival, priority;

    while (inputFile >> burst >> arrival >> priority) {
        processes.push_back({burst, arrival, priority, 0});
    }

    inputFile.close();
    return processes;
}

// Function to write output to a file
void writeOutputToFile(const string& fileName, const vector<Process>& processes) {
    ofstream outputFile(fileName.c_str());

    for (const auto& process : int ) {
        outputFile << process.waitingTime << "\n";
    }

    outputFile.close();
}

// Function to implement First Come, First Served Scheduling
void firstComeFirstServed(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;

    for (auto& process : processes) {
        process.waitingTime = currentTime - process.arrivalTime;
        currentTime += process.burstTime;
    }
}

// Function to implement Shortest-Job-First Scheduling
void shortestJobFirst(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.burstTime < b.burstTime;
    });

    int currentTime = 0;

    for (auto& process : processes) {
        process.waitingTime = currentTime - process.arrivalTime;
        currentTime += process.burstTime;
    }
}

// Function to implement Priority Scheduling
void priorityScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority;
    });

    int currentTime = 0;

    for (auto& process : processes) {
        process.waitingTime = currentTime - process.arrivalTime;
        currentTime += process.burstTime;
    }
}

// Function to implement Round-Robin Scheduling
void roundRobinScheduling(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    vector<int> remainingTime(n, 0);

    int currentTime = 0;
    int index = 0;

    while (true) {
        bool done = true;

        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0) {
                done = false;

                if (remainingTime[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingTime[i] -= timeQuantum;
                } else {
                    currentTime += remainingTime[i];
                    processes[i].waitingTime = currentTime - processes[i].arrivalTime - processes[i].burstTime;
                    remainingTime[i] = 0;
                }
            }
        }

        if (done)
            break;
    }
}

// Function to display the results
void showResults(const vector<Process>& processes) {
    cout << "Process\tWaiting Time\n";
    for (size_t i = 0; i < processes.size(); i++) {
        cout << i + 1 << "\t" << processes[i].waitingTime << "\n";
    }

    double averageWaitingTime = 0.0;
    for (const auto& process : processes) {
        averageWaitingTime += process.waitingTime;
    }
    averageWaitingTime /= processes.size();

    cout << "Average Waiting Time: " << averageWaitingTime << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " -f input.txt -o output.txt\n";
        return 1;
    }

    string inputFile, outputFile;
    for (int i = 1; i < argc; i += 2) {
        if (string(argv[i]) == "-f") {
            inputFile = argv[i + 1];
        } else if (string(argv[i]) == "-o") {
            outputFile = argv[i + 1];
        }
    }

    vector<Process> processes = readInputFromFile(inputFile);

    while (true) {
        cout << "1) Scheduling Method (None)\n";
        cout << "2) Preemptive Mode (Off)\n";
        cout << "3) Show Result\n";
        cout << "4) End Program\n";
        cout << "Option > ";

        int option;
        cin >> option;

        switch (option) {
            case 1: {
                cout << "Choose Scheduling Method:\n";
                cout << "1) None\n";
                cout << "2) First Come, First Served Scheduling\n";
                cout << "3) Shortest-Job-First Scheduling\n";
                cout << "4) Priority Scheduling\n";
                cout << "5) Round-Robin Scheduling\n";

                int method;
                cin >> method;

                switch (method) {
                    case 1:
                        // None
                        break;
                    case 2:
                        firstComeFirstServed(processes);
                        break;
                    case 3:
                        shortestJobFirst(processes);
                        break;
                    case 4:
                        priorityScheduling(processes);
                        break;
                    case 5: {
                        int timeQuantum;
                        cout << "Enter Time Quantum: ";
                        cin >> timeQuantum;
                        roundRobinScheduling(processes, timeQuantum);
                        break;
                    }
                    default:
                        cout << "Invalid Scheduling Method\n";
                        break;
                }

                break;
            }
            case 2:
                cout << "Choose Preemptive Mode:\n";
                cout << "1) Clock-Driven Mode\n";
                cout << "2) Non-preemptive Mode\n";
                // Add your implementation for Preemptive Mode here
                break;
            case 3:
                showResults(processes);
                break;
            case 4:
                writeOutputToFile(outputFile, processes);
                cout << "Results written to " << outputFile << "\n";
                return 0;
            default:
                cout << "Invalid Option\n";
                break;
        }
    }

    return 0;
}

