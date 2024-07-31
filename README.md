# Gaze-tracking

## Na chwilę obecną: demo OpenFace
Na ten moment zaimplementowane jest tylko proste wykrywanie twarzy przy użyciu MTCNN i OpenFace.
OpenFace jest zarazem dosyć skomplikowana w użyciu jak i posiadająca duży potencjał, więc wraz z coraz lepszym zrozumieniem funkcjonalności OpenFace 
będą pojawiały się tu nowe elementy, których finalnym celem jest obliczenie kierunku wzroku postaci znajdujących się na zdjęciu / nagraniu.


## Krótki opis struktury projektu:
- */src* - kod źródłowy
- */model* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib* - tu trafiają zbudowane pliki .lib biblioteki OpenFace
- */include* - tu trafiają pliki .h biblioteki OpenFace
- */dev* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
