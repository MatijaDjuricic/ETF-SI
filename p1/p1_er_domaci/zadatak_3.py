def ucitavanje_podataka():
    broj_piva = int(input())
    if broj_piva < 1:
        return [], []
    lista_piva = []
    for _ in range(broj_piva):
        linija_unosa = input()
        naziv_piva = linija_unosa.split("|")[0]
        procenat_alkohola = linija_unosa.split("|")[1]
        kolicina_mililitri = linija_unosa.split("|")[2]
        cena_piva = linija_unosa.split("|")[3]
        lista_piva.append([naziv_piva, float(procenat_alkohola), int(kolicina_mililitri), int(cena_piva)])
    popijena_piva = [x for x in input().split(", ")]
    return lista_piva, popijena_piva

def racunanje_kolicine_alkohola(lista_piva, popijena_piva):
    kolicina_mililitri = 0
    ukupno_alkohola_mililitri = 0
    for popijeno_pivo in popijena_piva:
        for pivo in lista_piva:
            if popijeno_pivo == pivo[0]:
                kolicina_mililitri += pivo[2]
                ukupno_alkohola_mililitri += (pivo[1] / 100) * pivo[2]
    print(f"Kolicina: {(kolicina_mililitri / 1000):.2f}L")
    print(f"Alkohola: {(ukupno_alkohola_mililitri / kolicina_mililitri) * 100:.2f}%")

def pronadji_neprobana_piva(lista_piva, popijena_piva):
    neprobana_piva = []
    for pivo in lista_piva:
        if pivo[0] not in popijena_piva:
            neprobana_piva.append(pivo[0])
    for pivo in neprobana_piva:
        print(pivo)

lista_piva, popijena_piva = ucitavanje_podataka()
if len(lista_piva) > 0 and len(popijena_piva) > 0:
    racunanje_kolicine_alkohola(lista_piva, popijena_piva)
    pronadji_neprobana_piva(lista_piva, popijena_piva)