## Installation & Setup

This project contains two different implementations that must be set up separately:
1.  **Full C (Grayscale)**: A complete C program for grayscale images.The current code is made to process and compress jpg files and not png as jpg is already a more compressed form than png .
2.  **Hybrid C/Python (Color)**: A Python script that calls a C library for color images.

---
## Project Structure
```
├── codes/
│   ├── c_libs/
│   │   └── jacobi.h           # Header file with function declarations
│   ├── c_main/
│   │   ├── jacobi.c           # Implementation of SVD and image processing
│   │   └── svd.c              # Main program
│   └── python/
│       └── plot.py            # Plotting script for error analysis
├── figs/                      # Output directory for plots
└── tables/                    # LaTeX tables with error metrics
```
---
### 1.  Model 1: Full C (Grayscale) Setup

This implementation uses the `stb_image.h`,`stb_image_write.h` `svd.h` and  libraries, which should be placed in the `c_libs` folder. The main program is in `c_main`.
The code may require changing the file path in the code so just go through that during setup.
#### Step 1: Download `stb` Libraries

If you haven't already, download `stb_image.h` and `stb_image_write.h` from the [stb GitHub repository](https://github.com/nothings/stb) and place both files inside the `c_libs` directory.
Coping and move this libs into the c_main folder.

#### Step 2: Compile the Program

Navigate to the `c_main` directory and run the `gcc` compiler. We will link against the math library (`-lm`) and tell the compiler where to find the headers (`-I../c_libs`).

```bash
# Navigate to the C main directory
cd c_main

# Compile the program

gcc -o ImageCompression main.c -I../c_libs -lm -O3 

```

#### Step 3: Run the Program

After compiling, you can run the executable (e.g., `compression`) from the `c_main` directory. The arguments will depend on how you wrote your `main.c`.

A common pattern would be:

```bash

./compression <input_image.jpg>  <k_value> ....
```

---

### 2. Model 2: Hybrid C/Python (Color) Setup

This model uses a Python frontend to manage images and a C backend for the fast SVD computation.

#### Step 1: Compile the C Shared Library (.so)

First, you must compile the C code (`SVD_lib.c`) into a shared library that Python can load.

```bash
# Navigate to the C backend directory
cd hybrid_c_python/c_backend

# Compile SVD_lib.c into SVD_lib.so
gcc -shared -o SVD_lib.so SVD_lib.c -lm -fPIC -O3

# Go back to the project root
cd ../../
```

The `SVD_lib.so` file should now be inside the `hybrid_c_python/c_backend` folder.

#### Step 2: Install Python Dependencies

The Python script requires `numpy` (for matrix handling) and `Pillow` (for image I/O).

```bash
# Install required Python packages
pip install numpy pillow
```

#### Step 3: Run the Python Frontend

Finally, navigate to the `python_frontend` directory and run the main Python script. The script is responsible for loading the `.so` file and processing the image.
You may need to change the file path in some cases just go through that in the code.
Also; manualy change the value of k u need the array in the code and can also change the input.jpg file name for your easiness.

```bash
# Navigate to the Python frontend
cd hybrid_c_python/python_frontend

# Run the main script
python3 main.py
```
