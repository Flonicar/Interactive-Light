// WLED UserMod Setup - Diese Datei in wled00/usermods/ kopieren
#pragma once

#include "wled.h"
#include "usermod_dfplayer.h"
#include "usermod_buttons.h"

// Globale Instanzen erstellen
UsermodDFPlayer* dfplayerMod = new UsermodDFPlayer();
UsermodButtons* buttonMod = new UsermodButtons();

// UserMods registrieren
void registerUsermods() {
    usermods.add(dfplayerMod);
    usermods.add(buttonMod);
}
