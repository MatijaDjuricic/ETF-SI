#include <stdio.h>
#include <stdlib.h>
#define N_SPIL 52
#define N_KARATA 13
#define BBS_S2 6
#define BBS_P 43
#define BBS_Q 47
char *KARTE[] = {
    "-", "A", "2", "3", "4", "5", "6",
    "7", "8", "9", "10", "J", "Q", "K"
};
typedef struct Lista {
    void *info;
    struct Lista *sledeci;
} Lista;
typedef struct {
    Lista *lista;
} Stek;
int BBS(int *x0) {
    int n = 0;
    for (int i = 0; i < BBS_S2; i++) {
        *x0 = (*x0 * *x0) % (BBS_P * BBS_Q);
        int j = 0;
        int t = *x0;
        while (t) {
            if (t & 1) j++;
            t >>= 1;
        }
        n = (n << 1) | (j % 2);
    }
    return n;
}
void dodaj_na_pocetak(Lista **pocetak, void *element) {
    Lista *t = malloc(sizeof(Lista));
    t->info = element;
    t->sledeci = *pocetak;
    *pocetak = t;
}
void dodaj_na_kraj(Lista **pocetak, void *element) {
    Lista *t = malloc(sizeof(Lista));
    t->info = element;
    t->sledeci = NULL;
    if (*pocetak == NULL) {
        *pocetak = t;
        return;
    }
    Lista *trenutni = *pocetak;
    while (trenutni->sledeci != NULL) {
        trenutni = trenutni->sledeci;
    }
    trenutni->sledeci = t;
}
void dodaj_na_poziciju(Lista **pocetak, void *element, int pozicija) {
    Lista *t = malloc(sizeof(Lista));
    t->info = element;
    if (pozicija == 0) {
        t->sledeci = *pocetak;
        *pocetak = t;
        return;
    }
    Lista *trenutni = *pocetak;
    for (int i = 0; i < pozicija - 1; i++) {
        if (trenutni == NULL) {
            free(t);
            return;
        }
        trenutni = trenutni->sledeci;
    }
    if (trenutni == NULL) {
        free(t);
        return;
    }
    t->sledeci = trenutni->sledeci;
    trenutni->sledeci = t;
}
void *ukloni_sa_pocetka(Lista **pocetak) {
    if (*pocetak == NULL) return NULL;
    void *v = (*pocetak)->info;
    Lista *t = *pocetak;
    *pocetak = (*pocetak)->sledeci;
    free(t);
    return v;
}
void *ukloni_sa_kraja(Lista **pocetak) {
    Lista *prethodni = NULL, *trenutni = *pocetak;
    void *v;
    if (trenutni == NULL) return NULL;
    if (trenutni->sledeci == NULL) {
        v = trenutni->info;
        free(trenutni);
        *pocetak = NULL;
        return v;
    }
    while (trenutni->sledeci != NULL) {
        prethodni = trenutni;
        trenutni = trenutni->sledeci;
    }
    v = trenutni->info;
    prethodni->sledeci = NULL;
    free(trenutni);
    return v;
}
void *ukloni_sa_pozicije(Lista **pocetak, int pozicija) {
    if (*pocetak == NULL || pozicija < 0) return NULL;
    Lista *prethodni = NULL, *trenutni = *pocetak;
    void *v;
    if (trenutni == NULL) return NULL;
    if (pozicija == 0) {
        v = trenutni->info;
        *pocetak = trenutni->sledeci;
        free(trenutni);
        return v;
    }
    for (int i = 0; i < pozicija; i++) {
        prethodni = trenutni;
        trenutni = trenutni->sledeci;
        if (trenutni == NULL) return NULL;
    }
    v = trenutni->info;
    prethodni->sledeci = trenutni->sledeci;
    free(trenutni);
    return v;
}
void *pogledaj_pocetak(Lista *pocetak) {
    if (pocetak == NULL) return NULL;
    return pocetak->info;
}
void *pogledaj_kraj(Lista *pocetak) {
    if (pocetak == NULL) return NULL;
    Lista *trenutni = pocetak;
    while (trenutni->sledeci != NULL) {
        trenutni = trenutni->sledeci;
    }
    return trenutni->info;
}
int je_prazna(Lista *pocetak) {
    return pocetak == NULL;
}
int velicina(Lista *pocetak) {
    int duzina = 0;
    Lista *trenutni = pocetak;
    if (trenutni == NULL) return 0;
    while (trenutni != NULL) {
        duzina++;
        trenutni = trenutni->sledeci;
    }
    return duzina;
}
void push(Stek *stek, void *element) {
    dodaj_na_pocetak(&stek->lista, element);
}
void *pop(Stek *stek) {
    if (je_prazna(stek->lista)) return NULL;
    return ukloni_sa_pocetka(&stek->lista);
}
void *pogledaj_vrh_steka(Stek *stek) {
    if (je_prazna(stek->lista)) return NULL;
    return pogledaj_pocetak(stek->lista);
}
int je_prazan(Stek *stek) {
    return je_prazna(stek->lista);
}
char *format_karta(int karta) {
    if (karta < 0 || karta > 13) return "-";
    return KARTE[karta];
}
void prikazi_glavni_meni() {
    printf("===== KARTASKA IGRA =====\n");
    printf("1. Promesaj spil i podeli karte\n");
    printf("2. Potez igraca 1\n");
    printf("3. Potez igraca 2\n");
    printf("4. Prikazi trenutno stanje\n");
    printf("0. Izlaz\n");
}
void prikazi_potez_igraca_meni(int igrac) {
    printf("===== POTEZ IGRACA %d =====\n", igrac);
    printf("1. Vuci kartu sa zatvorenog spila\n");
    printf("2. Vuci kartu sa otvorenog spila\n");
    printf("3. Postavi izvucenu kartu na sredinu\n");
    printf("4. Postavi izvucenu kartu kod protivnika\n");
    printf("5. Postavi kartu na svoju gomilu i zavrsi potez\n");
    printf("6. Prikazi trenutno stanje\n");
    printf("0. Prikazi glavni meni(izlaz)\n");
}
void prikazi_trenutno_stanje(int n_zatvoreni1, int izvucena1, int n_otvoreni1,
    int otvorena1, int sredina, int n_sredina, int otvorena2,
    int n_otvoreni2, int izvucena2, int n_zatvoreni2
) {
    printf("===== SIMULACIJA IGRE KARATA\n");
    printf("===== TRENUTNO STANJE\n");
    printf("===== Igrac(1) - Igrac(2)\n");
    printf("zatoreni  izvucena  otvoreni    sredina   otvoreni  izvucena  zatvoreni\n");
    printf("+-------+ +-------+ +-------+  +-------+  +-------+ +-------+ +-------+\n");
    printf("|=======| |%s======| |%s======|  |%s======|  |%s======| |%s======| |=======|\n",
        format_karta(izvucena1), format_karta(otvorena1), format_karta(sredina), format_karta(otvorena2), format_karta(izvucena2)
    );
    printf("|===-===| |===%s===| |===%s===|  |===%s===|  |===%s===| |===%s===| |===-===|\n",
        format_karta(izvucena1), format_karta(otvorena1), format_karta(sredina), format_karta(otvorena2), format_karta(izvucena2)
    );
    printf("|=======| |======%s| |======%s|  |======%s|  |======%s| |======%s| |=======|\n",
        format_karta(izvucena1), format_karta(otvorena1), format_karta(sredina), format_karta(otvorena2), format_karta(izvucena2)
    );
    printf("+-------+ +-------+ +-------+  +-------+  +-------+ +-------+ +-------+\n");
    printf("+--%d--+-------------+---%d---+  +---%d---+  +---%d---+-------------+--%d--+\n",
        n_zatvoreni1, n_otvoreni1, n_sredina, n_otvoreni2, n_zatvoreni2
    );
}
void ocisti_talon(Lista **zatvoreni_spil1, int *izvucena_karta1, Stek *otvoreni_spil1,
    Stek *sredina_spil, Stek *otvoreni_spil2, int *izvucena_karta2, Lista **zatvoreni_spil2
) {
    *izvucena_karta1 = 0;
    *izvucena_karta2 = 0;
    while (!je_prazna(*zatvoreni_spil1)) {
        int *x = ukloni_sa_pocetka(zatvoreni_spil1);
        free(x);
    }
    while (!je_prazna(*zatvoreni_spil2)) {
        int *x = ukloni_sa_pocetka(zatvoreni_spil2);
        free(x);
    }
    while (!je_prazan(sredina_spil)) {
        int *x = pop(sredina_spil);
        free(x);
    }
    while (!je_prazan(otvoreni_spil1)) {
        int *x = pop(otvoreni_spil1);
        free(x);
    }
    while (!je_prazan(otvoreni_spil2)) {
        int *x = pop(otvoreni_spil2);
        free(x);
    }
}
void promesaj_podeli_karte(int *x0, Lista **zatvoreni_spil1, Lista **zatvoreni_spil2) {
    int i, spil[N_SPIL];
    for (i = 0; i < N_SPIL; i++) {
        spil[i] = i % N_KARATA + 1;
    }
    for (i = N_SPIL - 1; i > 0; i--) {
        int j = BBS(x0) % (i + 1);
        int t = spil[i];
        spil[i] = spil[j];
        spil[j] = t;
    }
    for (i = 0; i < N_SPIL; i+=2) {
        if (i + 1 >= N_SPIL) break;
        int *k1 = malloc(sizeof(int));
        int *k2 = malloc(sizeof(int));
        *k1 = spil[i];
        *k2 = spil[i + 1];
        dodaj_na_kraj(zatvoreni_spil1, k1);
        dodaj_na_kraj(zatvoreni_spil2, k2);
    }
}
void okreni_otvoreni_spil(Stek *otvoreni_spil, Lista **zatvoreni_spil) {
    while (!je_prazan(otvoreni_spil)) {
        dodaj_na_pocetak(zatvoreni_spil, pop(otvoreni_spil));
    }
}
int odigraj_potez(int igrac, int *izvucena, Lista **zatvoreni_spil, Stek *otvoreni_spil,
    Stek *sredina_spil, Stek *protivnik_otvoreni_spil, Lista **protivnik_zatvoreni_spil
) {
    int potez, karta_vrh_spila;
    prikazi_potez_igraca_meni(igrac);
    while (1) {
        printf("(Igrac %d): Unesi potez(1,2,3,4,5,6,0): ", igrac);
        scanf("%d", &potez);
        if (potez == 0) {
            prikazi_glavni_meni();
            return 0;
        }
        if (potez == 5) {
            if (*izvucena == 0) {
                printf("Igrac %d nije povukao kartu\n", igrac);
                continue;
            }
            int *karta = malloc(sizeof(int));
            *karta = *izvucena;
            push(otvoreni_spil, karta);
            printf("Postavljena karta:(%s)\n", format_karta(*izvucena));
            *izvucena = 0;
            return igrac == 1 ? 2 : 1;
        }
        if (*izvucena == 0 && je_prazna(*zatvoreni_spil)) {
            okreni_otvoreni_spil(otvoreni_spil, zatvoreni_spil);
        }
        switch (potez) {
            case 1: {
                if (*izvucena != 0) {
                    printf("Igrac %d ima izvucenu kartu\n", igrac);
                    break;
                }
                int *karta = ukloni_sa_pocetka(zatvoreni_spil);
                if (karta) {
                    *izvucena = *karta;
                    free(karta);
                }
                printf("Izvucena karta:(%s)\n", format_karta(*izvucena));
                break;
            }
            case 2: {
                if (*izvucena != 0) {
                    printf("Igrac %d ima izvucenu kartu\n", igrac);
                    break;
                }
                if (je_prazan(otvoreni_spil)) {
                    printf("Otvoreni spil je prazan(ponovi potez)\n");
                    break;
                }
                int *karta = pop(otvoreni_spil);
                if (karta) {
                    *izvucena = *karta;
                    free(karta);
                }
                printf("Izvucena karta:(%s)\n", format_karta(*izvucena));
                break;
            }
            case 3: {
                if (*izvucena == 0) {
                    printf("Igrac %d nije povukao kartu\n", igrac);
                    break;
                }
                int *karta_vrh = pogledaj_vrh_steka(sredina_spil);
                if (karta_vrh) {
                    karta_vrh_spila = *karta_vrh;
                } else {
                    karta_vrh_spila = 0;
                }
                if (*izvucena != (karta_vrh_spila % N_KARATA) + 1) {
                    printf("Igrac %d ne moze odigrati potez(pravilo postavljanja karata)\n", igrac);
                    break;
                }
                int *karta = malloc(sizeof(int));
                *karta = *izvucena;
                push(sredina_spil, karta);
                printf("Postavljena karta:(%s)\n", format_karta(*izvucena));
                *izvucena = 0;
                break;
            }
            case 4: {
                if (*izvucena == 0) {
                    printf("Igrac %d nije povukao kartu\n", igrac);
                    break;
                }
                int *karta_vrh = pogledaj_vrh_steka(protivnik_otvoreni_spil);
                if (karta_vrh) {
                    karta_vrh_spila = *karta_vrh;
                } else {
                    karta_vrh_spila = 0;
                }
                if (*izvucena != (karta_vrh_spila % N_KARATA) + 1) {
                    printf("Igrac %d ne moze odigrati potez(pravilo postavljanja karata)\n", igrac);
                    break;
                }
                int *karta = malloc(sizeof(int));
                *karta = *izvucena;
                push(protivnik_otvoreni_spil, karta);
                printf("Postavljena karta:(%s)\n", format_karta(*izvucena));
                *izvucena = 0;
                break;
            }
            case 6: {
                int *otvorena1 = pogledaj_vrh_steka(otvoreni_spil);
                int *sredina = pogledaj_vrh_steka(sredina_spil);
                int *otvorena2 = pogledaj_vrh_steka(protivnik_otvoreni_spil);
                prikazi_trenutno_stanje(
                    velicina(igrac == 1 ? *zatvoreni_spil : *protivnik_zatvoreni_spil),
                    igrac == 1 ? *izvucena : 0,
                    velicina(igrac == 1 ? otvoreni_spil->lista : protivnik_otvoreni_spil->lista),
                    igrac == 1 ? (otvorena1 ? *otvorena1 : 0) : (otvorena2 ? *otvorena2 : 0),
                    sredina ? *sredina : 0,
                    velicina(sredina_spil->lista),
                    igrac == 2 ? (otvorena1 ? *otvorena1 : 0) : (otvorena2 ? *otvorena2 : 0),
                    velicina(igrac == 2 ? otvoreni_spil->lista : protivnik_otvoreni_spil->lista),
                    igrac == 2 ? *izvucena : 0,
                    velicina(igrac == 2 ? *zatvoreni_spil : *protivnik_zatvoreni_spil)
                );
                break;
            }
            default: {
                printf("Nepostojeca opcija\n");
                break;
            }
        }
        if (*izvucena == 0 && je_prazna(*zatvoreni_spil) && je_prazan(otvoreni_spil)) {
            printf("\n==== KRAJ IGRE ====\n(Pobednik): Igrac %d\n===================\n", igrac);
            prikazi_glavni_meni();
            return 0;
        }
    }
}
int main() {
    int opcija, potez_igraca = 0, x0 = 0, izvucena_karta1 = 0, izvucena_karta2 = 0;
    Lista *zatvoreni_spil1 = NULL, *zatvoreni_spil2 = NULL;
    Stek otvoreni_spil1, sredina_spil, otvoreni_spil2;
    otvoreni_spil1.lista = NULL;
    sredina_spil.lista = NULL;
    otvoreni_spil2.lista = NULL;
    prikazi_glavni_meni();
    while (1) {
        printf("Unesi opciju(1,2,3,4,0): ");
        scanf("%d", &opcija);
        if (opcija == 0) break;
        switch (opcija) {
            case 1: {
                while (1) {
                    printf("Unesi vidan x0(seed): ");
                    scanf("%d", &x0);
                    if (x0 > 2 && x0 % BBS_P != 0 && x0 % BBS_Q != 0) break;
                }
                ocisti_talon(&zatvoreni_spil1, &izvucena_karta1, &otvoreni_spil1, &sredina_spil,
                    &otvoreni_spil2, &izvucena_karta2, &zatvoreni_spil2
                );
                promesaj_podeli_karte(&x0, &zatvoreni_spil1, &zatvoreni_spil2);
                prikazi_glavni_meni();
                break;
            }
            case 2:
            case 3: {
                if (je_prazna(zatvoreni_spil1) && je_prazna(zatvoreni_spil2)) {
                    printf("Prvo promesaj spil (opcija: 1)\n");
                    break;
                }
                potez_igraca = (opcija == 2) ? 1 : 2;
                while (potez_igraca) {
                    if (potez_igraca == 1) {
                        potez_igraca = odigraj_potez(potez_igraca, &izvucena_karta1, &zatvoreni_spil1,
                                      &otvoreni_spil1, &sredina_spil,
                                      &otvoreni_spil2,&zatvoreni_spil2);
                    } else {
                        potez_igraca = odigraj_potez(potez_igraca, &izvucena_karta2, &zatvoreni_spil2,
                                      &otvoreni_spil2, &sredina_spil,
                                      &otvoreni_spil1,&zatvoreni_spil1);
                    }
                }
                break;
            }
            case 4: {
                int *otvorena1 = pogledaj_vrh_steka(&otvoreni_spil1);
                int *sredina = pogledaj_vrh_steka(&sredina_spil);
                int *otvorena2 = pogledaj_vrh_steka(&otvoreni_spil2);
                prikazi_trenutno_stanje(
                    velicina(zatvoreni_spil1),
                    izvucena_karta1,
                    velicina(otvoreni_spil1.lista),
                    otvorena1 ? *otvorena1 : 0,
                    sredina ? *sredina : 0,
                    velicina(sredina_spil.lista),
                    otvorena2 ? *otvorena2 : 0,
                    velicina(otvoreni_spil2.lista),
                    izvucena_karta2,
                    velicina(zatvoreni_spil2)
                );
                break;
            }
            default: {
                printf("Nepostojeca opcija\n");
                break;
            }
        }
    }
    ocisti_talon(&zatvoreni_spil1, &izvucena_karta1, &otvoreni_spil1, &sredina_spil,
        &otvoreni_spil2, &izvucena_karta2, &zatvoreni_spil2
    );
    return 0;
}