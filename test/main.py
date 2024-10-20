from misc import load_image, estimate_camera_intrinsics
from estimate import estimate_gaze
from render import draw_gaze


if __name__ == "__main__":
    img_path = "test_image.jpg"
    roi = [214.467, 96.9926, 110.877, 117.08]

    image = load_image(img_path)
    
    fx, fy, cx, cy = estimate_camera_intrinsics(image)
    focal_length = (fx, fy)
    optical_center = (cx, cy)

    gaze = estimate_gaze(image, roi, focal_length, optical_center)

    print(gaze)

    if gaze is None:
        print("Unable to detect gaze")
    else:
        draw_gaze(image, gaze, focal_length, optical_center)