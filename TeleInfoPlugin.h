#ifndef TELEINFOPLUGIN_H
#define TELEINFOPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QComboBox>
#include <QTcpSocket>
#include <QThread>
#include <QElapsedTimer>
#include <QTimer>

#include "ui_TeleInfo.h"
#include "../interface.h"


#ifdef Q_OS_WIN32
#include "windows.h"
#include "Winsock2.h"
#endif

#ifdef Q_OS_LINUX
#include "sys/socket.h"
#endif

static const char STX = 0x02;			// Frame Start
static const char ETX = 0x03;			// Frame End
static const char EOT = 0x04;			// Frame Interrupted
static const char LF = 0x0A;			// Line Feed
static const char SP = 0x20;			// Space
static const char CR = 0x0D;			// Carriage Return
static const int DataTimeOut = 60000;



enum TeleInfoValeur
    {
        ADCO = 0, OPTARIF, ISOUSC, BASE, HCHC, HCHP, EJPHN, EJPPM, EJPHPM, BBRHCJB, \
    BBRHPJB, BBRHCJW, BBRHPJW, BBRHCJR, BBRHPJR, PEJP, PTEC, DEMAIN, IINST, ADPS, \
    IMAX, PAPP, HHPHC, MOTDETAT, IINST1, IINST2, IINST3, IMAX1, IMAX2, IMAX3, \
    ADSC, VTIC, DATE, NGTF, LTARF, EAST, EASF01, EASF02, EASF03, EASF04, EASF05, \
    EASF06, EASF07, EASF08, EASF09, EASF10, EASD01, EASD02, EASD03, EASD04, EAIT, \
    ERQ1, ERQ2, ERQ3, ERQ4, IRMS1, IRMS2, IRMS3, URMS1, URMS2, URMS3, PREF, PCOUP, SINSTS, \
    SINSTS1, SINSTS2, SINSTS3, SMAXSN, SMAXSN1, SMAXSN2, SMAXSN3, \
    SMAXSN_1, SMAXSN1_1, SMAXSN2_1, SMAXSN3_1, SINSTI, SMAXIN, SMAXIN_1, \
    CCASN, CCASN_1, CCAIN, CCAIN_1, UMOY1, UMOY2, UMOY3, STGE, DPM1, FPM1, \
    DPM2, FPM2, DPM3, FPM3, MSG1, MSG2, PRM, RELAIS, NTARF, NJOURF, \
    NJOUR_1, PJOUR_1, PPOINTE, UserDefined, TeleInfoValMax
    };


