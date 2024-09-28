# Mini-face

## Installation with pip
> Requirements: `python 3.12`.
Install the package with a following command:
```
   python3 -m pip install mini-face
```

## Installation from source
> Requirements: `cmake`, `vcpkg`, `python 3.12`.<br>
> Recommended to use a virtual environment.
1. Install the `scikit-build` module:
```
pip install scikit-build
```
2. Download and install the repository:
```
git clone https://github.com/child-lab-uj/gaze-tracking
cd gaze-tracking
pip install .
```

## Short description of the project structure:
- */src/* - source code - OpenFace source, wrappers and Python bindings
- */model/* - Model data from the official OpenFace release
- */AU_predictors/* - Model data from the official OpenFace release

## Python usage
Below is a minimalist example of using the module from Python code:

```
   import cv2
   import mini_face.api

   if __name__ == "__main__":
      image = cv2.imread("test_image.jpg")
      roi = (214.467, 96.9926, 110.877, 117.08)

      extractor = mini_face.api.GazeExtractor()

      extractor.estimate_camera_calibration(image)

      print(extractor.detect_gaze(image, 0, roi))
```