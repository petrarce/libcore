import cv2
import numpy as np


def match_pattern(input: str, pattern: str):
	img = cv2.imread(input)
	img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	pattern_gray = cv2.imread(pattern, cv2.IMREAD_GRAYSCALE)

	if img_gray is None:
		raise FileNotFoundError(f"Source image not found: {input}")
	if pattern_gray is None:
		raise FileNotFoundError(f"Template image not found: {pattern}")

	methods = [
		cv2.TM_CCOEFF,
		cv2.TM_CCOEFF_NORMED,
		cv2.TM_CCORR,
		cv2.TM_CCORR_NORMED,
		cv2.TM_SQDIFF,
		cv2.TM_SQDIFF_NORMED,
	]

	method_names = [
		"TM_CCOEFF",
		"TM_CCOEFF_NORMED",
		"TM_CCORR",
		"TM_CCORR_NORMED",
		"TM_SQDIFF",
		"TM_SQDIFF_NORMED",
	]

	for method, name in zip(methods, method_names):
		result = cv2.matchTemplate(img_gray, pattern_gray, method)
		result_norm = cv2.normalize(result, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)
		result_colored = cv2.applyColorMap(result_norm, cv2.COLORMAP_JET)
		result_resized = cv2.resize(result_colored, (img.shape[1], img.shape[0]))
		overlay = cv2.addWeighted(img, 0.5, result_resized, 0.5, 0)
		cv2.imshow(name, overlay)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

if __name__ == "__main__":
	match_pattern(input="../ChessBoard.png", pattern="../ChessKnightpng.png")