static QString TeleInfoParamtoStr(int index)
{
    QString str;
    switch(index)
    {
        case ADCO :	str = "ADCO"; break;
        case OPTARIF :	str = "OPTARIF"; break;
        case ISOUSC :	str = "ISOUSC"; break;
        case BASE :	str = "BASE"; break;
        case HCHC :	str = "HCHC"; break;
        case HCHP :	str = "HCHP"; break;
        case EJPHN :	str = "EJPHN"; break;
        case EJPPM :	str = "EJPPM"; break;
        case EJPHPM :	str = "EJPHPM"; break;
        case BBRHCJB :	str = "BBRHCJB"; break;
        case BBRHPJB :	str = "BBRHPJB"; break;
        case BBRHCJW :	str = "BBRHCJW"; break;
        case BBRHPJW :	str = "BBRHPJW"; break;
        case BBRHCJR :	str = "BBRHCJR"; break;
        case BBRHPJR :	str = "BBRHPJR"; break;
        case PEJP :	str = "PEJP"; break;
        case PTEC :	str = "PTEC"; break;
        case DEMAIN :	str = "DEMAIN"; break;
        case IINST :	str = "IINST"; break;
        case IINST1 :	str = "IINST1"; break;
        case IINST2 :	str = "IINST2"; break;
        case IINST3 :	str = "IINST3"; break;
        case ADPS :	str = "ADPS"; break;
        case IMAX :	str = "IMAX"; break;
        case IMAX1 :	str = "IMAX1"; break;
        case IMAX2 :	str = "IMAX2"; break;
        case IMAX3:	str = "IMAX3"; break;
        case PAPP :	str = "PAPP"; break;
        case HHPHC :	str = "HHPHC"; break;
        case MOTDETAT :	str = "MOTDETAT"; break;
        case ADSC :	str = "ADSC"; break;
        case VTIC :	str = "VTIC"; break;
        case DATE :	str = "DATE"; break;
        case NGTF :	str = "NGTF"; break;
        case LTARF :	str = "LTARF"; break;
        case EAST :	str = "EAST"; break;
        case EASF01 :	str = "EASF01"; break;
        case EASF02 :	str = "EASF02"; break;
        case EASF03 :	str = "EASF03"; break;
        case EASF04 :	str = "EASF04"; break;
        case EASF05 :	str = "EASF05"; break;
        case EASF06 :	str = "EASF06"; break;
        case EASF07 :	str = "EASF07"; break;
        case EASF08 :	str = "EASF08"; break;
        case EASF09 :	str = "EASF09"; break;
        case EASF10 :	str = "EASF10"; break;
        case EASD01 :	str = "EASD01"; break;
        case EASD02 :	str = "EASD02"; break;
        case EASD03 :	str = "EASD03"; break;
        case EASD04 :	str = "EASD04"; break;
        case EAIT :	str = "EAIT"; break;
        case ERQ1 :	str = "ERQ1"; break;
        case ERQ2 :	str = "ERQ2"; break;
        case ERQ3 :	str = "ERQ3"; break;
        case ERQ4 :	str = "ERQ4"; break;
        case IRMS1 :	str = "IRMS1"; break;
        case IRMS2 :	str = "IRMS2"; break;
        case IRMS3 :	str = "IRMS3"; break;
        case URMS1 :	str = "URMS1"; break;
        case URMS2 :	str = "URMS2"; break;
        case URMS3 :	str = "URMS3"; break;
        case PREF :	str = "PREF"; break;
        case PCOUP :	str = "PCOUP"; break;
        case SINSTS :	str = "SINSTS"; break;
        case SINSTS1 :	str = "SINSTS1"; break;
        case SINSTS2 :	str = "SINSTS2"; break;
        case SINSTS3 :	str = "SINSTS3"; break;
        case SMAXSN :	str = "SMAXSN"; break;
        case SMAXSN1 :	str = "SMAXSN1"; break;
        case SMAXSN2 :	str = "SMAXSN2"; break;
        case SMAXSN3 :	str = "SMAXSN3"; break;
        case SMAXSN_1 :	str = "SMAXSN-1"; break;
        case SMAXSN1_1 :	str = "SMAXSN1-1"; break;
        case SMAXSN2_1 :	str = "SMAXSN2-1"; break;
        case SMAXSN3_1 :	str = "SMAXSN3-1"; break;
        case SINSTI :	str = "SINSTI"; break;
        case SMAXIN :	str = "SMAXIN"; break;
        case SMAXIN_1 :	str = "SMAXIN-1"; break;
        case CCASN :	str = "CCASN"; break;
        case CCASN_1 :	str = "CCASN-1"; break;
        case CCAIN :	str = "CCAIN"; break;
        case CCAIN_1 :	str = "CCAIN-1"; break;
        case UMOY1 :	str = "UMOY1"; break;
        case UMOY2 :	str = "UMOY2"; break;
        case UMOY3 :	str = "UMOY3"; break;
        case STGE :	str = "STGE"; break;
        case DPM1 :	str = "DPM1"; break;
        case FPM1 :	str = "FPM1"; break;
        case DPM2 :	str = "DPM2"; break;
        case FPM2 :	str = "FPM2"; break;
        case DPM3 :	str = "DPM3"; break;
        case FPM3 :	str = "FPM3"; break;
        case MSG1 :	str = "MSG1"; break;
        case MSG2 :	str = "MSG2"; break;
        case PRM :	str = "PRM"; break;
        case RELAIS :	str = "RELAIS"; break;
        case NTARF :	str = "NTARF"; break;
        case NJOURF :	str = "NJOURF"; break;
        case NJOUR_1 :	str = "NJOURF+1"; break;
        case PJOUR_1 :	str = "PJOURF+1"; break;
        case PPOINTE :	str = "PPOINTE"; break;
        default : 	str = ""; break;
    }
    return str;
}

