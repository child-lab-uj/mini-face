# Mini-face

## Instalacja
> Wymagania: `cmake`, `vcpkg`, `python 3.12`.<br>
> Zalecane użycie środowiska wirtualnego.
1. Zainstaluj moduł `scikit-build`:
   ```
   pip install scikit-build
   ```
2. Pobierz i zainstaluj repozytorium:
   ```
   git clone https://github.com/child-lab-uj/gaze-tracking
   cd gaze-tracking
   pip install .
   ```


## Krótki opis struktury projektu:
- */src/* - kod źródłowy
- */model/* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib/* - kod źródłowy bibliotek kompilowanych razem z projektem (głównie OpenFace)
- */dev/* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
- */src/config.h* - derfinicje ścieżek i stałych parametrów wykonania programu


## Użycie w Pythonie
Poniżej zamieszczam minimalistyczny przykład użycia modułu z poziomu kodu języka Python:
```
import mini_face

import cv2
import mini_face._python_api

if __name__ == "__main__":
    image = cv2.imread("test_image.jpg")
    roi = (214.467, 96.9926, 110.877, 117.08)

    extractor = mini_face._python_api.GazeExtractor()
    extractor.estimate_camera_calibration(image)

    print(extractor.detect_gaze(image, 0, roi))
```
W tym przykładzie, powstały z kompilacji plik biblioteki znajduje się w tym samym katalogu co plik z powyższym kodem.
