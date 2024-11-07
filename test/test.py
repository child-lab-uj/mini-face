from pathlib import Path

import cv2
import numpy as np
from mini_face import PredictionMode, action_unit, gaze

TEST_DIRECTORY = Path(__file__).parent
PROJECT_ROOT = TEST_DIRECTORY.parent
MODEL_DIRECTORY = PROJECT_ROOT / "model"

if __name__ == "__main__":
    image = cv2.imread(str(TEST_DIRECTORY / "image.png"))
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    gaze_extractor = gaze.Extractor(
        mode=PredictionMode.IMAGE,
        focal_length=(500, 500),
        optical_center=(256.0, 256.0),
        models_directory=MODEL_DIRECTORY,
    )

    au_extractor = action_unit.Extractor(
        landmark_mode=PredictionMode.IMAGE,
        au_mode=PredictionMode.IMAGE,
        models_directory=MODEL_DIRECTORY,
    )

    gazes = gaze_extractor.predict(image, np.array([0.0, 0.0, 512.0, 512.0]))
    print(f"{gazes = }")

    aus = au_extractor.predict(image, np.array([0.0, 0.0, 512.0, 512.0]))
    print(f"{aus = }")
