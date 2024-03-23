// Affichage du compe à Rebours
// et de l'état de la course Open Led Race
// On récupère les données envoyées en série par OLR
// pour piloter une matrice de 16x16 LEDs

#include <FastLED.h>        //https://github.com/FastLED/FastLED
#include <LEDMatrix.h>      //https://github.com/Jorgen-VikingGod/LEDMatrix


#define DATA_PIN            6         // Sortie sur laquelle le Din de la matrice est connecté
#define COLOR_ORDER         GRB       // Type de code à envoyer pour les WS2812B
#define CHIPSET             WS2812B   // Type de LEDs du bandeau

// initial matrix layout (to get led strip index by x/y)
#define MATRIX_WIDTH        16                                  // Taille X de la matrice
#define MATRIX_HEIGHT       16                                  // Taille Y de la matrice
#define MATRIX_TYPE         HORIZONTAL_ZIGZAG_MATRIX            // Organisation des LEDs
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)        // Nombre total de LEDs
#define NUMPIXELS           MATRIX_SIZE                         // Nombre total de LEDs

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;      // Créer l'instance LEDMatrix

#define pause 300

// Open Led Race envoie les données de télémétrie sur le port série
// Plus d'infos : https://gitlab.com/open-led-race/olr-arduino/-/blob/master/doc/OLR_Protocol_Serial.pdf
// Variables pour stocker les données de la course
int numeroVoiture = 0;
int toursComplets = 0;
int pourcentageTour = 0;

void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds[0], leds.Size()).setCorrection(TypicalSMD5050);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(200);
  FastLED.clear(true);  // on éteint toutes les LEDs

}


void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.print("\n");
    Serial.print(data);
    Serial.print("\n");
    if (data.startsWith("T")) {
      // Analyse des données sur les voitures et les tours
      traiterDonnees(data);
    } else if (data.startsWith("R4")) {
      // Compte à rebours avant la course
      afficherCompteARebours();
      // Serial.print("C_A_R\n");
    } else if (data.startsWith("w")) {
      // Course terminée, affichage du gagnant
      afficherGagnant(data);
      // Serial.print("WIN\n");
      // afficherGagnant(data);

    }
  }

}

void traiterDonnees(String data) {
  String donnees = "";
  donnees = data.substring(2);

  // Trouver l'index de 'p' dans la chaîne de caractères
  int indexp = donnees.indexOf("p");

  // Trouver l'index de 'M' dans la chaîne de caractères
  int indexM = donnees.indexOf("M");

  // Trouver l'index de la première virgule dans la chaîne de caractères
  int indexVirgule1 = donnees.indexOf(",");

  // Trouver l'index de la deuxième virgule dans la chaîne de caractères
  int indexVirgule2 = donnees.indexOf(",", indexVirgule1 + 1);

  // Extraire le numéro de voiture de la chaîne de caractères et le convertir en entier
  int numeroVoiture = donnees.substring(indexp + 1, indexM).toInt();

  // Extraire le nombre de tours complets de la chaîne de caractères et le convertir en entier
  int toursComplets = donnees.substring(indexM + 1, indexVirgule1).toInt();

  // Extraire le pourcentage du tour de la chaîne de caractères et le convertir en entier
  int pourcentageTour = donnees.substring(indexVirgule1 + 1, indexVirgule2).toInt();

   Serial.print("Voiture : ");
   Serial.print(numeroVoiture);
   Serial.print("\n");
   Serial.print("Tours effectués : ");
   Serial.print(toursComplets); 
   Serial.print("\n");
   Serial.print("Tour en cours : ");
   Serial.print(pourcentageTour);
   Serial.print("\n");


  afficherInformationsCourse(numeroVoiture, toursComplets, pourcentageTour);

}

void afficherGagnant(String data) {
  // Afficher le gagnant sur la matrice LED
  if (data.startsWith("w1")) {
    // Voiture 1 gagne (rouge)
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Red));
  } else if (data.startsWith("w2")) {
    // Voiture 2 gagne (vert)
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Green));
  } else if (data.startsWith("w3")) {
    // Voiture 3 gagne (bleu)
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Blue));
  } else if (data.startsWith("w4")) {
    // Voiture 4 gagne (vert)
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::White));
  }
  FastLED.show();
  Serial.print(data);
  Serial.print("Le gagnant est : \n");

  delay(pause);

}

