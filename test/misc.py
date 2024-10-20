import cv2


def load_image(image_path: str):
    return cv2.imread(image_path)

def estimate_camera_intrinsics(image):
    height, width, channels = image.shape

    fx = 500 * (width / 640)
    fy = 500 * (height / 480)

    fx, fy = (fx + fy) / 2, fx

    cx = width / 2
    cy = height / 2

    return fx, fy, cx, cy

# From 3D do 2D
def perspective_projection(points, focal_length, optical_center):
    result = []

    for point in points:
        x, y, z = point

        x_2D = int(focal_length[0] * x / z + optical_center[0])
        y_2D = int(focal_length[1] * y / z + optical_center[1])

        result.append([x_2D, y_2D])

    return result