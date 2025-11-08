# SVD Image Compression Library (Full C + C/Python)
---
## OVERVIEW
* This project is done under the Prof. GVV Sharma for the  course Matrix Theory.
---
* The project is done using full C implementation both for Image handling and computing the Compression for grayscale (single channel images).For Colored Images; a hybrid C + python implementation has been done .Python is used for image handling while C backend code is same as for single channel images as in case of grayscale.
---
## Features

* **High Performance:** Optimized SVD computation using C with the `-O3` flag.
* **External Interface:** Contains the `compress_channel` function for external linking.
* **Image Ready:** Handles 2D data (like an image channel) and returns the compressed result.

---

## Acknowledgments & Dependencies
* The full C implementation (for grayscale images) relies heavily on the excellent single-header C libraries stb_image.h and stb_image_write.h by Sean Barrett (nothings) for image loading and saving. Their work was essential for building the pure C-based solution.
* GILBERT STRANG (MIT)
