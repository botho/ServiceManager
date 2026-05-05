#ifndef SERVICEITEM_H
#define SERVICEITEM_H

#include <QObject>
#include <QString>

enum ServiceItemType {
    engineOil=1,                       // Huile moteur
    frontBrake=2,                      // Freins avant
    brakeFluid=3,                      // Liquide de frein
    microFilter=4,                     // Microfiltre
    recirculatingAirFilter=5,          // Filtre à air de recirculation
    rearBreak=6,                       // Freins arrière
    dieselFilter=7,                    // Filtre à gazole
    fuelFilter=8,                      // Filtre à carburant
    sparkPlugs=10,                     // Bougies d'allumage
    airFilter=11,                      // Filtre à air
    fuelFilterAlt=12,                  // Filtre à carburant (alternatif)
    sparkPlugsAlt=16,                  // Bougies d'allumage (alternatif)
    vehicleCheckManual=17,             // Contrôle du véhicule
    coolant=18,                        // Liquide de refroidissement
    preDelivery=20,                    // Contrôle avant livraison
    serviceInspection=21,              // Inspection de service
    dieselAdditive=22,                 // Additif pour gazole
    statutoryInspection=32,            // Contrôle réglementaire
    emissionsTest=33,                  // Contrôle des émissions
    airFilterAlt=34,                   // Filtre à air (alternatif)
    vehicleCheckAlt=90,                // Contrôle du véhicule (alternatif)
    vehicleCheck=100,                  // Contrôle du véhicule
    sparkPlugsAlt2=118,                // Bougies d'allumage (alternatif 2)
    microFilterAlt=119,                // Microfiltre (alternatif)
    recirculatingAirFilterAlt=219      // Filtre à air de recirculation (alternatif)
};

enum ServiceTime { ok=1, late=2, over=3 };

QString TimeStringValue(ServiceTime time);
QString ServiceStringValue(ServiceItemType item);

int IndexForTime(ServiceTime time);
int IndexForService(ServiceItemType type);
ServiceItemType ServiceForIndex(int index);

QStringList ServiceItemsList();
QStringList ServiceTimeList();

class ServiceItem {
public:
    ServiceItemType type;
    ServiceTime time;
    int remainingDistance;  // Distance restante en km (peut être négative)
    int remainingTime;      // Temps restant en mois (ne peut pas être négatif)

    ServiceItem(ServiceItemType, ServiceTime, int remainingDistance = 0, int remainingTime = 0);
    QString toString();
};

#endif // SERVICEITEM_H