static QString TeleInfoValeurtoStr(int index)
{
    QString str;
    switch(index)
    {
        case ADCO :	str = "Adresse du compteur (ADCO)"; break;
        case OPTARIF :	str = "Option tarifaire choisie (OPTARIF)"; break;
        case ISOUSC :	str = "Intensité souscrite (ISOUSC)"; break;
        case BASE :	str = "Index option Base (BASE)"; break;
        case HCHC :	str = "Heures Creuses (HCHC)"; break;
        case HCHP :	str = "Heures Pleines (HCHP)"; break;
        case EJPHN :	str = "Heures Normales (EJPHN)"; break;
        case EJPPM :	str = "Heures de Pointes EJP (EJPPM)"; break;
        case EJPHPM :	str = "Heures de Pointes EJP (EJPHPM)"; break;
        case BBRHCJB :	str = "Heures Creuses Jours Bleus (BBRHCJB)"; break;
        case BBRHPJB :	str = "Heures Pleines Jours Bleus (BBRHPJB)"; break;
        case BBRHCJW :	str = "Heures Creuses Jours Blancs (BBRHCJW)"; break;
        case BBRHPJW :	str = "Heures Pleines Jours Blancs (BBRHPJW)"; break;
        case BBRHCJR :	str = "Heures Creuses Jours Rouges (BBRHCJR)"; break;
        case BBRHPJR :	str = "Heures Pleines Jours Rouges (BBRHPJR)"; break;
        case PEJP :	str = "Préavis Début EJP (30 min) (PEJP)"; break;
        case PTEC :	str = "Période Tarifaire en cours (PTEC)"; break;
        case DEMAIN :	str = "Couleur du lendemain (DEMAIN)"; break;
        case IINST :	str = "Intensité Instantanée (IINST)"; break;
        case ADPS :	str = "Avertissement de Dépassement de Puissance Souscrite (ADPS)"; break;
        case IMAX :	str = "Intensité maximale appelée (IMAX)"; break;
        case PAPP :	str = "Puissance apparente (PAPP)"; break;
        case HHPHC :	str = "Horaire Heures Pleines Heures Creuses (HHPHC)"; break;
        case MOTDETAT :	str = "Mot d'état du compteur (MOTDETAT)"; break;
        case IINST1 :	str = "Intensité Instantanée Phase 1 (IINST1)"; break;
        case IINST2 :	str = "Intensité Instantanée Phase 2 (IINST2)"; break;
        case IINST3 :	str = "Intensité Instantanée Phase 3 (IINST3)"; break;
        case IMAX1 :	str = "Intensité maximale appelée Phase 1 (IMAX1)"; break;
        case IMAX2 :	str = "Intensité maximale appelée Phase 2 (IMAX2)"; break;
        case IMAX3 :	str = "Intensité maximale appelée Phase 3 (IMAX3)"; break;
        case ADSC :	str = "Adresse Secondaire du Compteur (ADSC)"; break;
        case VTIC :	str = "Version de la TIC (VTIC)"; break;
        case DATE :	str = "Date et heure courante (DATE)"; break;
        case NGTF :	str = "Nom du calendrier tarifaire fournisseur (NGTF)"; break;
        case LTARF :	str = "Libellé tarif fournisseur en cours (LTARF)"; break;
        case EAST :	str = "Energie active soutirée totale (EAST)"; break;
        case EASF01 :	str = "Energie active soutirée Fournisseur, index 01 (EASF01)"; break;
        case EASF02 :	str = "Energie active soutirée Fournisseur, index 02 (EASF02)"; break;
        case EASF03 :	str = "Energie active soutirée Fournisseur, index 03 (EASF03)"; break;
        case EASF04 :	str = "Energie active soutirée Fournisseur, index 04 (EASF04)"; break;
        case EASF05 :	str = "Energie active soutirée Fournisseur, index 05 (EASF05)"; break;
        case EASF06 :	str = "Energie active soutirée Fournisseur, index 06 (EASF06)"; break;
        case EASF07 :	str = "Energie active soutirée Fournisseur, index 07 (EASF07)"; break;
        case EASF08 :	str = "Energie active soutirée Fournisseur, index 08 (EASF08)"; break;
        case EASF09 :	str = "Energie active soutirée Fournisseur, index 09 (EASF09)"; break;
        case EASF10 :	str = "Energie active soutirée Fournisseur, index 10 (EASF10)"; break;
        case EASD01 :	str = "Energie active soutirée Distributeur, index 01 (EASD01)"; break;
        case EASD02 :	str = "Energie active soutirée Distributeur, index 02 (EASD02)"; break;
        case EASD03 :	str = "Energie active soutirée Distributeur, index 03 (EASD03)"; break;
        case EASD04 :	str = "Energie active soutirée Distributeur, index 04 (EASD04)"; break;
        case EAIT :	str = "Energie active injectée totale (EAIT)"; break;
        case ERQ1 :	str = "Energie réactive Q1 totale (ERQ1)"; break;
        case ERQ2 :	str = "Energie réactive Q2 totale (ERQ2)"; break;
        case ERQ3 :	str = "Energie réactive Q3 totale (ERQ3)"; break;
        case ERQ4 :	str = "Energie réactive Q4 totale (ERQ4)"; break;
        case IRMS1 :	str = "Courant efficace, phase 1 (IRMS1)"; break;
        case IRMS2 :	str = "Courant efficace, phase 2 (IRMS2)"; break;
        case IRMS3 :	str = "Courant efficace, phase 3 (IRMS3)"; break;
        case URMS1 :	str = "Tension efficace, phase 1 (URMS1)"; break;
        case URMS2 :	str = "Tension efficace, phase 2 (URMS2)"; break;
        case URMS3 :	str = "Tension efficace, phase 3 (URMS3)"; break;
        case PREF :	str = "Puissance app. de référence (PREF)"; break;
        case PCOUP :	str = "Puissance app. de coupure (PCOUP)"; break;
        case SINSTS :	str = "Puissance app. Instantanée soutirée (SINSTS)"; break;
        case SINSTS1 :	str = "Puissance app. Instantanée soutirée phase 1 (SINSTS1)"; break;
        case SINSTS2 :	str = "Puissance app. Instantanée soutirée phase 2 (SINSTS2)"; break;
        case SINSTS3 :	str = "Puissance app. Instantanée soutirée phase 3 (SINSTS3)"; break;
        case SMAXSN :	str = "Puissance app. max. soutirée n (SMAXSN)"; break;
        case SMAXSN1 :	str = "Puissance app. max. soutirée n phase 1 (SMAXSN1)"; break;
        case SMAXSN2 :	str = "Puissance app. max. soutirée n phase 2 (SMAXSN2)"; break;
        case SMAXSN3 :	str = "Puissance app. max. soutirée n phase 3 (SMAXSN3)"; break;
        case SMAXSN_1 :	str = "Puissance app max. soutirée n-1 (SMAXSN-1)"; break;
        case SMAXSN1_1 :	str = "Puissance app max. soutirée n-1 phase 1 (SMAXSN1-1)"; break;
        case SMAXSN2_1 :	str = "Puissance app max. soutirée n-1 phase 2 (SMAXSN2-1)"; break;
        case SMAXSN3_1 :	str = "Puissance app max. soutirée n-1 phase 3 (SMAXSN3-1)"; break;
        case SINSTI :	str = "Puissance app. Instantanée injectée (SINSTI)"; break;
        case SMAXIN :	str = "Puissance app. max. injectée n (SMAXIN)"; break;
        case SMAXIN_1 :	str = "Puissance app max. injectée n-1 (SMAXIN-1)"; break;
        case CCASN :	str = "Point n de la courbe de charge active soutirée (CCASN)"; break;
        case CCASN_1 :	str = "Point n-1 de la courbe de charge active soutirée (CCASN_1)"; break;
        case CCAIN :	str = "Point n de la courbe de charge active injectée (CCAIN)"; break;
        case CCAIN_1 :	str = "Point n-1 de la courbe de charge active injectée (CCAIN_1)"; break;
        case UMOY1 :	str = "Tension moy. ph. 1 (UMOY1)"; break;
        case UMOY2 :	str = "Tension moy. ph. 2 (UMOY2)"; break;
        case UMOY3 :	str = "Tension moy. ph. 3 (UMOY3)"; break;
        case STGE :	str = "Registre de Statuts (STGE)"; break;
        case DPM1 :	str = "Début Pointe Mobile 1 (DPM1)"; break;
        case FPM1 :	str = "Fin Pointe Mobile 1 (FPM1)"; break;
        case DPM2 :	str = "Début Pointe Mobile 2 (DPM2)"; break;
        case FPM2 :	str = "Fin Pointe Mobile 2 (FPM2)"; break;
        case DPM3 :	str = "Début Pointe Mobile 3 (DPM3)"; break;
        case FPM3 :	str = "Fin Pointe Mobile 3 (FPM3)"; break;
        case MSG1 :	str = "Message court (MSG1)"; break;
        case MSG2 :	str = "Message Ultra court (MSG2)"; break;
        case PRM :	str = "PRM (PRM)"; break;
        case RELAIS :	str = "Relais (RELAIS)"; break;
        case NTARF :	str = "Numéro de l’index tarifaire en cours (NTARF)"; break;
        case NJOURF :	str = "Numéro du jour en cours calendrier fournisseur (NJOURF)"; break;
        case NJOUR_1 :	str = "Numéro du prochain jour calendrier fournisseur (NJOUR+1)"; break;
        case PJOUR_1 :	str = "Profil du prochain jour calendrier fournisseur (PJOUR+1)"; break;
        case PPOINTE :	str = "Profil du prochain jour de pointe (PPOINTE)"; break;
        case UserDefined :	str = "User Defined"; break;
        default : 	str = ""; break;
    }
    return str;
}

