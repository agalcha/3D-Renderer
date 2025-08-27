import numpy as np
from scipy.ndimage import gaussian_filter
import sys

GAUSSIAN_SIGMA = 5

def load_ppm_p3(filename):
    with open(filename, 'r') as f:
        header = f.readline().strip()
        if header != 'P3':
            print("Not a PPM P3 file!")
            exit(2)

        # Skip comments
        while True:
            line = f.readline().strip()
            if line.startswith('#'):
                continue
            else:
                width, height = map(int, line.split())
                break

        max_color_value = int(f.readline().strip())
        if max_color_value != 255:
            print("Max color value should be 255!")
            exit(3)

        # Read pixel data
        pixel_data = []
        for line in f:
            pixel_data.extend(map(int, line.split()))

        # Convert pixel data to numpy array and reshape it
        try:
            image = np.array(pixel_data, dtype=np.uint8).reshape((height, width, 3))
        except Exception as e:
            print(e.args[0])
            exit(5)

    return width, height, image

def apply_gaussian_blur(image, sigma=1):
    blurred_image = np.zeros_like(image)
    # Apply blur to each color channel separately
    for channel in range(3):
        blurred_image[..., channel] = gaussian_filter(image[..., channel], sigma=sigma)
    return blurred_image

def compare_images(img1, img2):
    # Convert to float32 to allow negative differences
    img1 = img1.astype(np.float32)
    img2 = img2.astype(np.float32)
    
    # Compute dist between each pixel, return mean
    diff = np.sqrt(np.sum((img1 - img2) ** 2, axis=2))
    return np.mean(diff)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: python3 {sys.argv[0]} <PPM path 1> <PPM path 2>")
        exit(1)

    img1_path = sys.argv[1]
    img2_path = sys.argv[2]

    width1, height1, img1 = load_ppm_p3(img1_path)
    width2, height2, img2 = load_ppm_p3(img2_path)

    # Ensure both images have the same dimensions
    if width1 != width2 or height1 != height2:
        print("Images have different dimensions and cannot be compared.")
        exit(4)
        
    # Apply Gaussian blur to both images
    img1_blur = apply_gaussian_blur(img1, sigma=GAUSSIAN_SIGMA)
    img2_blur = apply_gaussian_blur(img2, sigma=GAUSSIAN_SIGMA)

    # Compare and output
    difference = compare_images(img1_blur, img2_blur)
    print(difference)
