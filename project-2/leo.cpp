#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class Matrix
{
public:
    vector<vector<int> > data;

    // Constructor
    Matrix(int rows, int cols) : data(rows, vector<int>(cols, 0)) {}

    void setMatrix(int nrows, int ncols) {
        data.resize(nrows, vector<int>(ncols));  // Resize matrix to nrows x ncols
        string line;

        for (int i = 0; i < nrows; i++) {
            getline(cin, line);  // Read a full line of input
            istringstream rowStream(line);  // Create a stream from the line

            for (int j = 0; j < ncols; j++) {
                rowStream >> data[i][j];  // Extract each value separated by spaces
            }
        }
    }
    
    // Método para agregar dos matrices
    Matrix Add(const Matrix &other) const
    {
        int rows = data.size();
        int cols = data[0].size();
        Matrix result(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    // Método para multiplicar dos matrices
    Matrix Multiply(const Matrix &other) const
    {
        int rows = data.size();
        int cols = other.data[0].size();
        int common_dim = data[0].size();
        Matrix result(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = 0;
                for (int k = 0; k < common_dim; k++)
                {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    // Método para transponer la matriz
    Matrix Transpose() const
    {
        int rows = data.size();
        int cols = data[0].size();
        Matrix result(cols, rows);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }

    // Método para multiplicar la matriz por un escalar
    Matrix MultiplyScalar(double scalar) const
    {
        int rows = data.size();
        int cols = data[0].size();
        Matrix result(rows, cols);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                result.data[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Método para imprimir la matriz (solo para verificar resultados)
    void Print() const
    {
        for (const auto &row : data)
        {
            for (const auto &elem : row)
            {
                cout << elem << " ";
            }
            cout << endl;
        }
    }
};

class CM
{
private:
    char CMType;           // Tipo de operación (A: Suma, S: Resta, etc.)
    string id;             // Identificador único
    CM *input1;            // Primer input
    CM *input2;            // Segundo input (puede ser NULL)
    CM *output;            // Output CM (puede ser NULL)
    Matrix computedOutput; // Resultado de la operación
    bool outputComputed;   // Indica si el output ha sido computado
public:
    // Constructor
    CM(char type, const string &id) : CMType(type), id(id), input1(nullptr), input2(nullptr), output(nullptr), computedOutput(1, 1), outputComputed(false) {}

    // Métodos
    void setInput1(CM *inputCM) { input1 = inputCM; }
    void setInput2(CM *inputCM) { input2 = inputCM; }
    void setOutput(CM *outputCM) { output = outputCM; }
    void setComputedOutput(Matrix M){ computedOutput = M; }

    string getId() const { return id; }
    CM *getInput1() const { return input1; }
    Matrix getOutput() const { return computedOutput; }
    

    // void compute() {
    //     // If already computed, do nothing
    //     cout<<"compute antes del if "<< id << endl;
    //     if (outputComputed) {
    //         cout<<"compute already computed "<< id << endl;
    //         return;
    //     }

    //     // If input1 exists, compute its output first
        
    //     if (input1 != nullptr && !outputComputed) {
    //         cout<<"compute input "<< id << endl;
    //         //input1->compute();
    //         computedOutput = input1->computedOutput;
    //     }

    //     // If input2 exists, compute its output first
        
    //     // if (input2 != nullptr && !this->outputComputed) {
    //     //     cout<<"compute input2 "<< id << endl;
    //     //     input2->compute();
    //     // }

    //     // Perform the operation based on CMType
    //     switch (CMType) {
    //         case 'I':  // Identity operation, just pass through input1's output
    //             cout<<"compute caso I "<< id << endl;
    //             computedOutput = input1->computedOutput;
    //             break;

    //         case 'A':  // Addition
    //             cout<<"compute caso A "<< id << endl;
    //             computedOutput = input1->computedOutput.Add(input2->computedOutput);
    //             break;

    //         case 'M':  // Multiplication
    //             cout<<"compute caso M "<< id << endl;
    //             computedOutput = input1->computedOutput.Multiply(input2->computedOutput);
    //             break;

    //         case 'T':  // Transpose
    //             cout<<"compute caso T "<< id << endl;
    //             computedOutput = input1->computedOutput.Transpose();
    //             break;

    //         case 'S':  // Subtraction (input1 - input2)
    //             cout<<"compute caso S "<< id << endl;
    //             computedOutput = input1->computedOutput.Add(input2->computedOutput.MultiplyScalar(-1));
    //             break;

    //         default:
    //             break;
    //     }

    //     // Mark this CM's output as computed
    //     outputComputed = true;
    // }


void compute() {
    // If already computed, do nothing
    cout << "Entering compute for CM: " << id << endl;
    if (outputComputed) {
        cout << "Already computed output for CM: " << id << endl;
        return;
    }

    // Check and compute input1's output first
    if (input1 != nullptr && !input1->outputComputed) {
        cout << "Computing input1 for CM: " << id << " (input1 id: " << input1->getId() << ")" << endl;
        input1->compute();
    }

    // Check and compute input2's output if it exists
    if (input2 != nullptr && !input2->outputComputed) {
        cout << "Computing input2 for CM: " << id << " (input2 id: " << input2->getId() << ")" << endl;
        input2->compute();
    }

    // Perform the operation based on CMType
    switch (CMType) {
        case 'I':  // Identity operation, just pass through input1's output
            cout << "Performing identity operation for CM: " << id << endl;
            computedOutput = input1->computedOutput;
            break;

        case 'A':  // Addition
            cout << "Performing addition for CM: " << id << endl;
            computedOutput = input1->computedOutput.Add(input2->computedOutput);
            break;

        case 'M':  // Multiplication
            cout << "Performing multiplication for CM: " << id << endl;
            computedOutput = input1->computedOutput.Multiply(input2->computedOutput);
            break;

        case 'T':  // Transpose
            cout << "Performing transpose for CM: " << id << endl;
            computedOutput = input1->computedOutput.Transpose();
            break;

        case 'S':  // Subtraction
            cout << "Performing subtraction for CM: " << id << endl;
            computedOutput = input1->computedOutput.Add(input2->computedOutput.MultiplyScalar(-1));
            break;

        default:
            cout << "Unknown operation type for CM: " << id << endl;
            break;
    }

    // Mark this CM's output as computed
    outputComputed = true;
    cout << "Finished compute for CM: " << id << endl;
}



    // Check if both inputs are ready
    bool isReady() const { return input1 != nullptr && (CMType == 'T' || input2 != nullptr); }
    bool hasOutput() const { return outputComputed; }

    void reset()
    {
        outputComputed = false;
    }

    void displayInfo() const
    {cout<<"prueba output antes del if"<<endl;
        if (CMType == 'O')
        {
            cout<<"prueba output denttro caso O"<<endl;
            computedOutput.Print();
        }
        cout<<"prueba output fuera if"<<endl;
    }
};

CM *findCMById(const vector<CM *> &cmList, const string &id)
{
    for (CM *cm : cmList)
    {
        if (cm->getId() == id)
        {
            return cm; // Devolver el puntero al objeto si el id coincide
        }
    }
    return nullptr; // Retorna nullptr si no se encuentra ningún CM con el id dado
}

int main() {

    int numCMs, numInstructions;
    string line;

    // Read the number of CMs from standard input
    cin >> numCMs;

    vector<CM*> allCMs;

    // Read the CM definitions
    for (int i = 0; i < numCMs; i++) {
        string cm;
        cin >> cm;
        char type = cm[0];
        allCMs.push_back(new CM(type, cm));
    }

    // Read the number of instructions from standard input
    cin >> numInstructions;
    cin.ignore(); // Clear the buffer for getline

    // Process all instructions (A, I, O)
    for (int i = 0; i < numInstructions; i++) {

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

        if(words[0] == "A"){
            cout << "Case A: " << endl;
            CM *foundNode = findCMById(allCMs, words[2]);
            CM *inputNode = findCMById(allCMs, words[1]);
            
            if(foundNode->getInput1() == nullptr){
                foundNode->setInput1(inputNode);
            }
            else{
                foundNode->setInput2(inputNode);
            }
            
        } 
        else if(words[0] == "I"){
            cout << "Case I: " << endl;
            CM *foundNode = findCMById(allCMs, words[1]);
            int noRow = stoi(words[2]);
            int noCol = stoi(words[3]);

            Matrix inputMatrix(noRow, noCol);
            inputMatrix.setMatrix(noRow, noCol);
            
            foundNode->setComputedOutput(inputMatrix);
            
        }
        else if(words[0] == "O"){
            cout << "Case O: " << endl;
            CM *foundNode = findCMById(allCMs, words[1]);
                // Ensure all dependent computations are done before printing
            foundNode->compute();
            foundNode->displayInfo();  // Print the result matrix
            
            
        };

    };

 
}



