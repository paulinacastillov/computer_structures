#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //We will assume that all our values will be integers
    public:
        SparseRow() : row(-1), col(-1), value(0) {} //Default constructor; row=-1;col=-1;value=0
        SparseRow(int _row, int _col, int _value) : row(_row), col(_col), value(_value) {} //Default constructor; row=-1;col=-1;value=0
        
        int getRow() const { return row; }
        int getCol() const { return col; }
        int getValue() const { return value; }        
        
        //Print Row#, Column#, value
        void display() const {
            cout << row << "\t" << col << "\t" << value << endl;
        }; 
};

class SparseMatrix {
    protected:
        int noRows; //Number of rows of the original matrix
        int noCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int noNonSparseValues;
        vector<SparseRow*> myMatrix; //Array
    public:
        SparseMatrix() : noRows(0), noCols(0), commonValue(0) {}
        SparseMatrix(int n, int m, int cv) : noRows(n), noCols(m), commonValue(cv) {}
        //SparseMatrix (const SparseMatrix& SM); //Copy Constructor
        
        void addValue(int row, int col, int value) {
            if (value != commonValue) {
                // Add only non-sparse values
                myMatrix.push_back(new SparseRow(row, col, value));
            }
        }

        void inputMatrix() {
            for (int i=0; i < noRows; i++) {
                cout << "Row " << i << endl; 
                for (int j=0; j < noCols; j++) {
                    int val;
                    cin >> val;
                    addValue(i, j, val);
                }
            }
        }
    

        // Define a order function such return a bolean
        static bool compareSparseRow(const SparseRow* a, const SparseRow* b) {
            if (a->getRow() == b->getRow()) {
                return a->getCol() < b->getCol();  // We check if b position is greater than a position if they share row 
            }
            return a->getRow() < b->getRow();  // If they don't share row we just check which row is smaller
        }


        void sortMatrix() {
            std::sort(myMatrix.begin(), myMatrix.end(), compareSparseRow);  // Use the comparison function to roder the rows in a lexicograph strucure
        }

        
        SparseMatrix* Transpose() {
            // Create a new SparseMatrix object for the transposed matrix
            SparseMatrix* transposedMatrix = new SparseMatrix(noCols, noRows, commonValue);

            // Iterate through the existing myMatrix and swap the row and column for each SparseRow
            for (int i = 0; i < myMatrix.size(); i++) {
                int row = myMatrix[i]->getRow();
                int col = myMatrix[i]->getCol();
                int value = myMatrix[i]->getValue();

                // Add the transposed SparseRow to the transposed matrix
                transposedMatrix->myMatrix.push_back(new SparseRow(col, row, value));
            }

            // Return the pointer to the new transposed matrix
            return transposedMatrix;
        }

        // Multiply two SparseMatrix objects
        SparseMatrix* Multiply(SparseMatrix& M) {
            // Check if matrix multiplication is valid (noCols of this must match noRows of M)
            if (noCols != M.noRows) {
                cout << "Matrix multiplication not possible: Dimensions do not match." << endl;
                return nullptr;
            }

            // Create a 2D array to accumulate the results
            vector<vector<int> > resultMatrix(noRows, vector<int>(M.noCols, commonValue));

            // Multiply only non-zero elements of the sparse matrices
            for (int i = 0; i < myMatrix.size(); i++) {
                SparseRow* a = myMatrix[i];

                // For each element in matrix A, find matching elements in matrix B
                for (int j = 0; j < M.myMatrix.size(); j++) {
                    SparseRow* b = M.myMatrix[j];

                    // Multiply A(i, k) * B(k, j) where a->col == b->row
                    if (a->getCol() == b->getRow()) {
                        // Accumulate the result in the appropriate cell (a->row, b->col)
                        resultMatrix[a->getRow()][b->getCol()] += a->getValue() * b->getValue();
                    }
                }
            } // It could be an error if the common value is not zero

            // Create a new SparseMatrix to store the non-zero results
            SparseMatrix* result = new SparseMatrix(noRows, M.noCols, commonValue);

            // Add non-zero results to the resulting sparse matrix
            for (int i = 0; i < noRows; i++) {
                for (int j = 0; j < M.noCols; j++) {
                    if (resultMatrix[i][j] != commonValue) {
                        result->addValue(i, j, resultMatrix[i][j]);
                    }
                }
            }

            return result;
        }

