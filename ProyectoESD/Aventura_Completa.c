#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

// --- ESTRUCTURAS PRINCIPALES ---
struct Jugador {
    char nombre[30];
    int vida;
    int oro;
};

struct NodoInventario {
    char nombreItem[30];
    struct NodoInventario* siguiente;
};

struct NodoHistorial {
    char descripcion[80];
    struct NodoHistorial* siguiente;
};

struct NodoColaEvento {
    char tipoEvento[30];
    struct NodoColaEvento* siguiente;
};

typedef struct NodoInventario* Inventario;
typedef struct NodoHistorial* PilaHistorial;

typedef struct {
    struct NodoColaEvento* frente;
    struct NodoColaEvento* final;
} ColaEventos;

// Prototipos
void inicializarJugador(struct Jugador* j);
void mostrarEstado(struct Jugador j, Inventario inv);
void agregarItemInventario(Inventario* inv, const char* item);
void mostrarInventario(Inventario inv);
void pushHistorial(PilaHistorial* pila, const char* texto);
void mostrarHistorial(PilaHistorial pila);
void inicializarCola(ColaEventos* c);
void encolarEvento(ColaEventos* c, const char* tipo);
char* desencolarEvento(ColaEventos* c);
int colaVacia(ColaEventos c);
void liberarMemoria(Inventario* inv, PilaHistorial* pila, ColaEventos* c);
void animacionCaminando();
void animacionCombate();

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
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
    encolarEvento(&colaCalabozo, "Jefe del Calabozo");

    pushHistorial(&historial, "El heroe ha entrado a la calabozo de datos.");

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
                printf("\n ¡Has superado todos los eventos de la calabozo!\n");
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
                    while (scanf("%d", &decision) != 1 || (decision != 1 && decision != 2)) {
                        while (getchar() != '\n');
                        printf("  Opcion invalida. Elige 1 o 2: ");
                    }

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
                    while (scanf("%d", &decision) != 1 || (decision != 1 && decision != 2)) {
                        while (getchar() != '\n');
                        printf("  Opcion invalida. Elige 1 o 2: ");
                    }

                    if (decision == 1) {
                        heroe.vida -= 15;
                        sprintf(buffer, "Huiste cobardemente perdiendo 15 HP.");
                    }
                    else {
                        heroe.vida -= 25;
                        sprintf(buffer, "Recibiste el impacto directo: -25 HP.");
                    }
                    printf(" %s\n", buffer);
                    pushHistorial(&historial, buffer);
                }
                else if (strcmp(eventoActual, "Fuente Curativa") == 0) {
                    printf("  1. Beber un trago moderado\n");
                    printf("  2. Beber hasta saciarse\n");
                    printf("  Elige tu accion: ");
                    scanf("%d", &decision);

                    if (decision == 1) {
                        heroe.vida += 10;
                        sprintf(buffer, "Bebiste un trago moderado: +10 HP.");
                    }
                    else {
                        heroe.vida += 15;
                        heroe.oro -= 5;
                        sprintf(buffer, "Bebiste hasta saciarte: +15HP, -5 oro.");
                    }
                    if (heroe.vida > 100) heroe.vida = 100;
                    printf(" %s\n", buffer);
                    pushHistorial(&historial, buffer);
                }
                else if (strcmp(eventoActual, "Jefe del Calabozo") == 0) {
                    animacionCombate();

                    printf("\n¡Es el combate final contra el Guardian de Datos!\n");
                    printf("Tu vida actual: %d HP\n", heroe.vida);

                    printf("\n1. Atacar directamente\n");
                    printf("2. Defenderse del ataque\n");
                    printf("Elige tu accion: ");

                    while (scanf("%d", &decision) != 1 ||
                        (decision != 1 && decision != 2)) {
                        while (getchar() != '\n');
                        printf("Opcion invalida. Elige 1 o 2: ");
                    }

                    if (decision == 1) {
                        heroe.vida -= 85;
                        sprintf(buffer, "Atacaste directamente: recibiste 85 de dano.");
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

            if (heroe.vida <= 0) {
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

// --- ANIMACIONES SIMPLES CON RETRASO ---
void animacionCaminando() {
    printf("\nAvanzando por los pasillos oscuros");
    for (int i = 0; i < 3; i++) {
        sleep_ms(400);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

void animacionCombate() {
    printf("\nPreparando armas y escudos");
    for (int i = 0; i < 3; i++) {
        sleep_ms(300);
        printf(" !");
        fflush(stdout);
    }
    printf("\n");
}

// --- FUNCIONES DE APOYO ---
void inicializarJugador(struct Jugador* j) {
    printf("\nIngrese el nombre del aventurero: ");
    if (scanf("%29s", j->nombre) != 1) {
        strcpy(j->nombre, "Heroe");
    }
    j->vida = 100;
    j->oro = 10;
    printf("¡Personaje creado con exito! 100 HP iniciales.\n");
}

void mostrarEstado(struct Jugador j, Inventario inv) {
    printf("\n--- ESTADO DEL HEROE ---\n");
    printf(" Nombre: %s\n", j.nombre);
    printf(" Vida (HP): %d / 100\n", j.vida);
    printf(" Oro: %d monedas\n", j.oro);
    mostrarInventario(inv);
    printf("-------------------------\n");
}

void agregarItemInventario(Inventario* inv, const char* item) {
    Inventario nuevo = (Inventario)malloc(sizeof(struct NodoInventario));
    if (nuevo != NULL) {
        strcpy(nuevo->nombreItem, item);
        nuevo->siguiente = *inv;
        *inv = nuevo;
    }
}

void mostrarInventario(Inventario inv) {
    printf(" Inventario (Lista Enlazada):\n");
    Inventario actual = inv;
    if (actual == NULL) {
        printf("   (Mochila vacia)\n");
        return;
    }
    while (actual != NULL) {
        printf("   - %s\n", actual->nombreItem);
        actual = actual->siguiente;
    }
}

void pushHistorial(PilaHistorial* pila, const char* texto) {
    PilaHistorial nuevo = (PilaHistorial)malloc(sizeof(struct NodoHistorial));
    if (nuevo != NULL) {
        strcpy(nuevo->descripcion, texto);
        nuevo->siguiente = *pila;
        *pila = nuevo;
    }
}

void mostrarHistorial(PilaHistorial pila) {
    printf("\n--- HISTORIAL RECIENTE ---\n");
    PilaHistorial actual = pila;
    if (actual == NULL) {
        printf("El historial esta vacio.\n");
        return;
    }
    while (actual != NULL) {
        printf(" > %s\n", actual->descripcion);
        actual = actual->siguiente;
    }
    printf("--------------------------------------\n");
}

void inicializarCola(ColaEventos* c) {
    c->frente = NULL;
    c->final = NULL;
}

int colaVacia(ColaEventos c) {
    return c.frente == NULL;
}

void encolarEvento(ColaEventos* c, const char* tipo) {
    struct NodoColaEvento* nuevo = (struct NodoColaEvento*)malloc(sizeof(struct NodoColaEvento));
    if (nuevo != NULL) {
        strcpy(nuevo->tipoEvento, tipo);
        nuevo->siguiente = NULL;
        if (c->final == NULL) {
            c->frente = nuevo;
        }
        else {
            c->final->siguiente = nuevo;
        }
        c->final = nuevo;
    }
}

char* desencolarEvento(ColaEventos* c) {
    if (colaVacia(*c)) return NULL;
    struct NodoColaEvento* temp = c->frente;
    char* tipo = (char*)malloc(30);
    if (tipo != NULL) {
        strcpy(tipo, temp->tipoEvento);
    }
    c->frente = c->frente->siguiente;
    if (c->frente == NULL) {
        c->final = NULL;
    }
    free(temp);
    return tipo;
}

void liberarMemoria(Inventario* inv, PilaHistorial* pila, ColaEventos* c) {
    if (inv != NULL) {
        while (*inv != NULL) {
            Inventario tempInv = *inv;
            *inv = (*inv)->siguiente;
            free(tempInv);
        }
    }
    if (pila != NULL) {
        while (*pila != NULL) {
            PilaHistorial tempP = *pila;
            *pila = (*pila)->siguiente;
            free(tempP);
        }
    }
    if (c != NULL) {
        while (!colaVacia(*c)) {
            char* t = desencolarEvento(c);
            if (t != NULL) free(t);
        }
    }
}