static QString NumBinding(int index)
{
    QString str;
    switch(index)
    {
        case ADCO :	str = "Adresse du compteur (ADCO)"; break;
        case OPTARIF :	str = "BASE = 0\nHC.. = 1\nEJP.=2"; break;
        case ISOUSC :	str = "Intensité souscrite (ISOUSC)"; break;
        case BASE :	str = "Index option Base (BASE)"; break;
        case HCHC :	str = "Heures Creuses (HCHC)"; break;
        case HCHP :	str = "Heures Pleines (HCHP)"; break;
        case EJPHN :	str = "Heures Normales (EJPHN)"; break;
        case EJPPM :	str = "Heures de Pointes EJP (EJPPM)"; break;
        case EJPHPM :	str = "Heures de Pointes EJP (EJPHPM)"; break;
        case BBRHCJB :	str = "Heures Creuses Jours Bleus (BBRHCJB)"; break;
        case BBRHPJB :	str = "Heures Pleines Jours Bleus (BBRHPJB)"; break;
        case BBRHCJW :	str = "Heures Creuses Jours Blancs (BBRHCJW)"; break;
        case BBRHPJW :	str = "Heures Pleines Jours Blancs (BBRHPJW)"; break;
        case BBRHCJR :	str = "Heures Creuses Jours Rouges (BBRHCJR)"; break;
        case BBRHPJR :	str = "Heures Pleines Jours Rouges (BBRHPJR)"; break;
        case PEJP :	str = "Préavis Début EJP (30 min) (PEJP)"; break;
        case PTEC :	str = "HC.. = 1\nTH.. = 2\nHN.. = 3\nPM.. = 4\nHCJB = 5\nHCJW = 6\nHCJR = 7\nHPJB = 8\nHPJW = 9\nHPJR = 10";
        case DEMAIN :	str = "BLEU = 1\nBLAN = 2\nROUG = 3"; break;
        case IINST :	str = "Intensité Instantanée (IINST)"; break;
        case ADPS :	str = "Avertissement de Dépassement de Puissance Souscrite (ADPS)"; break;
        case IMAX :	str = "Intensité maximale appelée (IMAX)"; break;
        case PAPP :	str = "Puissance apparente (PAPP)"; break;
        case HHPHC :	str = "Horaire Heures Pleines Heures Creuses (HHPHC)"; break;
        case MOTDETAT :	str = "Mot d'état du compteur (MOTDETAT)"; break;
        case IINST1 :	str = "Intensité Instantanée Phase 1 (IINST1)"; break;
        case IINST2 :	str = "Intensité Instantanée Phase 2 (IINST2)"; break;
        case IINST3 :	str = "Intensité Instantanée Phase 3 (IINST3)"; break;
        case IMAX1 :	str = "Intensité maximale appelée Phase 1 (IMAX1)"; break;
        case IMAX2 :	str = "Intensité maximale appelée Phase 2 (IMAX2)"; break;
        case IMAX3 :	str = "Intensité maximale appelée Phase 3 (IMAX3)"; break;
        case ADSC :	str = "Adresse Secondaire du Compteur (ADSC)"; break;
        case VTIC :	str = "Version de la TIC (VTIC)"; break;
        case DATE :	str = "Date et heure courante (DATE)"; break;
        case NGTF :	str = "Nom du calendrier tarifaire fournisseur (NGTF)"; break;
        case LTARF :	str = "Libellé tarif fournisseur en cours (LTARF)"; break;
        case EAST :	str = "Energie active soutirée totale (EAST)"; break;
        case EASF01 :	str = "Energie active soutirée Fournisseur, index 01 (EASF01)"; break;
        case EASF02 :	str = "Energie active soutirée Fournisseur, index 02 (EASF02)"; break;
        case EASF03 :	str = "Energie active soutirée Fournisseur, index 03 (EASF03)"; break;
        case EASF04 :	str = "Energie active soutirée Fournisseur, index 04 (EASF04)"; break;
        case EASF05 :	str = "Energie active soutirée Fournisseur, index 05 (EASF05)"; break;
        case EASF06 :	str = "Energie active soutirée Fournisseur, index 06 (EASF06)"; break;
        case EASF07 :	str = "Energie active soutirée Fournisseur, index 07 (EASF07)"; break;
        case EASF08 :	str = "Energie active soutirée Fournisseur, index 08 (EASF08)"; break;
        case EASF09 :	str = "Energie active soutirée Fournisseur, index 09 (EASF09)"; break;
        case EASF10 :	str = "Energie active soutirée Fournisseur, index 10 (EASF10)"; break;
        case EASD01 :	str = "Energie active soutirée Distributeur, index 01 (EASD01)"; break;
        case EASD02 :	str = "Energie active soutirée Distributeur, index 02 (EASD02)"; break;
        case EASD03 :	str = "Energie active soutirée Distributeur, index 03 (EASD03)"; break;
        case EASD04 :	str = "Energie active soutirée Distributeur, index 04 (EASD04)"; break;
        case EAIT :	str = "Energie active injectée totale (EAIT)"; break;
        case ERQ1 :	str = "Energie réactive Q1 totale (ERQ1)"; break;
        case ERQ2 :	str = "Energie réactive Q2 totale (ERQ2)"; break;
        case ERQ3 :	str = "Energie réactive Q3 totale (ERQ3)"; break;
        case ERQ4 :	str = "Energie réactive Q4 totale (ERQ4)"; break;
        case IRMS1 :	str = "Courant efficace, phase 1 (IRMS1)"; break;
        case IRMS2 :	str = "Courant efficace, phase 2 (IRMS2)"; break;
        case IRMS3 :	str = "Courant efficace, phase 3 (IRMS3)"; break;
        case URMS1 :	str = "Tension efficace, phase 1 (URMS1)"; break;
        case URMS2 :	str = "Tension efficace, phase 2 (URMS2)"; break;
        case URMS3 :	str = "Tension efficace, phase 3 (URMS3)"; break;
        case PREF :	str = "Puissance app. de référence (PREF)"; break;
        case PCOUP :	str = "Puissance app. de coupure (PCOUP)"; break;
        case SINSTS :	str = "Puissance app. Instantanée soutirée (SINSTS)"; break;
        case SINSTS1 :	str = "Puissance app. Instantanée soutirée phase 1 (SINSTS1)"; break;
        case SINSTS2 :	str = "Puissance app. Instantanée soutirée phase 2 (SINSTS2)"; break;
        case SINSTS3 :	str = "Puissance app. Instantanée soutirée phase 3 (SINSTS3)"; break;
        case SMAXSN :	str = "Puissance app. max. soutirée n (SMAXSN)"; break;
        case SMAXSN1 :	str = "Puissance app. max. soutirée n phase 1 (SMAXSN1)"; break;
        case SMAXSN2 :	str = "Puissance app. max. soutirée n phase 2 (SMAXSN2)"; break;
        case SMAXSN3 :	str = "Puissance app. max. soutirée n phase 3 (SMAXSN3)"; break;
        case SMAXSN_1 :	str = "Puissance app max. soutirée n-1 (SMAXSN-1)"; break;
        case SMAXSN1_1 :	str = "Puissance app max. soutirée n-1 phase 1 (SMAXSN1-1)"; break;
        case SMAXSN2_1 :	str = "Puissance app max. soutirée n-1 phase 2 (SMAXSN2-1)"; break;
        case SMAXSN3_1 :	str = "Puissance app max. soutirée n-1 phase 3 (SMAXSN3-1)"; break;
        case SINSTI :	str = "Puissance app. Instantanée injectée (SINSTI)"; break;
        case SMAXIN :	str = "Puissance app. max. injectée n (SMAXIN)"; break;
        case SMAXIN_1 :	str = "Puissance app max. injectée n-1 (SMAXIN-1)"; break;
        case CCASN :	str = "Point n de la courbe de charge active soutirée (CCASN)"; break;
        case CCASN_1 :	str = "Point n-1 de la courbe de charge active soutirée (CCASN_1)"; break;
        case CCAIN :	str = "Point n de la courbe de charge active injectée (CCAIN)"; break;
        case CCAIN_1 :	str = "Point n-1 de la courbe de charge active injectée (CCAIN_1)"; break;
        case UMOY1 :	str = "Tension moy. ph. 1 (UMOY1)"; break;
        case UMOY2 :	str = "Tension moy. ph. 2 (UMOY2)"; break;
        case UMOY3 :	str = "Tension moy. ph. 3 (UMOY3)"; break;
        case STGE :	str = "Registre de Statuts (STGE)"; break;
        case DPM1 :	str = "Début Pointe Mobile 1 (DPM1)"; break;
        case FPM1 :	str = "Fin Pointe Mobile 1 (FPM1)"; break;
        case DPM2 :	str = "Début Pointe Mobile 2 (DPM2)"; break;
        case FPM2 :	str = "Fin Pointe Mobile 2 (FPM2)"; break;
        case DPM3 :	str = "Début Pointe Mobile 3 (DPM3)"; break;
        case FPM3 :	str = "Fin Pointe Mobile 3 (FPM3)"; break;
        case MSG1 :	str = "Message court (MSG1)"; break;
        case MSG2 :	str = "Message Ultra court (MSG2)"; break;
        case PRM :	str = "PRM (PRM)"; break;
        case RELAIS :	str = "Relais (RELAIS)"; break;
        case NTARF :	str = "Numéro de l’index tarifaire en cours (NTARF)"; break;
        case NJOURF :	str = "Numéro du jour en cours calendrier fournisseur (NJOURF)"; break;
        case NJOUR_1 :	str = "Numéro du prochain jour calendrier fournisseur (NJOUR+1)"; break;
        case PJOUR_1 :	str = "Profil du prochain jour calendrier fournisseur (PJOUR+1)"; break;
        case PPOINTE :	str = "Profil du prochain jour de pointe (PPOINTE)"; break;
        case UserDefined :	str = "User Defined"; break;
        default : 	str = ""; break;
    }
    return str;
}


