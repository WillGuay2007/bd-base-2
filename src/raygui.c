/**
 * Si jamais vous ajouter raygui dans d'autre fichier n'ajouter
 * pas le #define RAYGUI_IMPLEMENTATION, on doit le retrouver seulement dans un fichier.
 * On le met dans un fichier separer parce que sinon, à chaque changement du fichier entrypoint on doit recompiler raygui au complet.
 * Donc, ca rend la compilation plus longue.
 */
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"