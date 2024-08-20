# Gaze-tracking

## Aktualny stan
Na ten moment zaimplementowany został komplet modułów do wykrywania poszczególnych elementów - obszaru twarzy, znaczników twarzy (landmarks), pozy twarzy oraz śledzenia kierunku wzroku (gaze tracking).
Całość jest wciąż skonstruowana głównie pod kątemn ręcznego testowania, dlatego też kolejnymi krokami będą zbudowanie API do łatwej obsługi odpowiedniego modułu, a także konteneryzacja.
Oczywiście wszelkie uwagi mile widziane :)
<br>

## Koncepcja API
Na ten moment API jest niekompletne, choć zdatne do podstawowego użytku. Poczyniłem założenie, iż najefektywniejszym sposobem komunikacji z modułem będzie
uruchomienie go jako osobnego procesu i odczyt wyników z pliku. Jest to o tyle lepsze pod względem wydajnościowym, że nie wprowadza dodatkowego narzutu wynikającego z sieciowego przesyłu danych (wariant
z kontenerem Dockera) i pozwala w prosty sposób osiągnąć wielowątkowość - można delegować osobny proces do nagrań video z każdej z kamer, tym samym przyśpieszając przetwarzanie.

Oczywiście kwestia ta jest do uzgodnienia. Jeżeli wymogi projektu wymuszą inny sposób komunikacji to jestem gotowy na odpowiednie dostosowanie modułu.
<br>

## Krótki opis struktury projektu:
- */src/* - kod źródłowy
- */test/* - funkcje testowe
- */model/* - odpowiednik /model z biblioteki OpenFace, przechowuje używane przez biblioteki modele
- */lib/* - tu trafiają zbudowane pliki .lib biblioteki OpenFace
- */include/* - tu trafiają pliki .h biblioteki OpenFace
- */dev/* - zasoby (filmy, zdjęcia) i wyniki przetwarzania
- */config.h* - derfinicje ścieżek i stałych parametrów wykonania programu
