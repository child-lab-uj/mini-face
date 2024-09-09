import sys
import pathlib
sys.path.append(str(pathlib.Path(__file__).parent.parent.absolute() / 'build'))

import GazeTracking
import cv2

if __name__ == "__main__":
    image = cv2.imread("dev/data/test_image.jpg")
    roi = (214.467, 96.9926, 110.877, 117.08)

    extractor = GazeTracking.GazeExtractor()
    extractor.estimate_camera_calibration(image)

    print(extractor.detect_gaze(image, 0, roi))