void afficherCompteARebours() {
  // Afficher 3
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Gray));
    leds.DrawFilledRectangle(5, 12, 11, 4, (CRGB::Orange));
    leds.DrawLine(6, 13, 10, 13, (CRGB::Black));
    leds.DrawPixel(5, 12, (CRGB::Black));
    leds.DrawPixel(11, 12, (CRGB::Black));
    leds.DrawLine(4, 11, 4, 8, (CRGB::Black));
    leds.DrawLine(12, 11, 12, 3, (CRGB::Black));
    leds.DrawLine(5, 7, 6, 7, (CRGB::Black));
    leds.DrawLine(4, 6, 4, 3, (CRGB::Black));
    leds.DrawPixel(5, 2, (CRGB::Black));
    leds.DrawPixel(11, 2, (CRGB::Black));
    leds.DrawLine(6, 1, 10, 1, (CRGB::Black));
    leds.DrawFilledRectangle(5, 9, 6, 8, (CRGB::Maroon));
    leds.DrawFilledRectangle(7, 10, 7, 9, (CRGB::Maroon));
    leds.DrawPixel(8, 10, (CRGB::Maroon));
    leds.DrawPixel(5, 4, (CRGB::Maroon));
    leds.DrawLine(5, 3, 11, 3, (CRGB::Maroon));
    leds.DrawLine(6, 2, 10, 2, (CRGB::Maroon));
    leds.DrawLine(10, 8, 11, 8, (CRGB::Maroon));
    leds.DrawLine(7, 6, 8, 6, (CRGB::Maroon));
    FastLED.show();
    delay(2000);

  // Afficher 2
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Gray));
    leds.DrawFilledRectangle(5, 12, 11, 4, (CRGB::Orange));
    leds.DrawLine(6, 13, 10, 13, (CRGB::Black));
    leds.DrawPixel(5, 12, (CRGB::Black));
    leds.DrawPixel(11, 12, (CRGB::Black));
    leds.DrawLine(4, 11, 4, 8, (CRGB::Black));
    leds.DrawLine(12, 11, 12, 2, (CRGB::Black));
    leds.DrawPixel(5, 7, (CRGB::Black));
    leds.DrawLine(4, 6, 4, 2, (CRGB::Black));
    leds.DrawLine(5, 1, 11, 1, (CRGB::Black));
    leds.DrawFilledRectangle(5, 9, 6, 8, (CRGB::Maroon));
    leds.DrawFilledRectangle(7, 10, 7, 9, (CRGB::Maroon));
    leds.DrawPixel(8, 10, (CRGB::Maroon));
    leds.DrawLine(5, 3, 11, 3, (CRGB::Maroon));
    leds.DrawFilledRectangle(5, 3, 11, 2, (CRGB::Maroon));
    leds.DrawLine(10, 7, 11, 7, (CRGB::Maroon));
    leds.DrawLine(9, 6, 11, 6, (CRGB::Maroon));
    FastLED.show();
    delay(2000);

  // Afficher 1
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Gray));
    leds.DrawFilledRectangle(7, 12, 11, 4, (CRGB::Orange));
    leds.DrawLine(8, 13, 10, 13, (CRGB::Black));
    leds.DrawPixel(7, 12, (CRGB::Black));
    leds.DrawLine(11, 12, 11, 2, (CRGB::Black));
    leds.DrawLine(6, 11, 6, 8, (CRGB::Black));
    leds.DrawLine(8, 1, 10, 1, (CRGB::Black));
    leds.DrawLine(7, 7, 7, 2, (CRGB::Black));
    leds.DrawFilledRectangle(8, 3, 10, 2, (CRGB::Maroon));
    leds.DrawLine(7, 9, 7, 8, (CRGB::Maroon));
    FastLED.show();
    delay(2000);

  // Allumer GO! en vert
    leds.DrawFilledRectangle(0, 0, 15, 15, (CRGB::Yellow));
    leds.DrawFilledRectangle(1, 11, 2, 3, (CRGB::Black));
    leds.DrawLine(2, 12, 5, 12, (CRGB::Black));
    leds.DrawLine(5, 11, 6, 11, (CRGB::Black));
    leds.DrawLine(2, 12, 5, 12, (CRGB::Black));
    leds.DrawLine(5, 11, 6, 11, (CRGB::Black));
    leds.DrawFilledRectangle(5, 6, 6, 3, (CRGB::Black));
    leds.DrawLine(3, 2, 4, 2, (CRGB::Black));
    leds.DrawPixel(6, 2, (CRGB::Black));
    leds.DrawPixel(4, 6, (CRGB::Black));

    leds.DrawFilledRectangle(8, 11, 9, 3, (CRGB::Black));
    leds.DrawFilledRectangle(11, 11, 12, 3, (CRGB::Black));
    leds.DrawLine(9, 12, 11, 12, (CRGB::Black));
    leds.DrawLine(9, 2, 11, 2, (CRGB::Black));

    leds.DrawLine(14, 12, 14, 4, (CRGB::Black));
    leds.DrawPixel(14, 2, (CRGB::Black));


    FastLED.show();
    delay(1000);
    FastLED.clear(true);  // on éteint toutes les LEDs

}

