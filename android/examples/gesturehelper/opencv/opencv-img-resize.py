#!/usr/bin/env python3
import cv2
import argparse

parser = argparse.ArgumentParser(prog="opencw read/write image tutorial")
parser.add_argument("input", help="path to input image", type=str)

args = parser.parse_args()

image = cv2.imread(args.input, cv2.IMREAD_UNCHANGED)
cv2.imshow("Original", image)

# image.shape - rows and cols of numpy array
image_size = tuple([ x for x in reversed(image.shape)])
print(image_size)
up_size = tuple([ x * 2 for x in image_size])
down_size = tuple([ int(x / 2) for x in image_size])

print(up_size)
print(down_size)
down_img = cv2.resize(image, up_size, interpolation=cv2.INTER_LINEAR)
up_img = cv2.resize(image, down_size, interpolation=cv2.INTER_LINEAR)

cv2.imshow("Down", down_img)
cv2.imshow("Up", up_img)
cv2.waitKey()
cv2.destroyAllWindows()
