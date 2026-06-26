#!/usr/bin/env python3

import cv2
import argparse

parser = argparse.ArgumentParser(prog = "Crop image using opencv")
parser.add_argument("inp", type=str, help="Path to the image to be cropped.", metavar="PATH_TO_IMAGE")
# parser.add_argument("out", type=str, help="Path to the output image to be cropped.", metavar="PATH_TO_IMAGE")
parser.add_argument("--px", help="Position x", required=True, type=int)
parser.add_argument("--py", help="Position y", required=True, type=int)
parser.add_argument("--width", help="Width", required=True, type=int)
parser.add_argument("--height", help="Height", required=True, type=int)

args = parser.parse_args()
print(args)
img = cv2.imread(args.inp, cv2.IMREAD_UNCHANGED)
# cropped_img = image[args.px:args.width, args.py:args.height]
print(f"{args.px}:{args.width}, {args.py}:{args.height}")
cropped_image = img[args.px:(args.px + args.width), args.py:(args.py + args.height)] # Slicing to crop the image
# cropped_image = img[100:100, 200:200] # Slicing to crop the image
# cropped_image = img[80:280, 150:330] # Slicing to crop the image

print(img.shape)
print(cropped_image.shape)

cv2.imshow("Original", img)
cv2.imshow("cropped", cropped_image)
cv2.waitKey()
cv2.destroyAllWindows()
