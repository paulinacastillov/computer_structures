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
    

        // Define a comparison function
        static bool compareSparseRow(const SparseRow* a, const SparseRow* b) {
            if (a->getRow() == b->getRow()) {
                return a->getCol() < b->getCol();  // Sort by column if rows are equal
            }
            return a->getRow() < b->getRow();  // Sort by row first
        }


        void sortMatrix() {
            std::sort(myMatrix.begin(), myMatrix.end(), compareSparseRow);  // Use the comparison function
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
            }

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
        }


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
            vector<vector<int> > fullMatrix(noRows, vector<int>(noCols, commonValue));

            // Step 2: Place non-sparse values in the correct positions
            for (int i = 0; i < myMatrix.size(); ++i) {
                SparseRow* sparseRow = myMatrix[i];
                fullMatrix[sparseRow->getRow()][sparseRow->getCol()] = sparseRow->getValue();
            }

            // Step 3: Print the matrix in its full form
            for (int i = 0; i < noRows; i++) {
                for (int j = 0; j < noCols; j++) {
                    cout << fullMatrix[i][j] << " ";
                }
                cout << endl;
            }
        }



        //other methods that are necessary such as get and set
};

int main() {
    int r, c;
    cout << "Insert the number of rows" << endl;
    cin >> r;
    cout << "Insert the number of columns" << endl;
    cin >> c;
    
    SparseMatrix matrix1 = SparseMatrix(r, c, 0);
    matrix1.inputMatrix();
    matrix1.displaySparse();

    cout << "Insert the number of rows" << endl;
    cin >> r;
    cout << "Insert the number of columns" << endl;
    cin >> c;
 
    SparseMatrix matrix2 = SparseMatrix(r, c, 0);
    matrix2.inputMatrix();
    matrix2.displaySparse();

    SparseMatrix* result1 = matrix1.Add(matrix2);

    if (result1) {
        cout << "Sum:" << endl;
        result1->displayMatrix();
    }    

    SparseMatrix* result2 = matrix1.Multiply(matrix2);

    if (result2) {
        cout << "Multiply:" << endl;
        result2->displayMatrix();
    }
 
    //SparseMatrix* tr = matrix1.Transpose();
    //tr->displayMatrix();
    
    //tr->sortMatrix();
    //tr->displayMatrix();

    return 0;
}
