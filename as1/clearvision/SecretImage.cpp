#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
     height = image.get_height(), width = image.get_width();

    upper_triangular = new int[width * (width + 1) / 2];
    lower_triangular = new int[height * (height - 1) / 2];
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    int upper_index = 0;
    int lower_index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_value = image.get_pixel(i, j);
            if (j < i) {
                lower_triangular[lower_index++] = pixel_value;
            } else {
                upper_triangular[upper_index++] = pixel_value;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    width = w, height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    // TODO: Your code goes here.
    int upper_index = 0;
    int lower_index = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j < i) {
                // Fill the lower triangular part
                image.set_pixel(i, j, lower_triangular[lower_index++]);
            } else {
                // Fill the upper triangular part
                image.set_pixel(i, j, upper_triangular[upper_index++]);
            }
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    int upper_index = 0;
    int lower_index = 0;
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j < i) {
                // Update lower triangular array (below diagonal)
                lower_triangular[lower_index++] = image.get_pixel(i, j);
            } else {
                // Update upper triangular array (including diagonal)
                upper_triangular[upper_index++] = image.get_pixel(i, j);
            }
        }
    }
}


// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // 1. Write width and height on the first line, separated by a single space
    file << width << " " << height << "\n";

    // 2. Write the upper_triangular array to the second line

    int upper_size = width * (width + 1) / 2;
    for (int i = 0; i < upper_size - 1; ++i) {
        file << upper_triangular[i] << " ";
    }
    file << upper_triangular[upper_size - 1] << "\n";

    // 3. Write the lower_triangular array to the third line
    int lower_size = height * (height - 1) / 2;
    for (int i = 0; i < lower_size - 1; ++i) {
        file << lower_triangular[i] << " ";
    }
    file << lower_triangular[lower_size - 1] << "\n";

    file.close();
}


// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    std::ifstream file(filename);
    // 1. Open the file and read width and height from the first line, separated by a space.
    int width, height;
    file >> width >> height;
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int upper_size = width * (width + 1) / 2;
    int lower_size = height * (height- 1) / 2;
    // 3. Allocate memory for both arrays.
    int* upper = new int[upper_size];
    int* lower = new int[lower_size];
    // 4. Read the upper_triangular array from the second line, space-separated.
    for (int i = 0; i < upper_size; ++i) {
        file >> upper[i];
    }
    // 5. Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < lower_size; ++i) {
        file >> lower[i];
    }
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    file.close();
    return SecretImage(width, height, upper, lower);
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