        SparseMatrix* Add(SparseMatrix& M) {
            // Ensure matrices have the same dimensions
            if (noRows != M.noRows || noCols != M.noCols) {
                cout << "Matrix addition not possible: Dimensions do not match." << endl;
                return nullptr;
            }

            // Sort both matrices to ensure they are in row-column order
            this->sortMatrix();
            M.sortMatrix();

            // Create a new SparseMatrix to store the result
            SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue);

            int i = 0, j = 0;
            
            // Traverse both matrices' non-sparse elements
            while (i < myMatrix.size() && j < M.myMatrix.size()) {
                SparseRow* a = myMatrix[i];
                SparseRow* b = M.myMatrix[j];
                
                // Compare rows and columns
                if (a->getRow() == b->getRow() && a->getCol() == b->getCol()) {
                    // Same position, sum the values
                    int summedValue = a->getValue() + b->getValue();
                    if (summedValue != commonValue) {
                        result->myMatrix.push_back(new SparseRow(a->getRow(), a->getCol(), summedValue));
                    }
                    i++;
                    j++;
                } else if (a->getRow() < b->getRow() || (a->getRow() == b->getRow() && a->getCol() < b->getCol())) {
                    // Element in this matrix but not in M, add this element
                    result->myMatrix.push_back(new SparseRow(a->getRow(), a->getCol(), a->getValue()));
                    i++;
                } else {
                    // Element in M but not in this matrix, add the element from M
                    result->myMatrix.push_back(new SparseRow(b->getRow(), b->getCol(), b->getValue()));
                    j++;
                }
            }

            // If there are remaining elements in this matrix, add them to the result
            while (i < myMatrix.size()) {
                SparseRow* a = myMatrix[i];
                result->myMatrix.push_back(new SparseRow(a->getRow(), a->getCol(), a->getValue()));
                i++;
            }

            // If there are remaining elements in matrix M, add them to the result
            while (j < M.myMatrix.size()) {
                SparseRow* b = M.myMatrix[j];
                result->myMatrix.push_back(new SparseRow(b->getRow(), b->getCol(), b->getValue()));
                j++;
            }

            return result;
        }// IT COULD BE NICE TO ORDER THE MATRIX AT THE END


        //Display the matrix in its sparse table format
        void displaySparse() const {
            cout << "Sparse matrix display" << endl;
            cout << "Row #\tCol #\tValue" << endl;
            for (int i=0; i < myMatrix.size(); i++) {
                myMatrix[i]->display();
            }
        };
        
        //Display matrix in its original format
        void displayMatrix() const {
            // Step 1: Initialize a 2D array filled with commonValue
            vector<vector<int> > fullMatrix(noRows, vector<int>(noCols, commonValue)); // the space is super important > > because the compiler  needs it for some reazon (?)

            // Step 2: Place non-sparse values in the correct positions
            for (int i = 0; i < myMatrix.size(); ++i) {
                SparseRow* sparseRow = myMatrix[i];
                fullMatrix[sparseRow->getRow()][sparseRow->getCol()] = sparseRow->getValue();
            }

            // Step 3: Print the matrix in its full form
            for (int i = 0; i < noRows; i++) {
                for (int j = 0; j < noCols; j++) {
                    cout << fullMatrix[i][j] << "\t";
                }
                cout << endl;
            }
        }



        void inputMatrixManually1() {
    addValue(0, 0, 100);
    addValue(0, 3, 900);
    addValue(0, 5, 500);
    addValue(1, 4, 200);
    addValue(1, 7, 300);
    addValue(2, 1, 400);
    addValue(2, 6, 800);
    addValue(3, 2, 200);
    addValue(4, 0, 1600);
    addValue(4, 4, 700);
}

