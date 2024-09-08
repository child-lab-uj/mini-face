# Gaze-tracking

## Instalacja
1. Pobierz menadżer pakietów vcpkg:
   ```
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.bat  # Windows
   ./bootstrap-vcpkg.sh   # Linux/macOS
   ./vcpkg integrate install
   ```
2. Zainstaluj wymagane biblioteki - OpenCV, OpenBLAS, dlib i pybind11 (OpenBLAS instalowany jest razem z OpenCV i dlib):
   ```
   ./vcpkg install opencv dlib pybind11
   ```
3. Zbuduj projekt przy użyciu CMake, podając ścieżkę do menadżera pakietów vcpkg (CMAKE_TOOLCHAIN_FILE). 
   Opcjonalnie, w przypadku problemów z wykryciem bibliotek, można podać ścieżkę do miejsca ich instalacji (CMAKE_PREFIX_PATH). 
   **Uwaga**: istotna jest zgodność wersji interpretera Pythona używanego podczas budowania modułu z tą wykorzystywaną podczas
   wykonywania programu. Aby zapewnić użycie danej wersji podczas budowania, można ustawić ścieżkę do interpretera (Python_EXECUTABLE).
   Przykładowo, budowanie dla VS (Windows):
   ```
   cmake -G "Visual Studio 17 2022" ^
      -DCMAKE_PREFIX_PATH="C:/Software/vcpkg/installed/x64-windows" ^
      -DCMAKE_TOOLCHAIN_FILE="C:/Software/vcpkg/scripts/buildsystems/vcpkg.cmake" ^
      -DPython_EXECUTABLE=C:/Users/PREDATOR/AppData/Local/Programs/Python/Python312/python.exe
      ..
   ```
4. Skompiluj rozwiązanie (Uwaga: zalecany tryb Release) wybranym przez siebie narzędziem.
5. Dodaj pliki .dll używanych bibliotek do katalogu z plikiem wykonywalnym.
   Opcjonalnie można dodać ścieżkę do katalogu z plikami wykonywalnymi menadżera vcpkg do zmiennej systemowej PATH. Przykładowo, dla systemu Windows:
   ```
   setx PATH "%PATH%;C:/Software/vcpkg/installed/x64-windows/bin"
   setx PATH "%PATH%;C:/Software/vcpkg/installed/x64-windows/debug/bin"
   ```
6. Pobierz resztę plików modeli (do podkatalogu **/model/patch_experts**) z https://github.com/TadasBaltrusaitis/OpenFace/wiki/Model-download


## Krótki opis struktury projektu:
- */src/* - kod źródłowy
- */model/* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib/* - kod źródłowy bibliotek kompilowanych razem z projektem (głównie OpenFace)
- */dev/* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
- */src/config.h* - derfinicje ścieżek i stałych parametrów wykonania programu


## Użycie w Pythonie
Poniżej zamieszczam minimalistyczny przykład użycia modułu z poziomu kodu języka Python:
```
import sys

sys.path.append('.')

import GazeTracking
import cv2

if __name__ == "__main__":
    image = cv2.imread("test_image.jpg")
    roi = ...          # Calculate face bounding box (Some face detection model call)
    timestamp = ...    # Timestamp of the frame, for single image we can simply use 0

    extractor = GazeTracking.GazeExtractor()
    extractor.estimate_camera_calibration(image)

    print(extractor.detect_gaze(image, timestamp, roi))
```
W tym przykładzie, powstały z kompilacji plik biblioteki znajduje się w tym samym katalogu co plik z powyższym kodem.
