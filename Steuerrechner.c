#include <stdio.h>
#include <stdlib.h>

/*

liest Transatkionen und verwaltet einen Kontostand

Ihr Programm liest abwechselnd Vorgangstyp und Betrag ein und
gibt als Zwischenergebnis den aktuellen Kontostand aus.
Nach Abschluss der Eingabe gibt das Programm das Restguthaben,
die Anzahl an Einnahmen und Ausgaben, die durchschnittlichen Einnahmen
und Ausgaben und die gezahlten Steuern aus.

Zu Beginn gibt das Programm das Guthaben aus, das am Anfang 100 Euro sind.

*/
double Eingabe();
double Einnahme_besteuert(double betrag, int anzahlSteuersaetze, double teilbetragUntereGrenze[], double steuersatz[]);
void Teilbetraege(double betrag, double* teilbetrag, int anzahlSteuersaetze, double teilbetragUntereGrenze[]);
double Steuerfreie_Einnahme(double betrag);
double Kosten(double kosten, double guthaben);


int main()
{
    //Variablen
    double guthaben = 100.00f;
    double steuern = 0.00f;             //Gezahlte Steuern bis jetzt
    double derzeitigeSteuern = 0.00f;   //Steuern von der derzeitigen Eingabe
    char typ = 'x';         //Vorgangstyp
    double betrag = 0.0f;   //Eingegebener Betrag
    int einnahmenAnzahl = 0;
    int ausgabenAnzahl = 0;
    double einnahmen = 0.00f;
    double derzeitigeEinnahmen = 0.00f;
    double ausgaben = 0.00f;
    double derzeitigeAusgaben = 0.00f;

    double durchschnittEinnahmen = 0.00f;
    double durchschnittAusgaben = 0.00f;

    //Variablen fuer Steuern
    int anzahlSteuersaetze = 4;
    double teilbetragUntereGrenze[anzahlSteuersaetze];
    double steuersatz[anzahlSteuersaetze];

    teilbetragUntereGrenze[0] = 0.00f;   //untere Grenze vom Teilbetrag. Fuer obere Grenze, siehe naechster Teilbetrag (falls vorhanden)
    steuersatz[0] = 0.0f;    //Der Teilbetrag bis 20 Euro, wird gar nicht besteuert.

    teilbetragUntereGrenze[1] = 20.00f;
    steuersatz[1] = 0.10f;   //Der Teilbetrag groesser als 20 Euro und kleiner gleich 50 Euro wird mit 10% besteuert.

    teilbetragUntereGrenze[2] = 50.00f;
    steuersatz[2] = 0.20f;  //Der Teilbetrag groesser als 50 Euro und kleiner gleich 200 Euro wird mit 20% besteuert.

    teilbetragUntereGrenze[3] = 200.00f;
    steuersatz[3] = 0.40f;  //Jeder Teilbetrag groesser als 200 Euro wird mit 40% besteuert.

    //Programmstart
    printf("\n");
    while (typ != '=')  //Programm endet bei Eingabe von '='
    {
        printf("Guthaben: %.02f Euro\n", guthaben);
        printf("Typ: ");
        scanf(" %c", &typ); //Leerzeichen, weil sonst \n in der Eingabe haengen bleibt
        printf("\n");
        if (typ == '=') //Pruefen zuerst, ob Programm beendet wird
        {
            break;
        }
        betrag = Eingabe();
        if (betrag < 0) //Betrag negativ: Ungueltig, Eingabe wird ignoriert
        {
            continue;
        }

        switch(typ)
        {
            case 'e': derzeitigeSteuern = Einnahme_besteuert(betrag, anzahlSteuersaetze, teilbetragUntereGrenze, steuersatz);
                      derzeitigeEinnahmen = (betrag - derzeitigeSteuern);
                      guthaben += derzeitigeEinnahmen;
                      einnahmen += derzeitigeEinnahmen;
                      steuern += derzeitigeSteuern;
                      einnahmenAnzahl++;
            break;
            case 's': derzeitigeEinnahmen = Steuerfreie_Einnahme(betrag);
                      guthaben += derzeitigeEinnahmen;
                      einnahmen += derzeitigeEinnahmen;
                      einnahmenAnzahl++;
            break;
            case 'k': derzeitigeAusgaben = Kosten(betrag, guthaben);
                      if (derzeitigeAusgaben == 0)
                      {
                          break;
                      }
                      guthaben -= derzeitigeAusgaben;
                      ausgaben += derzeitigeAusgaben;
                      ausgabenAnzahl++;
            break;
        }
    }


    //folgende ifs sind da, um dividiert durch 0 zu vermeiden, wenn Durchschnitt berechnet wird und Einnahmen Anzahl gleich 0 ist
    if (einnahmenAnzahl == 0)
    {
        durchschnittEinnahmen = 0;
    }
    else
    {
        durchschnittEinnahmen = einnahmen / einnahmenAnzahl;
    }

    if (ausgabenAnzahl == 0)
    {
        durchschnittAusgaben = 0;
    }
    else
    {
        durchschnittAusgaben = ausgaben / ausgabenAnzahl;
    }


    printf("Restguthaben: %.02f Euro\n", guthaben);
    printf("%d Einnahmen mit durchschnittlichem Wert %.02f Euro\n", einnahmenAnzahl, durchschnittEinnahmen);
    printf("%d Ausgaben mit durchschnittlichem Wert %.02f Euro\n", ausgabenAnzahl, durchschnittAusgaben);
    printf("Gezahlte Steuern: %.02f Euro", steuern);
    return 0;
}