void inputMatrixManually2() {
    addValue(0, 1, 25);
    addValue(0, 6, 49);
    addValue(1, 3, 36);
    addValue(2, 7, 101);
    addValue(3, 0, 67);
    addValue(3, 5, 72);
    addValue(4, 3, 44);
    addValue(4, 5, 93);
    addValue(5, 1, 55);
    addValue(6, 3, 76);
    addValue(7, 3, 85);
    addValue(7, 5, 23);
}
};


int main () {

    int n, m, cv, noNSV;

    SparseMatrix* temp;
    cin >> n >> m >> cv ; // I took out NoNSV because it wasnt useful
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv);
    firstOne->inputMatrix();

    cin >> n >> m >> cv ;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv);
    secondOne->inputMatrix();

    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

    cout << "First one in sparse matrix format" << endl;
    (*firstOne).displaySparse();

    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    cout << "Second one in sparse matrix format" << endl;
    (*secondOne).displaySparse();

    SparseMatrix* anotherCopy = new SparseMatrix(*secondOne);

    cout << "Transpose of the first one in matrix" << endl;
    SparseMatrix* tr = firstOne->Transpose();
    tr->displayMatrix();


    cout << "Matrix Addition Result" << endl;
    temp = ((firstOne)->Add(*secondOne));
    temp->displayMatrix();

    cout << "Matrix Multiplication Result" << endl;
    temp = ((firstOne)->Multiply(*secondOne));
    temp->displayMatrix();

}

// int main() {
//     int r, c, cv;
//     cout << "Insert the number of rows" << endl;
//     cin >> r;
//     cout << "Insert the number of columns" << endl;
//     cin >> c;
//     cout << "Insert the common value" << endl;
//     cin >> cv;
    
//     SparseMatrix matrix1 = SparseMatrix(r, c, cv);
//     matrix1.inputMatrix();
//     matrix1.displaySparse();

//     cout << "Insert the number of rows" << endl;
//     cin >> r;
//     cout << "Insert the number of columns" << endl;
//     cin >> c;
//     cout << "Insert the common value" << endl;
//     cin >> cv;
 
//     SparseMatrix matrix2 = SparseMatrix(r, c, cv);
//     matrix2.inputMatrix();
//     matrix2.displaySparse();

//     SparseMatrix* result1 = matrix1.Add(matrix2);

//     if (result1) {
//         cout << "Sum:" << endl;
//         result1->displayMatrix();
//     }    

//     SparseMatrix* result2 = matrix1.Multiply(matrix2);

//     if (result2) {
//         cout << "Multiply:" << endl;
//         result2->displayMatrix();
//     }
 
//     //SparseMatrix* tr = matrix1.Transpose();
//     //tr->displayMatrix();
    
//     //tr->sortMatrix();
//     //tr->displayMatrix();

//     return 0;
// }

// int main() {
//     SparseMatrix matrix1 = SparseMatrix(5, 8, 0);
//     matrix1.inputMatrixManually1();
//     matrix1.displaySparse();

//     SparseMatrix* tr = matrix1.Transpose();
//     cout << "Transpose sparse matrix" << endl;
//     tr->displaySparse();

//     SparseMatrix matrix2 = SparseMatrix(8, 8, 0);
//     matrix2.inputMatrixManually2();
//     matrix2.displaySparse();

//     SparseMatrix* result1 = matrix1.Add(matrix2);

//     if (result1) {
//         cout << "Sum:" << endl;
//         result1->displayMatrix();
//     }    

//     SparseMatrix* result2 = matrix1.Multiply(matrix2);

//     if (result2) {
//         cout << "Multiply:" << endl;
//         result2->displayMatrix();
//     }

//     return 0;
// }
