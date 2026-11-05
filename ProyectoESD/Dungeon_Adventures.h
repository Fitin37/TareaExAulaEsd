#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// --- PROTOTIPOS ---
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
void animacionCaminando(void);
void animacionCombate(void);
int leerDecision(void);

#endif#pragma once