void afficherInformationsCourse(int numeroVoiture, int toursComplets, int pourcentageTour) {
  // Afficher les informations sur la matrice LED
  // Déterminer la couleur de la voiture en fonction du numéro
  // position y du rectangle représentant la voiture
  // 1 = Rouge, 2 = Vert, 3 = Bleu, 4 = Blanc
  CRGB color;      // Couleur du tour en cours
  CRGB color2;     // Couleur du nombre de tours finis
  int y=0;

  // Déterminer le nombre de LED à allumer en fonction du pourcentage du tour
  // on allume les 12 premières LED pour 100% du tour
  int n = map(pourcentageTour, 0, 100, 0, 12);
  Serial.print("n vaut : ");
  Serial.print(n);
  Serial.print("\n");

  // Traitement pour chaque voiture
  switch (numeroVoiture) {
    //  Voiture Rouge
    case 1:
      color = CRGB::DarkRed;
      color2 = CRGB::Red;
      y = 12;
      break;
    // Voiture Verte  
    case 2:
      color = CRGB::DarkGreen;
      color2 = CRGB::Green;
      y = 8;
      break;
    // Voiture Bleue   
    case 3:
      color = CRGB::DarkBlue;
      color2 = CRGB::Blue;
      y = 4;
      break;
    // Voiture Blanche
    case 4:
      color = CRGB::Grey;
      color2 = CRGB::White;
      y = 0;
      break;

    default:
      color = CRGB::Black;
      break;
  }  

  // Au départ on reçoit p1M1,0,100
  // Quand on est dans le premier tour on reçoit p1
  
  // Si le pourcentage est à 0 et le nombre de tour à UN
  // on n'allume aucune LED pour cette voiture
  if ((pourcentageTour == 0 ) && (toursComplets == 1)) {
    // Eteindre le rectangle complet
    leds.DrawFilledRectangle(0, y, 15, y+3, (CRGB::Black));
    Serial.print("Aucune LED ======================= \n");
    FastLED.show();
  }
  // Si le nombre de tours est à UN on n'affiche que la position dans ce tour
  if (toursComplets == 1 && (pourcentageTour != 0)) {
    // Eteindre le rectangle complet
    leds.DrawFilledRectangle(0, y, 15, y+3, (CRGB::Black));
    // Allumer les LED correspondant au % de tour effectué
    leds.DrawFilledRectangle(0, y, n, y+3, color);
    Serial.print("Que la position dans le tour ==== \n");
    FastLED.show();
  }

  // Si le nombre de tours est supérieur à 1
  if (toursComplets >1) {
    // Eteindre le rectangle complet de la voiture, y compris le nombre de tours
    leds.DrawFilledRectangle(0, y, 15, y+3, (CRGB::Black));
    // Allumer les LED correspondant au % de tour effectué
    leds.DrawFilledRectangle(0, y, n, y+3, color);
    // Allumer le nombre de LEDs correspondant au nombre de tours complets
    leds.DrawFilledRectangle(12, y, 10 + toursComplets, y+3, color2);
    Serial.print("Position + Nombre de tours ===== \n");
    FastLED.show();
  }
}

