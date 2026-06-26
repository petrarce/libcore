import cv2

def transform_image(img_path, angle):
	img = cv2.imread(img_path, cv2.IMREAD_UNCHANGED)
	rot_mat = cv2.getRotationMatrix2D((img.shape[1]/2, img.shape[0]/2), angle, 1.0)
	rotated_img = cv2.warpAffine(img, rot_mat, (img.shape[1], img.shape[0]))
	cv2.imshow("Rotated image", rotated_img)
	cv2.waitKey()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	transform_image("./test_img.png", 45)