static int TeleInfoValeurLength(int index)
{
    int L;
    switch(index)
    {
        case ADCO :	 L = 12; break;
        case OPTARIF :	 L = 4; break;
        case ISOUSC :	 L = 2; break;
        case BASE :	 L = 9; break;
        case HCHC :	 L = 9; break;
        case HCHP :	 L = 9; break;
        case EJPHN :	 L = 9; break;
        case EJPPM :	 L = 9; break;
        case EJPHPM :	 L = 9; break;
        case BBRHCJB :	 L = 9; break;
        case BBRHPJB :	 L = 9; break;
        case BBRHCJW :	 L = 9; break;
        case BBRHPJW :	 L = 9; break;
        case BBRHCJR :	 L = 9; break;
        case BBRHPJR :	 L = 9; break;
        case PEJP :	 L = 2; break;
        case PTEC :	 L = 4; break;
        case DEMAIN :	 L = 4; break;
        case IINST :	 L = 3; break;
        case IINST1 :	 L = 3; break;
        case IINST2 :	 L = 3; break;
        case IINST3 :	 L = 3; break;
        case ADPS :	 L = 3; break;
        case IMAX1 :	 L = 3; break;
        case IMAX2 :	 L = 3; break;
        case IMAX3 :	 L = 3; break;
        case IMAX :	 L = 3; break;
        case PAPP :	 L = 5; break;
        case HHPHC :	 L = 1; break;
        case MOTDETAT :	 L = 6; break;
        case ADSC :	L = 12; break;
        case VTIC :	L = 2; break;
        case DATE :	L = 0; break;
        case NGTF :	L =16; break;
        case LTARF :	L = 16; break;
        case EAST :	L = 9; break;
        case EASF01 :	L = 9; break;
        case EASF02 :	L = 9; break;
        case EASF03 :	L = 9; break;
        case EASF04 :	L = 9; break;
        case EASF05 :	L = 9; break;
        case EASF06 :	L = 9; break;
        case EASF07 :	L = 9; break;
        case EASF08 :	L = 9; break;
        case EASF09 :	L = 9; break;
        case EASF10 :	L = 9; break;
        case EASD01 :	L = 9; break;
        case EASD02 :	L = 9; break;
        case EASD03 :	L = 9; break;
        case EASD04 :	L = 9; break;
        case EAIT :	L = 9; break;
        case ERQ1 :	L = 9; break;
        case ERQ2 :	L = 9; break;
        case ERQ3 :	L = 9; break;
        case ERQ4 :	L = 9; break;
        case IRMS1 :	L = 3; break;
        case IRMS2 :	L = 3; break;
        case IRMS3 :	L = 3; break;
        case URMS1 :	L = 3; break;
        case URMS2 :	L = 3; break;
        case URMS3 :	L = 3; break;
        case PREF :	L = 2; break;
        case PCOUP :	L = 2; break;
        case SINSTS :	L = 5; break;
        case SINSTS1 :	L = 5; break;
        case SINSTS2 :	L = 5; break;
        case SINSTS3 :	L = 5; break;
        case SMAXSN :	L = 5; break;
        case SMAXSN1 :	L = 5; break;
        case SMAXSN2 :	L = 5; break;
        case SMAXSN3 :	L = 5; break;
        case SMAXSN_1 :	L = 5; break;
        case SMAXSN1_1 :	L = 5; break;
        case SMAXSN2_1 :	L = 5; break;
        case SMAXSN3_1 :	L = 5; break;
        case SINSTI :	L = 5; break;
        case SMAXIN :	L = 5; break;
        case SMAXIN_1 :	L = 5; break;
        case CCASN :	L = 5; break;
        case CCASN_1 :	L = 5; break;
        case CCAIN :	L = 5; break;
        case CCAIN_1 :	L = 5; break;
        case UMOY1 :	L = 3; break;
        case UMOY2 :	L = 3; break;
        case UMOY3 :	L = 3; break;
        case STGE :	L = 8; break;
        case DPM1 :	L = 2; break;
        case FPM1 :	L = 2; break;
        case DPM2 :	L = 2; break;
        case FPM2 :	L = 2; break;
        case DPM3 :	L = 2; break;
        case FPM3 :	L = 2; break;
        case MSG1 :	L = 32; break;
        case MSG2 :	L = 16; break;
        case PRM :	L = 14; break;
        case RELAIS :	L = 3; break;
        case NTARF :	L = 2; break;
        case NJOURF :	L = 2; break;
        case NJOUR_1 :	L = 2; break;
        case PJOUR_1 :	L = 98; break;
        case PPOINTE :	L = 98; break;
        default : 		L = 0; break;
    }
    return L;
}

