import numpy as np
from mini_face import GazeFeatureExtractor, PredictionMode
from misc import estimate_camera_intrinsics


model_dir = "../model"

def estimate_gaze(image, roi, focal_length, optical_center):
    extractor = GazeFeatureExtractor(
        mode=PredictionMode.IMAGE,
        multiple_views=True,
        focal_length=focal_length,
        optical_center=optical_center,
        models_directory=model_dir
    )

    result = extractor.predict(image, np.array(roi))

    return result