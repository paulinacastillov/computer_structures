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
    int rows, cols; // Matrix dimensions

public:
    // Constructor
    CM(char type, const string& id) : CMType(type), id(id), input1(nullptr), input2(nullptr), output(nullptr), outputComputed(false), rows(0), cols(0) {}

// Set input1 and input2
    void setInput1(CM* inputCM) { input1 = inputCM; }
    void setInput2(CM* inputCM) { input2 = inputCM; }
    void setOutput(CM* outputCM) { output = outputCM; }

    // Getter for outputComputed
    bool isOutputComputed() const { return outputComputed; }

    // Getter for rows and cols
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Getter for computedOutput
    const vector<vector<int>>& getComputedOutput() const { return computedOutput; }

    // Perform the operation based on the CM type
    void compute() {
        if (outputComputed) return; // If already computed, return

        if (CMType == 'A') {
            // Addition logic
            
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                if (input1->rows != input2->rows || input1->cols != input2->cols) {
                    cout << "Matrices are not compatible for addition at " << id << "." << endl;
                    return;
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
        } else if (CMType == 'S') {
            // Subtraction logic
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                if (input1->rows != input2->rows || input1->cols != input2->cols) {
                    cout << "Matrices are not compatible for subtraction at " << id << "." << endl;
                    return;
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
        } else if (CMType == 'M') {
            // Multiplication logic
            if (input1 && input2) {
                input1->compute();
                input2->compute();
                if (input1->cols != input2->rows) {
                    cout << "Matrices are not compatible for multiplication at " << id << "." << endl;
                    return;
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
        } else if (CMType == 'T') {
            // Transpose logic
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
        }else if (CMType == 'O') {
        // Caso especial para el nodo de tipo 'O', que solo toma el output del nodo conectado
        if (input1) {
            input1->compute();  // Asegúrate de que el nodo conectado haya computado su salida
            rows = input1->getRows();
            cols = input1->getCols();
            computedOutput = input1->getComputedOutput();  // Copiar la salida del nodo conectado
        }
    }

        outputComputed = true;
    }

    // Display matrix information for output modules
    void displayOutput() const {
        if (CMType == 'O') {
            cout << "The output value from this circuit is:" << endl;
            //cout << computedOutput << " Prueba";
        }
        if (outputComputed) {
            // Make sure the matrix is printed row by row with spaces between elements
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    cout << computedOutput[i][j] << " ";
                }
                cout << endl;  // Move to the next line after printing each row
            }
        } else {
            cout << "Output has not been computed yet." << endl;
        }
    }

    void setMatrix(const vector<vector<int>>& matrix, int r, int c) {
        computedOutput = matrix;
        rows = r;
        cols = c;
        outputComputed = true;
    }

    string getId() const { return id; }
    CM* getInput1() const { return input1; }
    CM* getInput2() const { return input2; }
    CM* getOutput() const { return output; }
};

// Find CM by ID
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

    // Input module creation
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

    for (int i = 0; i < numConnections; ++i) {
        string input;
        vector<string> words;

        // Leer la línea de entrada
        getline(cin, input);

        // Usar stringstream para dividir la cadena por espacios
        stringstream ss(input);
        string word;

        // Almacenar cada palabra en el vector
        while (ss >> word) {
            words.push_back(word);
        }

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

            // Leer cada fila de la matriz
            for (int i = 0; i < rows; ++i) {
                string rowInput;
                getline(cin, rowInput);

                stringstream ss(rowInput);
                for (int j = 0; j < cols; ++j) {
                    ss >> matrix[i][j];
                }
            }

            CM* inputModule = findCMById(allCMs, numCMs, id);
            if (inputModule) {
                inputModule->setMatrix(matrix, rows, cols);
            }

        } else if (words[0] == "O") {
            string id = words[1];

            CM* outputModule = findCMById(allCMs, numCMs, id);
            cout<<"Computation Starts"<<endl;
            outputModule->compute();
            outputModule->displayOutput();
        }
    }

    // Recorrer todos los nodos y computar sus resultados antes de imprimir las matrices
    

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