//liest Betrag ein
double Eingabe()
{
    double betrag = 0.0f;
    printf("Betrag in Euro: ");
    scanf("%lf", &betrag);
    if (betrag < 0)
    {
        printf("\nDer Betrag kann nicht negativ sein.\n");
        return -1;
    }
    printf("\n");
    return betrag;
}

//Fuer Eingabe 'e', besteuert die Eingabe nach Steuersatz und Teilbetraege
double Einnahme_besteuert(double betrag, int anzahlSteuersaetze, double teilbetragUntereGrenze[], double steuersatz[])
{
    double steuern = 0.0f;

    //Zuerst Betrag in Teilbetraege aufteilen
    double teilbetrag[anzahlSteuersaetze];
    Teilbetraege(betrag, &teilbetrag[0], anzahlSteuersaetze, teilbetragUntereGrenze);

    for (int i=0; i<(anzahlSteuersaetze); i++)
    {
        steuern += teilbetrag[i] * steuersatz[i];
    }

    printf("Gezahlte Steuern: %.02f Euro\n", steuern);
    return steuern;
}


//Zum Aufteilen des Betrags in Teilbetraege, nur in Funktion Einnahme_besteuert verwendet
void Teilbetraege(double betrag, double* teilbetrag, int anzahlSteuersaetze, double teilbetragUntereGrenze[])
{
    for (int i=0; i<(anzahlSteuersaetze-1); i++)    //Wir gehen alle Teilbetraege durch, ausser den letzten, der kein oberes Limit hat
    {
        if (betrag > teilbetragUntereGrenze[i] && betrag <= teilbetragUntereGrenze[i+1])    //Falls Betrag genau dazwischen (z.b. 30 ist zwischen  [20-50])...
        {
            *teilbetrag = betrag - teilbetragUntereGrenze[i];                               //Teilbetrag ist der Betrag, minus untere Grenze (also bei 30 waere es 10, da 30-20=10)
        }
        else if (betrag > teilbetragUntereGrenze[i] && betrag > teilbetragUntereGrenze[i+1])// Wenn sogar groesser als dieser Teilbetrag (z.b. 60 ist groesser als [20-50]...
        {
            *teilbetrag = teilbetragUntereGrenze[i+1] - teilbetragUntereGrenze[i];         //dann ist Teilbetrag die Differenz obere Grenze und untere Grenze,
                                                                                            //also bei 60 ist die Groesse von Teilbereich [20-50] gleich 30 (50-20)
        }
        else if (betrag < teilbetragUntereGrenze[i])                                           //Falls Betrag gar nicht im Teilbetrag dabei ist (z.b ist 10 kleiner als [20-50])
        {
            *teilbetrag = 0;                                                                //dann ist der Teilbetrag leer
        }
        teilbetrag++;
    }

    if (betrag > teilbetragUntereGrenze[anzahlSteuersaetze-1])  //Wir gehen den letzten Teilbetrag ausserhalb der Schleife an, da keine obere Grenze vorhanden ist die wir pruefen koennten
    {
        *teilbetrag = betrag - teilbetragUntereGrenze[anzahlSteuersaetze-1];
    }
    else
    {
        *teilbetrag = 0;
    }
}

//Fuer Eingabe 's', steuerfreie Einnahme
double Steuerfreie_Einnahme(double betrag)
{
    return betrag;
}

//Fuer Eingabe 'k', Kosten
double Kosten(double kosten, double guthaben)
{
    if (kosten > guthaben)
    {
        printf("Diese Kosten koennen nicht bezahlt werden.\n");
        return 0;   //eingabe wird ignoriert, Kosten sind 0
    }
    else
    {
        return kosten;
    }
}
