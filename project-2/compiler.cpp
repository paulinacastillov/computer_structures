#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// CM class to represent Compute Modules
class CM {
private:
    char CMType;    // Type of the CM (A: Addition, S: Subtraction, etc.)
    string id;      // Unique identifier for the CM
    CM* input1;     // Pointer to the first input CM
    CM* input2;     // Pointer to the second input CM (can be NULL)
    CM* output;     // Pointer to the output CM
    vector<vector<int>> computedOutput;  // The result/output of the CM's computation
    bool outputComputed;  // Flag to indicate if the output has been computed
    bool computationFailed; // Flag to indicate if the computation failed
    int rows, cols; // Matrix dimensions

public:
    // Constructor to initialize the CM object
    CM(char type, const string& id) : CMType(type), id(id), input1(nullptr), input2(nullptr), output(nullptr), outputComputed(false), computationFailed(false), rows(0), cols(0) {}

    // Set the first input for the CM
    void setInput1(CM* inputCM) { input1 = inputCM; }

    // Set the second input for the CM
    void setInput2(CM* inputCM) { input2 = inputCM; }

    // Set the output for the CM
    void setOutput(CM* outputCM) { output = outputCM; }

    // Getters for matrix dimensions
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Get the computed output matrix
    const vector<vector<int>>& getComputedOutput() const { return computedOutput; }

    // Set the matrix and its dimensions for input CMs (used for input modules)
    void setMatrix(const vector<vector<int>>& matrix, int r, int c) {
        computedOutput = matrix;
        rows = r;
        cols = c;
        outputComputed = true;
    }

    // Main computation function for the CM
    void compute() {
        //if (outputComputed || computationFailed) return; // Skip if already computed or failed

        // Compute addition for 'A' type CMs
        if (CMType == 'A') {
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                
                if (input1->rows != input2->rows || input1->cols != input2->cols) {
                    cout << "Matrices are not compatible for addition at " << id << "." << endl;
                    computationFailed = true;
                    //return;
                }
                rows = input1->rows;
                cols = input1->cols;
                computedOutput.resize(rows, vector<int>(cols));
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        computedOutput[i][j] = input1->computedOutput[i][j] + input2->computedOutput[i][j];
                    }
                }
            }
        }

        // Compute subtraction for 'S' type CMs
        else if (CMType == 'S') {
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                if (input1->rows != input2->rows || input1->cols != input2->cols) {
                    cout << "Matrices are not compatible for subtraction at " << id << "." << endl;
                    computationFailed = true;
                    //return;
                }
                rows = input1->rows;
                cols = input1->cols;
                computedOutput.resize(rows, vector<int>(cols));
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        computedOutput[i][j] = input1->computedOutput[i][j] - input2->computedOutput[i][j];
                    }
                }
            }
        }

        // Compute multiplication for 'M' type CMs
        else if (CMType == 'M') {
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                if (input1->cols != input2->rows) {
                    cout << "Matrices are not compatible for multiplication at " << id << "." << endl;
                    computationFailed = true;
                    
                }
                rows = input1->rows;
                cols = input2->cols;
                computedOutput.resize(rows, vector<int>(cols, 0));
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        for (int k = 0; k < input1->cols; ++k) {
                            computedOutput[i][j] += input1->computedOutput[i][k] * input2->computedOutput[k][j];
                        }
                    }
                }
            }
        }

        // Compute transpose for 'T' type CMs
        else if (CMType == 'T') {
            if (input1) {
                input1->compute();
                rows = input1->cols;
                cols = input1->rows;
                computedOutput.resize(rows, vector<int>(cols));
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        computedOutput[i][j] = input1->computedOutput[j][i];
                    }
                }
            }
        }

        // Compute for 'O' type CMs, which just take output from input1
        else if (CMType == 'O') {
            if (input1) {
                input1->compute();
                rows = input1->getRows();
                cols = input1->getCols();
                computedOutput = input1->getComputedOutput(); // Copy the result from input1
            }
        }

        outputComputed = true; // Mark as computed
    }

    // Display output matrix if it has been computed
    void displayOutput() const {
        if (outputComputed) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    cout << computedOutput[i][j] << " ";
                }
                cout << endl;
            }
        } else {
            cout << "Output has not been computed yet." << endl;
        }
    }

    // Return the ID of the CM
    string getId() const { return id; }

    // Return pointers to input and output modules
    CM* getInput1() const { return input1; }
    CM* getInput2() const { return input2; }
    CM* getOutput() const { return output; }
};

// Find CM by its ID from the list of CMs
CM* findCMById(CM* allCMs[], int numCMs, const string& id) {
    for (int i = 0; i < numCMs; ++i) {
        if (allCMs[i]->getId() == id) {
            return allCMs[i];
        }
    }
    return nullptr;
}

int main() {
    int numCMs;
    cin >> numCMs;

    CM* allCMs[numCMs];

    // Create CM modules based on user input
    for (int i = 0; i < numCMs; ++i) {
        string id;
        cin >> id;
        char type = id[0];
        allCMs[i] = new CM(type, id);
    }

    // Input for connections
    int numConnections;
    cin >> numConnections;
    cin.ignore();

    // Setup the connections
    for (int i = 0; i < numConnections; ++i) {
        string input;
        vector<string> words;

        // Read input line and split it into words
        getline(cin, input);
        stringstream ss(input);
        string word;

        while (ss >> word) {
            words.push_back(word);
        }

        // Handling the addition of inputs to CMs
        if (words[0] == "A") {
            string id1 = words[1];
            string id2 = words[2];

            CM* cm1 = findCMById(allCMs, numCMs, id1);
            CM* cm2 = findCMById(allCMs, numCMs, id2);

            if (!cm2->getInput1()) {
                cm2->setInput1(cm1);
            } else {
                cm2->setInput2(cm1);
            }
            cm1->setOutput(cm2);  // Set output connection

        } else if (words[0] == "I") {
            string id = words[1];
            int rows = stoi(words[2]);
            int cols = stoi(words[3]);

            vector<vector<int>> matrix(rows, vector<int>(cols));

            // Read matrix input row by row
            for (int i = 0; i < rows; ++i) {
                string rowInput;
                getline(cin, rowInput);
                stringstream ss(rowInput);
                for (int j = 0; j < cols; ++j) {
                    ss >> matrix[i][j];
                }
            }

            // Set the matrix for input module
            CM* inputModule = findCMById(allCMs, numCMs, id);
            if (inputModule) {
                inputModule->setMatrix(matrix, rows, cols);
            }

        } else if (words[0] == "O") {
            string id = words[1];

            CM* outputModule = findCMById(allCMs, numCMs, id);
            cout << "Computation Starts" << endl;
            outputModule->compute();
            outputModule->displayOutput();
        }
    }

    // Show all established connections and computed results
    cout << "***** Showing the connections that were established" << endl;
    for (int i = 0; i < numCMs; ++i) {
        CM* cm = allCMs[i];
        cm->compute();
        cout << allCMs[i]->getId();
        if (allCMs[i]->getInput1()) cout << ", Input 1 = " << allCMs[i]->getInput1()->getId();
        if (allCMs[i]->getInput2()) cout << ", Input 2 = " << allCMs[i]->getInput2()->getId();
        if (allCMs[i]->getOutput()) cout << ", Output = " << allCMs[i]->getOutput()->getId();
        cout << endl;
    }

    return 0;
}
