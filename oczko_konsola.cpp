#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

//using std::cin, std::cout, std::vector, std::endl, std::pair, std::string, std::random_device, std::mt19937;

using namespace std; 

///////////////////////////////////////////////////////////
// Klasa reprezentująca talie kart do gry.
///////////////////////////////////////////////////////////
class Karty {
public:
    // Wektory reprezentujące dostępne wartości kart i kolory.
    vector<string> karta = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "Walet", "Dama", "Krol", "As" };
    vector<string> kolor = { "pik", "kier", "karo", "trefl" };

    // Wektor par reprezentujących karty w talii.
    vector<pair<string, string>> talia;

    // Konstruktor inicjalizujący talie kart.
    Karty() {
        for (int i = 0; i < kolor.size(); ++i) {
            for (int j = 0; j < karta.size(); ++j) {
                talia.push_back(make_pair(karta[j], kolor[i]));
            }
        }
    }
};

///////////////////////////////////////////////////////////
// Klasa reprezentująca grę w oczko.
///////////////////////////////////////////////////////////
class Oczko {
private:
    // Wektory przechowujące talie kart oraz ręce gracza i komputera.
    vector<pair<string, string>> taliaKart;
    vector<pair<string, string>> rekaGracza;
    vector<pair<string, string>> rekaKomputera;

public:
    // Konstruktor inicjalizujący talie kart i tasujący je.
    Oczko() {
        Karty talia;
        taliaKart = talia.talia;
        tasowanie();
    }

    // Tasuje talie kart.
    void tasowanie() {
        random_device rd;    // Inicjalizacja generatora liczb losowych
        mt19937 g(rd()); // Utworzenie obiektu generatora MT
        shuffle(taliaKart.begin(), taliaKart.end(), g); // Tasowanie talii kart
    }

    // Oblicza wartość ręki gracza lub komputera.
    // INPUT:
    //      reka - wektor par reprezentujących karty w ręce
    // OUTPUT:
    //      Wartość ręki (int)
    // REMARKS:
    //      Wartość asów dostosowana do sytuacji.
    int wartoscReki(const vector<pair<string, string>>& reka) {
        int wartosc = 0;
        int liczbaAsow = 0;

        for (int i = 0; i < reka.size(); ++i) {
            if (reka[i].first == "As") {
                wartosc += 11;
                liczbaAsow++;
            }
            else if (reka[i].first == "Walet" || reka[i].first == "Dama" || reka[i].first == "Krol") {
                wartosc += 10;
            }
            else {
                wartosc += stoi(reka[i].first); //funkcja 'stoi' konwertuje wartości string na int
            }
        }
        //dostosowanie wartości Asa w zależności od sytuacji
        while (wartosc > 21 && liczbaAsow > 0) {
            wartosc -= 10;
            liczbaAsow--;
        }

        return wartosc;
    }

    // Rozdaje początkowe karty graczowi i komputerowi.
    // INPUT:
    //      None
    // OUTPUT:
    //      None
    // REMARKS:
    //      Karty są dodawane do ręki gracza i komputera.
    void rozdaniePoczatkowe() {
        dobierzKarte(rekaGracza);
        dobierzKarte(rekaKomputera);
        dobierzKarte(rekaGracza);


        cout << "Twoje karty: ";
        pokazReke(rekaGracza);
        cout << "Wartosc Twoich kart: " << wartoscReki(rekaGracza) << endl;

        cout << "Karta komputera: " << rekaKomputera[0].first << " " << rekaKomputera[0].second << endl;
    }

    // Wyświetla karty w ręce.
    // INPUT:
    //      reka - wektor par reprezentujących karty w ręce
    // OUTPUT:
    //      None
    // REMARKS:
    //      Karty wyświetlane są na standardowym wyjściu.
    void pokazReke(const vector<pair<string, string>>& reka) {
        for (int i = 0; i < reka.size(); i++) {
            cout << reka[i].first << " " << reka[i].second;
            if (i < reka.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }

    // Dobiera kartę do ręki gracza lub komputera.
    // INPUT:
    //      reka - wektor par reprezentujących karty w ręce
    // OUTPUT:
    //      None
    // REMARKS:
    //      Karta jest pobierana z talii.
    void dobierzKarte(vector<pair<string, string>>& reka) {
        reka.push_back(taliaKart.back());
        taliaKart.pop_back();
    }

    // Logika gry w oczko.
    // INPUT:
    //      None
    // OUTPUT:
    //      None
    // REMARKS:
    //      Gracz decyduje, czy dobrac karte, czy spasować.
    void graj() {
        char wybor;
        do {
            cout << "Czy chcesz dobrac karte? (D - dobierz, S - stoj): ";
            cin >> wybor;

            if (wybor == 'D' || wybor == 'd') {
                dobierzKarte(rekaGracza);
                cout << "Twoje karty: ";
                pokazReke(rekaGracza);
                cout << "Wartosc Twoich kart: " << wartoscReki(rekaGracza) << endl;

                if (wartoscReki(rekaGracza) > 21) {
                    cout << "Przekroczyles 21! Przegrales." << endl;
                    return;
                }
            }
            else if (wybor == 'S' || wybor == 's') {
                break;
            }
            else {
                cout << "Niepoprawny wybor. Wybierz ponownie." << endl;
            }
        } while (true);

        cout << "Ruch komputera:" << endl;
        while (wartoscReki(rekaKomputera) < 17) {
            dobierzKarte(rekaKomputera);
            pokazReke(rekaKomputera);
            cout << "Wartosc kart komputera: " << wartoscReki(rekaKomputera) << endl;
        }

        int wartoscGracza = wartoscReki(rekaGracza);
        int wartoscKomputera = wartoscReki(rekaKomputera);

        cout << "Twoje karty: ";
        pokazReke(rekaGracza);
        cout << "Wartosc Twoich kart: " << wartoscGracza << endl;

        cout << "Karty komputera: ";
        pokazReke(rekaKomputera);
        cout << "Wartosc kart komputera: " << wartoscKomputera << endl;

        if (wartoscGracza > wartoscKomputera && wartoscGracza <= 21) {
            cout << "Gratulacje! Wygrales." << endl;
        }
        else if (wartoscKomputera > 21) {
            cout << "Komputer przekroczyl 21! Wygrales." << endl;
        }
        else if (wartoscGracza == wartoscKomputera) {
            cout << "Remis!" << endl;
        }
        else {
            cout << "Przegrales." << endl;
        }
    }
};

///////////////////////////////////////////////////////////
// Funkcja główna programu gry w oczko.
///////////////////////////////////////////////////////////
int main() {
    char grajPonownie;
    do {
        Oczko oczko;
        oczko.rozdaniePoczatkowe();
        oczko.graj();

        cout << "Czy chcesz zagrac ponownie? (T - tak, N - nie): ";
        cin >> grajPonownie;
    } while (grajPonownie == 'T' || grajPonownie == 't');

    return 0;
}