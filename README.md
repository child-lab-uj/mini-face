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
2. Zainstaluj wymagane biblioteki - OpenCV, OpenBLAS i dlib (OpenBLAS instalowany jest razem z OpenCV i dlib):
   ```
   ./vcpkg install opencv dlib
   ```
3. Zbuduj projekt przy użyciu CMake, podając ścieżkę do menadżera pakietów vcpkg (CMAKE_TOOLCHAIN_FILE). 
   Opcjonalnie, w przypadku problemów z wykryciem bibliotek, można podać ścieżkę do miejsca ich instalacji (CMAKE_PREFIX_PATH). Przykładowo, budowanie dla VS (Windows):
   ```
   cmake -G "Visual Studio 17 2022" ^
      -DCMAKE_PREFIX_PATH="C:/Software/vcpkg/installed/x64-windows" ^
      -DCMAKE_TOOLCHAIN_FILE="C:/Software/vcpkg/scripts/buildsystems/vcpkg.cmake" ^
      ..
   ```
4. Dodaj pliki .dll używanych bibliotek do katalogu z plikiem wykonywalnym.
   Opcjonalnie można dodać ścieżkę do katalogu z plikami wykonywalnymi menadżera vcpkg do zmiennej systemowej PATH. Przykładowo, dla systemu Windows:
   ```
   setx PATH "%PATH%;C:/Software/vcpkg/installed/x64-windows/bin"
   setx PATH "%PATH%;C:/Software/vcpkg/installed/x64-windows/debug/bin"
   ```
5. Pobierz zawartość katalogu **/model** z ... (jeszcze nie gotowe)
6. Pobierz resztę plików modeli (do podkatalogu **/model/patch_experts**) z https://github.com/TadasBaltrusaitis/OpenFace/wiki/Model-download


## Krótki opis struktury projektu:
- */src/* - kod źródłowy
- */test/* - funkcje testowe
- */model/* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib/* - tu trafiają zbudowane pliki .lib biblioteki OpenFace
- */include/* - tu trafiają pliki .h biblioteki OpenFace
- */dev/* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
- */config.h* - derfinicje ścieżek i stałych parametrów wykonania programu
