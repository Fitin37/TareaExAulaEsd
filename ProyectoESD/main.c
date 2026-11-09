#include "Dungeon_Adventures.h"

int main() {
    srand((unsigned int)time(NULL));

    struct Jugador heroe;
    Inventario miInventario = NULL;
    PilaHistorial historial = NULL;
    ColaEventos colaCalabozo;

    inicializarCola(&colaCalabozo);
    inicializarJugador(&heroe);

    encolarEvento(&colaCalabozo, "Cofre de Oro");
    encolarEvento(&colaCalabozo, "Trampa de Monstruo");
    encolarEvento(&colaCalabozo, "Fuente Curativa");
    encolarEvento(&colaCalabozo, "Dungeon Boss");

    pushHistorial(&historial, "Has entrado al calabozo de datos.");

    int opcion = 0;
    int juegoTerminado = 0;

    do {
        printf("\n========================================\n");
        printf("         DUNGEON ADVENTURES       \n");
        printf("========================================\n");
        printf("1. Explorar evento \n");
        printf("2. Ver tu loot \n");
        printf("3. Ver historial de acciones \n");
        printf("4. Abandonar el calabozo\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (opcion) {
        case 1:
            procesarSiguienteEvento(&heroe, &miInventario, &historial, &colaCalabozo, &juegoTerminado);
            
            if (heroe.vida <= 0 || juegoTerminado) {
                opcion = 4; 
            }
            break;
        case 2:
            mostrarEstado(heroe, miInventario);
            break;
        case 3:
            mostrarHistorial(historial);
            break;
        case 4:
            if (!juegoTerminado && heroe.vida > 0) {
                printf("\nSaliendo del juego...\n");
            }
            break;
        default:
            printf("\nOpcion no valida.\n");
        }
    } while (opcion != 4);

    if (heroe.vida <= 0) {
        printf("\n==========================================\n");
        printf("      ¡Ti muriste! Vuelve a intentarlo\n");
        printf("============================================\n");
    } else if (juegoTerminado) {
        printf("\n====================================================\n");
        printf("     ¡Aura total, disfruta a la aldeana premium!\n");
        printf("======================================================\n");
    }

    printf("\nDele al ENTER, se merece un descanso.");
    getchar(); 

    liberarMemoria(&miInventario, &historial, &colaCalabozo);
    return 0;
}
