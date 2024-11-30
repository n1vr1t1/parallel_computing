/**
 * @brief Checks if a matrix is symmetric by comparing the elements at (i, j) and (j, i). 
 * 
 * @param M The matrix to check.
 * @param n The size of the matrix (n x n).
 * @return true if the matrix is symmetric, false otherwise.
 */
bool checkSym(float** M, int n) {
    bool sym = true;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (M[j][i] != M[i][j]) {
                sym = false;
            }
        }
    }
    return sym;
}

/**
 * @brief Copies the transpose of a matrix into another matrix.
 * The function follows row-major order for reading and column-major order for writing
 * @param M The original matrix.
 * @param T The transposed matrix.
 * @param n The size of the matrix (n x n).
 */
void matTranspose(float** M, float** T, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[j][i] = M[i][j];
        }
    }
}
/**
 * @brief Copies the transpose of a matrix into another matrix.
 * The function follows column-major order for reading and row-major order for writing
 * @param M The original matrix.
 * @param T The transposed matrix.
 * @param n The size of the matrix (n x n).
 */
void matTranspose2(float** M, float** T, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[i][j] = M[j][i];
        }
    }
}