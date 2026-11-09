#include "Dungeon_Adventures.h"

void animacionCaminando() {
    printf("\nMira por donde caminas pero fijate mejor con quién te rodeas");
    for (int i = 0; i < 3; i++) {
        sleep_ms(400);
        printf(".");
        fflush(stdout);
    }
    printf("\n");
}

void animacionCombate() {
    printf("\nPoniendote macizo, preparando armas y escudos");
    for (int i = 0; i < 3; i++) {
        sleep_ms(300);
        printf(" !");
        fflush(stdout);
    }
    printf("\n");
}

void procesarSiguienteEvento(struct Jugador* heroe, Inventario* inv, PilaHistorial* pila, ColaEventos* c, int* juegoTerminado) {
    if (colaVacia(*c)) {
        printf("\n ¡Rusheaste el juego, ya no tienes nada que hacer aquí, eres un crack!\n");
        *juegoTerminado = 1;
        return;
    }

    animacionCaminando();

    char* eventoActual = desencolarEvento(c);
    if (eventoActual != NULL) {
        printf("\n¡BUSO! te encuentras frente a: %s\n", eventoActual);

        char buffer[150];
        int decision = 0;

        if (strcmp(eventoActual, "Cofre de Oro") == 0) {
            printf("  1. Forzar la cerradura\n");
            printf("  2. Inspeccionar la zona en busca de trampas\n");
            printf("  Elige tu accion: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->oro += 20;
                sprintf(buffer, "Forzaste el cofre: +20 oro.");
            } else {
                heroe->oro += 35;
                agregarItemInventario(inv, "Gema Brillante");
                sprintf(buffer, "Inspeccionaste seguro: +35 oro y Gema.");
            }
            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }
        else if (strcmp(eventoActual, "Trampa de Monstruo") == 0) {
            animacionCombate();
            printf("  1. Intentar huir\n");
            printf("  2. Darte duro\n");
            printf("  Elige tu accion: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida -= 20;
                sprintf(buffer, "Huiste cobardemente, perdiste 20 HP.");
            } else {
                heroe->vida -= 35;
                sprintf(buffer, "Ahhhg!! eso dolió -35 HP.");
            }
            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }
        else if (strcmp(eventoActual, "Fuente Curativa") == 0) {
            printf("  1. Beber un trago moderado\n");
            printf("  2. Beber sin pena\n");
            printf("  Elige tu accion: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida += 10;
                sprintf(buffer, "Fuiste precavido y recuperastes +10 HP.");
            } else {
                heroe->vida += 30;
                sprintf(buffer, "El agua es el próximo petróleo, la disfrutaste como nunca y recuperaste +30 HP.");
            }

            if (heroe->vida > 100) heroe->vida = 100;
            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }
        else if (strcmp(eventoActual, "Dungeon Boss") == 0) {
            animacionCombate();
            printf("  ¡Para esto naciste, estás cara a cara con el mero mero de este calabozo!\n");
            printf("  1. Darte duro con el mero mero\n");
            printf("  2. Huir como cobarde frente a la aldeana premium\n");
            printf("  Elige tu accion: ");
            scanf("%d", &decision);
            while (getchar() != '\n');

            if (decision == 1) {
                heroe->vida -= 75; 
                
                if (heroe->vida > 0) {
                    heroe->oro += 100;
                    agregarItemInventario(inv, "Aldeana premium");
                    sprintf(buffer, "¡Era más duro pasar POO que vencer a este tipo!, Farmeaste 100,000,000HP y la aldeana premium quiere estar contigo.");
                    *juegoTerminado = 1;
                } else {
                    sprintf(buffer, "Te metieron una paliza histórica y enfrente de la aldeana premium, ¡La decepcionaste!");
                }
            } else {
                heroe->vida -= 100; 
                sprintf(buffer, "Intentaste huir del jefe, te alcanzó por la espalda y te humilló frente a la aldeana premium.");
            }
            
            printf(" %s\n", buffer);
            pushHistorial(pila, buffer);
        }

        free(eventoActual);
    }

    if (heroe->vida <= 0) {
        printf("\n Bajaste a 0HP. Has muerto en el calabozo...\n");
    }
}

void inicializarJugador(struct Jugador* j) {
    printf("\nIngrese el nombre del aventurero: ");
    if (scanf("%29s", j->nombre) != 1) {
        strcpy(j->nombre, "Heroe");
    }
    while (getchar() != '\n');
    j->vida = 100;
    j->oro = 10;
    printf("¡Eso es bello, vamos a la aventura! 100 HP iniciales.\n");
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
    printf("Este es tu loot:\n");
    Inventario actual = inv;
    if (actual == NULL) {
        printf(" Farméa más cosas héroe, está más vacío que ese exámen de cálculo... \n");
        return;
    }
    while (actual != NULL) {
        printf("    - %s\n", actual->nombreItem);
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
        printf("El historial esta vacio, necesitamos que te atrevas a aventurear.\n");
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
        } else {
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