static bool horodatage(int index)
{
    bool r = false;
    switch(index)
    {
        case DATE :	r = true; break;
        case SMAXSN :	r = true; break;
        case SMAXSN1 :	r = true; break;
        case SMAXSN2 :	r = true; break;
        case SMAXSN3 :	r = true; break;
        case SMAXSN_1 :	r = true; break;
        case SMAXSN1_1 :	r = true; break;
        case SMAXSN2_1 :	r = true; break;
        case SMAXSN3_1 :	r = true; break;
        case SMAXIN :	r = true; break;
        case SMAXIN_1 :	r = true; break;
        case CCASN :	r = true; break;
        case CCASN_1 :	r = true; break;
        case CCAIN :	r = true; break;
        case CCAIN_1 :	r = true; break;
        case UMOY1 :	r = true; break;
        case UMOY2 :	r = true; break;
        case UMOY3 :	r = true; break;
        case DPM1 :	r = true; break;
        case FPM1 :	r = true; break;
        case DPM2 :	r = true; break;
        case FPM2 :	r = true; break;
        case DPM3 :	r = true; break;
        case FPM3 :	r = true; break;
    }
    return r;
}


class comboParameters : public QComboBox
{
    Q_OBJECT
public:
    comboParameters() {
        for (int n=0; n<TeleInfoValMax; n++) {
            addItem(TeleInfoValeurtoStr(n));
            setItemData(n, NumBinding(n), Qt::ToolTipRole); }
    }
};

