# Gaze-tracking

## Aktualny stan
Na ten moment zaimplementowany został komplet modułów do wykrywania poszczególnych elementów - obszaru twarzy, znaczników twarzy (landmarks), pozy twarzy oraz śledzenia kierunku wzroku (gaze tracking).
Całość jest wciąż skonstruowana głównie pod kątemn ręcznego testowania, dlatego też kolejnymi krokami będzie zbudowanie API do łatwej obsługi odpowiedniego modułu, a także konteneryzacja.
Oczywiście wszelkie uwagi mile widziane :)


## Krótki opis struktury projektu:
- */sr/c* - kod źródłowy
- */model/* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib/* - tu trafiają zbudowane pliki .lib biblioteki OpenFace
- */include/* - tu trafiają pliki .h biblioteki OpenFace
- */dev/* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
- */config.h* - derfinicje ścieżek i stałych parametrów wykonania programu
