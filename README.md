# mini-face

## Installation with `pip`
> Requirements: `python >= 3.12`, `numpy >= 1.21`.
1. Install the package with a following command:
```shell
   python3 -m pip install mini-face
```
2. Download model data files (`model` and `AU_predictors`) from this repository or official
   [OpenFace release](https://github.com/TadasBaltrusaitis/OpenFace/releases).
   You will also need files from [OpenFace models](https://github.com/TadasBaltrusaitis/OpenFace/wiki/Model-download).

## Example
To estimate gaze on a single image, simply run:

```python
from pathlib import Path

import cv2
import numpy as np
from mini_face import gaze, PredictionMode


if __name__ == "__main__":
    extractor = gaze.Extractor(
        mode=PredictionMode.IMAGE,         # Specify prediction mode
        focal_length=(500.0, 500.0),       # Specify camera parameters
        optical_center=(860.0, 540.0),     # required for proper 3D estimation
        models_directory=Path("./model"),  # Pass a directory to search for OpenFace weights in
    )

    image = cv2.imread("test_image.jpg")
    result = extractor.predict(
        image,                             # Pass the image to analyze
        np.array([0, 0, 1080, 1920])       # along with a region to search for face in
    )

    print(f"{result = }")
```
