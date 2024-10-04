#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream> // because of the split
using namespace std;

class Matrix
{
public:
    vector<vector<double> > data;

    // Constructor
    Matrix(int rows, int cols) : data(rows, vector<double>(cols, 0)) {}


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
    CM() : CMType(), id(), computedOutput(1, 1) {}; // Inicializa con una matriz vacía de 1x1
    CM(char type, const string &id) : CMType(type), id(id), computedOutput(1, 1) {};

    // Métodos
    void setInput1(CM *inputCM) { input1 = inputCM; }
    void setInput2(CM *inputCM) { input2 = inputCM; }
    void setOutput(CM *outputCM) { output = outputCM; }
    string getId() const {return id;}
    CM* getInput1() const {return input1;}

    void compute()
    {
        switch (CMType)
        {
        case 'I': // Identidad
            computedOutput = input1->computedOutput;
            break;
        case 'A': // Suma
            computedOutput = input1->computedOutput.Add(input2->computedOutput);
            break;
        case 'T': // Transpuesta
            computedOutput = input1->computedOutput.Transpose();
            break;
        case 'M': // Multiplicación
            computedOutput = input1->computedOutput.Multiply(input2->computedOutput);
            break;
        case 'S': // Resta
            computedOutput = input1->computedOutput.Add(input2->computedOutput.MultiplyScalar(-1));
            break;
        default:
            break;
        }
    }

    // Otras funciones
    bool isReady() const { return input1 != nullptr; }
    void reset() { outputComputed = false; }
    bool hasOutput() const { return outputComputed; }
    void displayInfo() const { cout << "CM " << CMType << " ID: " << id << endl; }
};

CM* findCMById(const vector<CM*>& cmList, const string& id) {
    for (CM* cm : cmList) {
        if (cm->getId() == id) {
            return cm;  // Devolver el puntero al objeto si el id coincide
        }
    }
    return nullptr;  // Retorna nullptr si no se encuentra ningún CM con el id dado
}

// int main()
// {
//     int numCMs;
//     int numConnections;
//     // number of comands
//     cin >> numCMs;
//     // create an array CM objects pointers
//     vector<CM*> allCMs;
//     // = new vector<CM*>();
//     // each array location is a pointer to a CM Object
//     for (int i = 0; i < numCMs; i++)
//     {
//         // create  new variables per iteration

//         // read the input
//         string cm;
//         char type;
    
//         cin >> cm;
//         type = cm[0];

//         allCMs.push_back(new CM(type,cm));
//     };

//     cin >> numConnections;
//     for (int i = 0; i < numConnections; i++)
//     {
//         string row;
//         string action;
//         string input;
//         string node_id;

//         // Leer una línea de texto
//         getline(cin, row); // Ejemplo: "A1 B2 C3"

//         // Crear un stringstream a partir de la fila (row)
//         istringstream ss(row);

//         // Extraer los valores separados por espacio
//         ss >> action >> input >> node_id;

//         // diccionario nodo:[input1]
//         if (action == "A"){
//             // buscar nodo con node ID
//             CM* node = findCMById(allCMs, node_id);
//             CM* inputNode = findCMById(allCMs, input);
//             if (node->getInput1() == nullptr){
//                 node->setInput1(inputNode);
//             }
//             else{
//                 node->setInput2(inputNode);
//             }

//         }
//         else if(action == "I"){
            

//         };


//     }


//     // cout << "CM information" << endl;
//     // for (int i = 0; i < allCMs->size(); i++)
//     // {
//     //     (*allCMs)[i]->displayInfo();
//     // }




//     // Liberar la memoria de los objetos CM
//     // for (int i = 0; i < allCMs.size(); i++)
//     // {
//     //     delete (*allCMs)[i];  // Liberar cada objeto
//     // }

//     // // Liberar la memoria del vector
//     // delete allCMs;

        
//     // read from input the links and make the appropriate connections.
//     // You may need to search the array allCMs to find the CM that is
//     // referred and connect.
//     // If the first letter is an O then execute the compute method on the
//     // object referred.
//     // if the first letter is an R, the perform reset on all the objects.
//     return 0;
// }

CM* findCMById(const vector<CM*>& cmList, const string& id) {
    for (CM* cm : cmList) {
        if (cm->getId() == id) {
            return cm;  // Return the pointer to the object if the id matches
        }
    }
    return nullptr;  // Return nullptr if no CM with the given id is found
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

    // Process instructions for connections (A action)
    for (int i = 0; i < numInstructions; i++) {
        string action, inputId, nodeId;
        getline(cin, line);
        istringstream ss(line);
        ss >> action >> inputId >> nodeId;

        if (action == "A") {
            CM* inputNode = findCMById(allCMs, inputId);
            CM* node = findCMById(allCMs, nodeId);
            if (inputNode != nullptr && node != nullptr) {
                node->setInput1(inputNode);  // Set the input1 connection
            }
        }
    }

    // Process the matrix inputs (I action) and output (O action)
    while (cin >> line) {
        if (line == "I") {
            string inputId;
            int rows, cols;
            cin >> inputId >> rows >> cols;

            CM* inputCM = findCMById(allCMs, inputId);
            if (inputCM != nullptr) {
                Matrix matrix(rows, cols);

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        cin >> matrix.data[i][j];
                    }
                }
                inputCM->computedOutput = matrix;  // Assign the matrix as input
            }
        }
        else if (line == "O") {
            // Output the result of a CM computation
            string outputId;
            cin >> outputId;

            CM* outputCM = findCMById(allCMs, outputId);
            if (outputCM != nullptr) {
                outputCM->compute();  // Perform the computation
                outputCM->computedOutput.Print();  // Print the result matrix
            }
        }
    }

    // Free memory for the CM objects
    for (CM* cm : allCMs) {
        delete cm;
    }

    return 0;
}