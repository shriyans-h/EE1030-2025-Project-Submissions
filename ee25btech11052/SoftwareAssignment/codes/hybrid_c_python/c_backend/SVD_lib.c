#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



typedef struct
{
    int row;
    int col;
    double **data;
} matrix;

matrix creatematrix(int row, int col)
{
    matrix m;
    m.row = row;
    m.col = col;
    m.data = (double **)malloc(row * sizeof(double *));
    
    for (int i = 0; i < row; i++)
    {
        m.data[i] = (double *)calloc(col, sizeof(double));
    }
    return m;
}

void freematrix(matrix m)
{
    if (m.data) {
        for (int i = 0; i < m.row; i++)
        {
            free(m.data[i]);
        }
        free(m.data);
    }
}


int clamp(double value)
{
    if (value < 0)
        return 0;
    else if (value > 255)
        return 255;
    else
        return (int)(value + 0.5);
}

matrix Transpose(matrix A)
{
    matrix A_t = creatematrix(A.col, A.row);
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.col; j++)
        {
            A_t.data[j][i] = A.data[i][j];
        }
    }
    return A_t;
}

matrix multiply(matrix A, matrix B)
{
    if (A.col != B.row)
    {
        printf("Error: Cannot multiply - dimension mismatch\n");
        exit(1);
    }

    matrix AB = creatematrix(A.row, B.col);
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < B.col; j++)
        {
            for (int k = 0; k < A.col; k++)
            {
                AB.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return AB;
}

double norm(double *v, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum += v[i] * v[i];
    }
    return sqrt(sum);
}

void normalize(double *v, int n)
{
    double norm_v = norm(v, n);
    if (norm_v > 1e-9)
    {
        for (int i = 0; i < n; i++)
        {
            v[i] /= norm_v;
        }
    }
}

double dot_product(double *v, double *w, int n)
{
    double result = 0.0;
    for (int i = 0; i < n; i++)
    {
        result += v[i] * w[i];
    }
    return result;
}

void matrix_vector_multiplication(matrix A, double *v, double *w)
{
    int m = A.row;
    int n = A.col;
    for (int i = 0; i < m; i++)
    {
        w[i] = 0.0;
        for (int j = 0; j < n; j++)
        {
            w[i] += A.data[i][j] * v[j];
        }
    }
}

matrix At_multiply_A(matrix A)
{
    matrix At = Transpose(A);
    matrix AtA = multiply(At, A);
    freematrix(At);
    return AtA;
}

void deflatematrix(matrix b, double l, double *v)
{
    int n = b.row;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            b.data[i][j] -= l * v[i] * v[j];
        }
    }
}

matrix copyMatrix(matrix A)
{
    matrix B = creatematrix(A.row, A.col);
    for (int i = 0; i < A.row; i++)
    {
        for (int j = 0; j < A.col; j++)
        {
            B.data[i][j] = A.data[i][j];
        }
    }
    return B;
}

double poweriteration(matrix b, double *v)
{
    int n = b.row;
    const int maxit = 2000  ;
    const double tol = 1e-7;
    double l = 0.0;
    double lprev = 0.0;

    for (int i = 0; i < n; i++)
    {
        v[i] = (double)rand() / RAND_MAX;
    }

    normalize(v, n);

    double *w = (double *)malloc(n * sizeof(double));
    
    for (int i = 0; i < maxit; i++)
    {
        matrix_vector_multiplication(b, v, w);
        l = dot_product(v, w, n);

        normalize(w, n);
        for (int j = 0; j < n; j++)
        {
            v[j] = w[j];
        }

        if (fabs(l - lprev) < tol)
        {
            break;
        }

        lprev = l;
    }

    free(w);
    return l;
}

typedef struct
{
    matrix u;
    matrix v;
    double *s;
    int k;
} svdresult;

void freeSVDResult(svdresult svd)
{
    freematrix(svd.u);
    freematrix(svd.v);
    free(svd.s);
}


svdresult SVD_compute(matrix a, int k)
{
    int m = a.row;
    int n = a.col;

    matrix b = At_multiply_A(a);
    matrix b_temp = copyMatrix(b);

    svdresult svd;
    svd.k = k;
    svd.s = (double *)malloc(k * sizeof(double));
    
    svd.v = creatematrix(n, k);
    svd.u = creatematrix(m, k);

    double *v = (double *)malloc(n * sizeof(double));
    double *u_col = (double *)malloc(m * sizeof(double));
    

    printf("Starting Power Iteration loop for k=%d...\n", k);

    for (int i = 0; i < k; i++)
    {
        double l = poweriteration(b_temp, v);
        svd.s[i] = sqrt(l);

        for (int j = 0; j < n; j++)
        {
            svd.v.data[j][i] = v[j];
        }

        deflatematrix(b_temp, l, v);
    }

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            v[j] = svd.v.data[j][i];
        }

        matrix_vector_multiplication(a, v, u_col);

        double sigma = svd.s[i];
        if (sigma > 1e-9)
        {
            for (int j = 0; j < m; j++)
            {
                svd.u.data[j][i] = u_col[j] / sigma;
            }
        }
        else
        {
            for (int j = 0; j < m; j++)
            {
                svd.u.data[j][i] = 0.0;
            }
        }
    }

    freematrix(b);
    freematrix(b_temp);
    free(v);
    free(u_col);

    return svd;
}

__attribute__((visibility("default"))) //taki compiller ko pata chale ki isko python me dikhe direct
void compress_channel(double *flat_matrix, int rows, int cols, int k) {

    matrix A = creatematrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            A.data[i][j] = flat_matrix[i * cols + j];
        }
    }

  
    srand(42); 
    
    svdresult svd = SVD_compute(A, k); 

    matrix A_k = creatematrix(rows, cols); 
    for (int r = 0; r < k; r++) {
        double sigma = svd.s[r]; 
        for (int row_idx = 0; row_idx < rows; row_idx++) {
            for (int col_idx = 0; col_idx < cols; col_idx++) {
                A_k.data[row_idx][col_idx] += sigma * svd.u.data[row_idx][r] * svd.v.data[col_idx][r]; 
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            flat_matrix[i * cols + j] = A_k.data[i][j]; 
        }
    }

    freematrix(A); 
    freematrix(A_k); 
    freeSVDResult(svd); 
}


