import ctypes
import numpy as np
from PIL import Image

svd_library = ctypes.CDLL('/home/shriyasnh/Desktop/SoftwareAssignment/ColourSVDimplementation/SVD_lib.so')

compress_channel_function = svd_library.compress_channel
compress_channel_function.argtypes = [
    np.ctypeslib.ndpointer(dtype=np.float64, flags='C_CONTIGUOUS'),
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int
]
compress_channel_function.restype = None

input_image_path = 'input.jpg'
k_values_list = [1, 10, 50 , 100 , 150 , 200 , 250 , 300 , 350]
original_image = Image.open(input_image_path)
image_array = np.array(original_image).astype(np.float64) #image ko array me kardega and type  double
height, width, num_channels = image_array.shape


for k_value in k_values_list:
    print(f"\nCompressing with k={k_value}")
    
    #RGB ko alag alag array me dalne ke liye aur fir compression lagayenge
    red_channel = image_array[:, :, 0].copy()
    green_channel = image_array[:, :, 1].copy() 
    blue_channel = image_array[:, :, 2].copy()
    
    compress_channel_function(red_channel, height, width, k_value)
    compress_channel_function(green_channel, height, width, k_value)
    compress_channel_function(blue_channel, height, width, k_value)
    

    compressed_image_array = np.zeros_like(image_array) #zeero se bhara hua array same size ka 
    compressed_image_array[:, :, 0] = red_channel
    compressed_image_array[:, :, 1] = green_channel
    compressed_image_array[:, :, 2] = blue_channel
    
    compressed_image_array = np.clip(compressed_image_array, 0, 255)
    compressed_image_array = compressed_image_array.astype(np.uint8) #8 byte  integer me direct convert
    
    output_image_path = f'compressed_k{k_value}.jpg'
    compressed_image = Image.fromarray(compressed_image_array)
    compressed_image.save(output_image_path)
    
    print(f"  Saved to {output_image_path}")

print("\nCompression complete!")