#include "serviceitem.h"

ServiceItem::ServiceItem(ServiceItemType itemType, ServiceTime itemTime, int remainingDistance, int remainingTime) {
    this->type = itemType;
    this->time = itemTime;
    this->remainingDistance = remainingDistance;
    this->remainingTime = remainingTime;
}

QString ServiceItem::toString() {
    return QString("%1").arg((int)this->type) + QString(";0x%1;%2;%3").arg((int)this->time).arg(this->remainingDistance).arg(this->remainingTime);
}

QString TimeStringValue(ServiceTime time) {
    return time == ok ? "OK" : time == late ? "Late" : "Overdue";
}

int IndexForTime(ServiceTime time) {
    switch (time) {
    case ok: return 0;
    case late: return 1;
    case over: return 2;
    }
}

QStringList ServiceTimeList() {
    return QStringList() << TimeStringValue(ok) << TimeStringValue(late) << TimeStringValue(over);
}

int IndexForService(ServiceItemType type) {
    switch (type) {
    case engineOil: return 0;
    case frontBrake: return 1;
    case brakeFluid: return 2;
    case microFilter: return 3;
    case recirculatingAirFilter: return 4;
    case rearBreak: return 5;
    case dieselFilter: return 6;
    case fuelFilter: return 7;
    case sparkPlugs: return 8;
    case airFilter: return 9;
    case fuelFilterAlt: return 10;
    case sparkPlugsAlt: return 11;
    case vehicleCheckManual: return 12;
    case coolant: return 13;
    case preDelivery: return 14;
    case serviceInspection: return 15;
    case dieselAdditive: return 16;
    case statutoryInspection: return 17;
    case emissionsTest: return 18;
    case airFilterAlt: return 19;
    case vehicleCheckAlt: return 20;
    case vehicleCheck: return 21;
    case sparkPlugsAlt2: return 22;
    case microFilterAlt: return 23;
    case recirculatingAirFilterAlt: return 24;
    }
}

ServiceItemType ServiceForIndex(int index) {
    switch (index) {
    case 0: return engineOil;
    case 1: return frontBrake;
    case 2: return brakeFluid;
    case 3: return microFilter;
    case 4: return recirculatingAirFilter;
    case 5: return rearBreak;
    case 6: return dieselFilter;
    case 7: return fuelFilter;
    case 8: return sparkPlugs;
    case 9: return airFilter;
    case 10: return fuelFilterAlt;
    case 11: return sparkPlugsAlt;
    case 12: return vehicleCheckManual;
    case 13: return coolant;
    case 14: return preDelivery;
    case 15: return serviceInspection;
    case 16: return dieselAdditive;
    case 17: return statutoryInspection;
    case 18: return emissionsTest;
    case 19: return airFilterAlt;
    case 20: return vehicleCheckAlt;
    case 21: return vehicleCheck;
    case 22: return sparkPlugsAlt2;
    case 23: return microFilterAlt;
    case 24: return recirculatingAirFilterAlt;
    default: return engineOil;
    }
}

QString ServiceStringValue(ServiceItemType item) {
    switch (item) {
    case engineOil: return "Engine oil";
    case frontBrake: return "Front brakes";
    case brakeFluid: return "Brake fluid";
    case microFilter: return "Microfilter";
    case recirculatingAirFilter: return "Recirculating air filter";
    case rearBreak: return "Rear brakes";
    case dieselFilter: return "Diesel filter";
    case fuelFilter: return "Fuel filter";
    case sparkPlugs: return "Spark plugs";
    case airFilter: return "Air filter";
    case fuelFilterAlt: return "Fuel filter (alt)";
    case sparkPlugsAlt: return "Spark plugs (alt)";
    case vehicleCheckManual: return "Vehicle check";
    case coolant: return "Coolant";
    case preDelivery: return "Pre-delivery check";
    case serviceInspection: return "Service inspection";
    case dieselAdditive: return "Diesel additive";
    case statutoryInspection: return "Statutory inspection";
    case emissionsTest: return "Emissions test";
    case airFilterAlt: return "Air filter (alt)";
    case vehicleCheckAlt: return "Vehicle check (alt)";
    case vehicleCheck: return "Vehicle check";
    case sparkPlugsAlt2: return "Spark plugs (alt 2)";
    case microFilterAlt: return "Microfilter (alt)";
    case recirculatingAirFilterAlt: return "Recirculating air filter (alt)";
    }
}

QStringList ServiceItemsList() {
    return QStringList() << ServiceStringValue(engineOil) << ServiceStringValue(frontBrake) << ServiceStringValue(brakeFluid) << ServiceStringValue(microFilter)
                         << ServiceStringValue(recirculatingAirFilter) << ServiceStringValue(rearBreak) << ServiceStringValue(dieselFilter) << ServiceStringValue(fuelFilter)
                         << ServiceStringValue(sparkPlugs) << ServiceStringValue(airFilter) << ServiceStringValue(fuelFilterAlt) << ServiceStringValue(sparkPlugsAlt)
                         << ServiceStringValue(vehicleCheckManual) << ServiceStringValue(coolant) << ServiceStringValue(preDelivery) << ServiceStringValue(serviceInspection)
                         << ServiceStringValue(dieselAdditive) << ServiceStringValue(statutoryInspection) << ServiceStringValue(emissionsTest) << ServiceStringValue(airFilterAlt)
                         << ServiceStringValue(vehicleCheckAlt) << ServiceStringValue(vehicleCheck) << ServiceStringValue(sparkPlugsAlt2) << ServiceStringValue(microFilterAlt)
                         << ServiceStringValue(recirculatingAirFilterAlt);
}
