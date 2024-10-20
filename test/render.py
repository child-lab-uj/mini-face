import cv2
import numpy as np
from misc import perspective_projection
from mini_face import GazeResult


GAZE_LINE_LENGTH = 50.0


def draw_gaze(image, gaze: GazeResult, focal_length, optical_center):
    eyes = gaze.eyes[0]
    directions = gaze.directions[0]

    pupil_left = np.array(eyes[0])
    pupil_right = np.array(eyes[1])
    gaze_direction_left = np.array(directions[0])
    gaze_direction_right = np.array(directions[1])

    points_left = [pupil_left, pupil_left + gaze_direction_left * GAZE_LINE_LENGTH]
    points_right = [pupil_right, pupil_right + gaze_direction_right * GAZE_LINE_LENGTH]

    proj_left = perspective_projection(points_left, focal_length, optical_center)
    proj_right = perspective_projection(points_right, focal_length, optical_center)

    cv2.line(image, proj_left[0], proj_left[1], (110, 220, 0), 2, cv2.LINE_AA)
    cv2.line(image, proj_right[0], proj_right[1], (110, 220, 0), 2, cv2.LINE_AA)

    cv2.imshow('Gaze Direction', image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()