enum modelValue
    {
        modeNormal = 0, modeRelative, modeOffset
    };

class comboMode : public QComboBox
{
    Q_OBJECT
public:
    comboMode() {
        addItem(tr("Normal"));
        addItem(tr("Relative"));
        addItem(tr("Offset"));
    }
};



enum IntervalValue
    {
        Constant = 0, t1mn, t2mn, t5mn, t10mn, t30mn, t1hour, t1day, t1week, t1month
    };

class comboInterval : public QComboBox
{
    Q_OBJECT
public:
    comboInterval() {
        addItem("Constant");
        addItem("1mn");
        addItem("2mn");
        addItem("5mn");
        addItem("10mn");
        addItem("30mn");
        addItem("1 hour");
        addItem("1 Day");
        addItem("1 Week");
        addItem("1 Month");
    }
};

enum NetTraffic
{
    Connecting, Waitingforanswer, Disconnected, Connected, Paused
};

#define NA "-32768"

struct teleInfoItem {
    QString RomID;
    int id = 0;
    comboParameters *ParameterList;
    comboMode *counterMode;
    comboInterval *interval;
    long int LastCounter = -1, Delta = 0;
    int lastMinute = -1, lastHour = -1, lastDay = -1, lastMonth = -1, lastWeek = -1, lastYear = -1;

    QTableWidgetItem *offset, *readValue, *value, *lastValue, *setValue;
    teleInfoItem(QString romid) : RomID(romid)
    {
        bool ok;
        id = RomID.right(5).left(3).toInt(&ok);
        ParameterList = new comboParameters;
        counterMode = new comboMode;
        interval = new comboInterval;
        offset = new QTableWidgetItem();
        readValue = new QTableWidgetItem();
        readValue->setFlags(readValue->flags() ^ Qt::ItemIsEditable);
        value = new QTableWidgetItem();
        value->setFlags(value->flags() ^ Qt::ItemIsEditable);
        lastValue = new QTableWidgetItem();
        lastValue->setFlags(lastValue->flags() ^ Qt::ItemIsEditable);
        setValue = new QTableWidgetItem();
        setValue->setFlags(setValue->flags() ^ Qt::ItemIsEditable);
        interval->setCurrentIndex(1);
    }
};


