# Mini-face

## Installation with pip
> Requirements: `python 3.12`.
1. Install the package with a following command:
```
   python3 -m pip install mini-face
```
2. Download model data files (subdirectories **/model/** and **/AU_predictors/**) from this repository or official
   <a href="https://github.com/TadasBaltrusaitis/OpenFace/releases" title="OpenFace releases">OpenFace release</a>.
   You will also need files from <a href="https://github.com/TadasBaltrusaitis/OpenFace/wiki/Model-download" title="OpenFace models">here</a>.

## Python usage
Below is a minimalist example of using the module from Python code:

```
   import cv2
   import numpy as np
   from mini_face.gaze import Extractor
   from mini_face.common import PredictionMode

   if __name__ == "__main__":
      image = cv2.imread("test_image.jpg")
      extractor = Extractor(
        mode=PredictionMode.VIDEO,
        focal_length=(500, 500),
        optical_center=(860.0, 540.0),
        models_directory="./model",
      )

       result = extractor.predict(image, np.array([0.0, 0.0, 1080.0, 1920.0]))

       print(f"{result = }")
```
