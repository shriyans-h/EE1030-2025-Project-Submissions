#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "svd.h"

matrix read_jpg_to_matrix(const char *filename)
{
    int width, height, channels;

    unsigned char *data = stbi_load(filename, &width, &height, &channels, 1);
    matrix img = creatematrix(height, width);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img.data[i][j] = (double)data[i * width + j];
        }
    }
    stbi_image_free(data);

    return img;
}

void write_matrix_to_jpg(const char *filename, matrix img)
{
    int width = img.col;
    int height = img.row;
    int channels = 1; // Grayscale hai

    //double matrix back to unsigned char for writing
    size_t buffer_size = (size_t)width * height * channels;
    unsigned char *buffer = (unsigned char *)malloc(buffer_size);

    // Converting matrix data to unsigned char buffer and clamping values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            buffer[i * width + j] = (unsigned char)clamp(img.data[i][j]);
        }
    }

    int quality = 90; 

    if (stbi_write_jpg(filename, width, height, channels, buffer, quality) == 0)
    {
        fprintf(stderr, "Error: Could not write image to %s\n", filename);
    }

    free(buffer);
}

int main(int argc, char *argv[])
{

    const char *filename = argv[1];

    matrix A = read_jpg_to_matrix(filename);

    int r_max = (A.row < A.col) ? A.row : A.col;
    int k_max_input = 0;
    srand(42);

    for (int i = 2; i < argc; i++) // sabse bada k
    {
        int k = atoi(argv[i]);
        if (k > k_max_input)
        {
            k_max_input = k;
        }
    }

    if (k_max_input > r_max)
    {
        printf("Warning: Largest k=%d is greater than max rank r=%d. Clamping to r.\n", k_max_input, r_max);
        k_max_input = r_max;
    }

    printf("\n--- Computing SVD for largest k=%d ---\n", k_max_input);
    svdresult svd_max = SVD_compute(A, k_max_input);

    for (int i = 2; i < argc; i++)
    {
        int k = atoi(argv[i]);
        if (k <= 0 || k > r_max)
        {
            printf("\nSkipping invalid k=%d. Must be > 0 and <= %d\n", k, r_max);
            continue;
        }

        printf("\n--- Processing for k=%d ---\n", k);

        // ab ham jo sabse bade k ke liye dundha uska reuse karenge.
        matrix A_k = creatematrix(A.row, A.col);

        for (int r_comp = 0; r_comp < k; r_comp++) // Loop only to k
        {
            double sigma = svd_max.s[r_comp];
            for (int row_idx = 0; row_idx < A.row; row_idx++)
            {
                for (int col_idx = 0; col_idx < A.col; col_idx++)
                {
                    A_k.data[row_idx][col_idx] += sigma * svd_max.u.data[row_idx][r_comp] * svd_max.v.data[col_idx][r_comp];
                }
            }
        }

        char out_filename[100];
        sprintf(out_filename, "reconstructed_k%d.jpg", k);
        printf("Writing %s...\n", out_filename);

        write_matrix_to_jpg(out_filename, A_k);

        freematrix(A_k);
        printf("Done for k=%d\n", k);
    }

    freeSVDResult(svd_max);
    freematrix(A);
    return 0;
}