class tcpSocket : public QThread
{
    Q_OBJECT
public:
QString ip, command, log;
QString Buffer;
quint16 port = 1470;
QAbstractSocket::SocketState socketState = QAbstractSocket::UnconnectedState;
QTcpSocket *socket;
bool endLessLoop = false;
    tcpSocket() {
        qRegisterMetaType<QAbstractSocket::SocketState>();  }
    void run()
    {
    //    qDebug() << "Start";
    QTcpSocket Socket;
    socket = &Socket;
    Socket.moveToThread(this);
    qintptr sd = Socket.socketDescriptor();
#ifdef Q_OS_LINUX
    int set = 1;
    setsockopt(int(sd), SOL_SOCKET, MSG_NOSIGNAL, (const char *)&set, sizeof(int));
#endif
    connect(&Socket, SIGNAL(readyRead()), this, SLOT(readData()), Qt::QueuedConnection);
    //connect(&Socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(socket, &QAbstractSocket::stateChanged, [this]() { int state = socket->state(); emit(tcpStatusChange(state)); });
    //QObject::connect(socket, &QIODevice::readyRead, [this](){
    while (endLessLoop) {
        if (Socket.state() == QAbstractSocket::ConnectedState) {
            Socket.disconnectFromHost();
            Socket.waitForDisconnected(); }
        if (endLessLoop) {
            Socket.connectToHost(ip, port);
            Socket.waitForConnected();
        while ((Socket.state() == QAbstractSocket::ConnectedState) && endLessLoop) {
            Socket.waitForReadyRead();
            sleep(1);
        } }
    }
    //qDebug() << "endLessLoop finished";
    Socket.disconnectFromHost();
    if (Socket.state() != QAbstractSocket::UnconnectedState) Socket.waitForDisconnected();
    socketState = Socket.state(); emit(tcpStatusChange(socketState));
    log.clear();
    //qDebug() << "Stop";
    }
private slots:
    void readData() {
        QString extract = "";
        int chdeb, chfin, L;
        if (socket) Buffer += socket->readAll();
        chdeb = Buffer.indexOf(STX);
        chfin = Buffer.indexOf(ETX);
        L = Buffer.length();
        if (chdeb > chfin) { Buffer = Buffer.right(L - chdeb); }
        else if ((chdeb != -1) and (chfin != -1)) {
            extract = Buffer.mid(chdeb + 1, chfin - chdeb - 1);
            emit(read(extract));
            Buffer.clear(); }
    }
    /*void socketStateChanged(QAbstractSocket::SocketState state)
    {
        socketState = state;
        if (state  == QAbstractSocket::UnconnectedState) qDebug() << "UnconnectedState";
        if (state  == QAbstractSocket::HostLookupState) qDebug() << "HostLookupState";
        if (state  == QAbstractSocket::ConnectingState) qDebug() << "ConnectingState";
        if (state  == QAbstractSocket::ConnectedState) qDebug() << "ConnectedState";
        if (state  == QAbstractSocket::BoundState) qDebug() << "BoundState";
        if (state  == QAbstractSocket::ListeningState) qDebug() << "ListeningState";
        if (state  == QAbstractSocket::ClosingState) qDebug() << "ClosingState";
        emit(tcpStatusChange());
    }*/
signals:
    void tcpStatusChange(int);
    void read(QString);
};

class TeleInfoPlugin : public QWidget, LogisDomInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "logisdom.network.LogisDomInterface/1.0" FILE "TeleInfoPlugin.json")
    Q_INTERFACES(LogisDomInterface)
public:
    TeleInfoPlugin( QWidget *parent = nullptr );
    ~TeleInfoPlugin() override;
    QObject* getObject() override;
   QWidget *widgetUi() override;
    QWidget *getDevWidget(QString RomID) override;
    void setConfigFileName(const QString) override;
    void readDevice(const QString &device) override;
    QString getDeviceCommands(const QString &device) override;
    void saveConfig() override;
    void readConfig() override;
    void setLockedState(bool) override;
    QString getDeviceConfig(QString) override;
    void setDeviceConfig(const QString &, const QString &) override;
    QString getName() override;
    void setStatus(const QString) override;
    bool acceptCommand(const QString) override;
    bool isDimmable(const QString) override;
    bool isManual(const QString) override;
    double getMaxValue(const QString) override;
signals:
    void newDeviceValue(QString, QString) override;
    void newDevice(LogisDomInterface*, QString) override;
    void deviceSelected(QString) override;
    void updateInterfaceName(LogisDomInterface*, QString) override;
    void connectionStatus(QAbstractSocket::SocketState) override;
private:
    QWidget *ui;
    QWidget *devUi;
    bool OnOff = true;
    QString configFileName;
    QIcon tcpIconUnconnectedState, tcpIconHostLookupState, tcpIconConnectingState, tcpIconConnectedState, tcpIconClosingState;
    void saveSetup();
    void loadSetup();
    void setipaddress(const QString &adr);
    void setport(const QString &adr);
    QString ip2Hex(const QString &ip);
    Ui::TeleInfoUI *mui;
    QString lastStatus;
    void log(const QString);
    QString logStr;
    tcpSocket socketThread;
    QTcpSocket *socket;
    bool isIpValid(QString str);
    bool isPortValid(QString str);
    QString buildRomID(int n);
    QList<teleInfoItem*> teleInfoDevices;
    teleInfoItem *addDevice(QString id = "");
    QString getParameters(QString, const QString &);
    QString translateMainValue(teleInfoItem *, QString &);
private slots:
    void logEnable(int);
    void IPEdited(QString);
    void PortEdited(QString);
    void tcpStatusChange(int);
    void Start();
    void Stop();
    void read(QString);
    void showLog();
    void on_editName_editingFinished();
    void on_AddButton_clicked();
    void on_RemoveButton_clicked();
    void displayDevice(int, int);
};


#endif

