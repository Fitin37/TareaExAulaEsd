#define _CRT_SECURE_NO_WARNINGS
#include "Dungeon_Adventures.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    struct Jugador heroe;
    Inventario miInventario = NULL;
    PilaHistorial historial = NULL;
    ColaEventos colaCalabozo;

    inicializarCola(&colaCalabozo);
    inicializarJugador(&heroe);

    encolarEvento(&colaCalabozo, "Cofre de Oro");
    encolarEvento(&colaCalabozo, "Trampa de Monstruo");
    encolarEvento(&colaCalabozo, "Fuente Curativa");
    encolarEvento(&colaCalabozo, "Jefe del Calabozo");

    pushHistorial(&historial, "El heroe ha entrado al calabozo de datos.");

    int opcion = 0;
    do {
        printf("\n========================================\n");
        printf("          DUNGEON ADVENTURES     \n");
        printf("========================================\n");
        printf("1. Explorar siguiente evento \n");
        printf("2. Ver estado y mochila \n");
        printf("3. Ver historial de acciones \n");
        printf("4. Salir de la aventura\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (opcion) {
        case 1: {
            if (colaVacia(colaCalabozo)) {
                printf("\n ¡Has superado todos los eventos del calabozo!\n");
                break;
            }

            animacionCaminando();

            char* eventoActual = desencolarEvento(&colaCalabozo);
            if (eventoActual != NULL) {
                printf("\n[Alerta] Te encuentras frente a: %s\n", eventoActual);

                char buffer[100];
                int decision = 0;

                if (strcmp(eventoActual, "Cofre de Oro") == 0) {
                    printf("  1. Forzar la cerradura rapidamente\n");
                    printf("  2. Inspeccionar en busca de trampas\n");
                    printf("  Elige tu accion: ");
                    decision = leerDecision();

                    if (decision == 1) {
                        heroe.oro += 20;
                        sprintf(buffer, "Forzaste el cofre: +20 oro (algo se rompio).");
                    }
                    else {
                        heroe.oro += 35;
                        agregarItemInventario(&miInventario, "Gema Brillante");
                        sprintf(buffer, "Inspeccionaste seguro: +35 oro y Gema.");
                    }
                    printf(" %s\n", buffer);
                    pushHistorial(&historial, buffer);
                }
                else if (strcmp(eventoActual, "Trampa de Monstruo") == 0) {
                    animacionCombate();
                    printf("  1. Intentar huir\n");
                    printf("  2. Defender con el cuerpo\n");
                    printf("  Elige tu accion: ");
                    decision = leerDecision();

                    if (decision == 1) {
                        heroe.vida -= 20;
                        sprintf(buffer, "Huiste cobardemente perdiendo 20 HP.");
                    }
                    else {
                        heroe.vida -= 40;
                        sprintf(buffer, "Recibiste el impacto directo: -40 HP.");
                    }
                    printf(" %s\n", buffer);
                    pushHistorial(&historial, buffer);
                }
                else if (strcmp(eventoActual, "Fuente Curativa") == 0) {
                    printf("  1. Beber un trago moderado\n");
                    printf("  2. Beber hasta saciarse (cuesta 5 de oro)\n");
                    printf("  Elige tu accion: ");
                    decision = leerDecision();

                    if (decision == 1) {
                        heroe.vida += 15;
                        sprintf(buffer, "Bebiste un trago moderado: +15 HP.");
                    }
                    else {
                        heroe.vida += 30;
                        heroe.oro -= 5;
                        if (heroe.oro < 0) heroe.oro = 0;
                        sprintf(buffer, "Bebiste hasta saciarte: +30 HP, -5 oro.");
                    }
                    if (heroe.vida > 100) heroe.vida = 100;
                    printf(" %s\n", buffer);
                    pushHistorial(&historial, buffer);
                }
                else if (strcmp(eventoActual, "Jefe del Calabozo") == 0) {
                    animacionCombate();

                    printf("\n¡Es el combate final contra el Guardian de Datos!\n");
                    printf("Tu vida actual: %d HP\n", heroe.vida);

                    printf("\n1. Atacar directamente (muy arriesgado)\n");
                    printf("2. Defenderse del ataque\n");
                    printf("Elige tu accion: ");
                    decision = leerDecision();

                    if (decision == 1) {
                        heroe.vida -= 80;
                        sprintf(buffer, "Atacaste directamente: recibiste 80 de dano.");
                    }
                    else {
                        heroe.vida -= 50;
                        sprintf(buffer, "Te defendiste del jefe: recibiste 50 de dano.");
                    }

                    printf("\n%s\n", buffer);
                    pushHistorial(&historial, buffer);

                    if (heroe.vida <= 0) {
                        printf("\n¡El Guardian de Datos te ha derrotado!\n");
                        printf("No conseguiste superar el calabozo.\n");
                        opcion = 4;
                    }
                    else {
                        printf("\n¡Has derrotado al Guardian de Datos!\n");

                        heroe.oro += 100;
                        agregarItemInventario(&miInventario, "Amuleto Legendario");

                        sprintf(buffer, "Recompensa: +100 oro y Amuleto Legendario.");
                        printf("%s\n", buffer);
                        pushHistorial(&historial, buffer);
                    }
                }

                free(eventoActual);
            }

            // Muerte por otros eventos (el jefe ya muestra su propio mensaje)
            if (heroe.vida <= 0 && opcion != 4) {
                printf("\n Tus puntos de vida llegaron a 0. Has muerto en el calabozo...\n");
                opcion = 4;
            }
            break;
        }
        case 2:
            mostrarEstado(heroe, miInventario);
            break;
        case 3:
            mostrarHistorial(historial);
            break;
        case 4:
            printf("\nSaliendo del juego...\n");
            break;
        default:
            printf("\n[Error] Opcion no valida.\n");
        }
    } while (opcion != 4);

    liberarMemoria(&miInventario, &historial, &colaCalabozo);
    return 0;
}