#define _CRT_SECURE_NO_WARNINGS
#include "Dungeon_Adventures.h"

// --- LECTURA VALIDADA: solo acepta 1 o 2 ---
int leerDecision(void) {
    int d = 0;
    while (scanf("%d", &d) != 1 || (d != 1 && d != 2)) {
        while (getchar() != '\n');
        printf("  Opcion invalida. Elige 1 o 2: ");
    }
    return d;
}

// --- ANIMACIONES SIMPLES CON RETRASO ---
void animacionCaminando(void) {
    printf("\nAvanzando por los pasillos oscuros");
    for (int i = 0; i < 3; i++) {
        sleep_ms(400);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

void animacionCombate(void) {
    printf("\nPreparando armas y escudos");
    for (int i = 0; i < 3; i++) {
        sleep_ms(300);
        printf(" !");
        fflush(stdout);
    }
    printf("\n");
}

// --- JUGADOR ---
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

// --- LISTA ENLAZADA: INVENTARIO ---
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

// --- PILA: HISTORIAL ---
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

// --- COLA: EVENTOS ---
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

// --- LIBERACIÓN DE MEMORIA ---
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