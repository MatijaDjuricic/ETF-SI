def ucitavanje_podataka():
    min_vreme, min_procenat_baterije = [int(x) for x in input().split(",")]
    lista_podataka = []
    while True:
        linija_unosa = input()
        if linija_unosa == "":
            break
        naziv_aplikacije = linija_unosa.split(",")[0]
        potroseno_vreme = int(linija_unosa.split(",")[1])
        procenat_potrosena_baterija = int(linija_unosa.split(",")[2])
        lista_podataka.append([naziv_aplikacije, potroseno_vreme, procenat_potrosena_baterija])
    return lista_podataka, min_vreme, min_procenat_baterije

def grupisi_aplikacije(lista_podataka):
    grupisane_aplikacije = []
    for i in range(len(lista_podataka)):
        duplikat = False
        for j in range(len(grupisane_aplikacije)):
            if lista_podataka[i][0] == grupisane_aplikacije[j][0]:
                duplikat = True
                grupisane_aplikacije[j][1] += lista_podataka[i][1]
                grupisane_aplikacije[j][2] += lista_podataka[i][2]
        if not duplikat:
            grupisane_aplikacije.append(lista_podataka[i])
    return grupisane_aplikacije

def filtriranje_aplikacija(grupisana_lista, min_vreme, min_procenat_baterije):
    filtrirane_aplikacije = []
    for i in range(len(grupisana_lista)):
        if min_vreme <= grupisana_lista[i][1] and min_procenat_baterije <= grupisana_lista[i][2]:
            filtrirane_aplikacije.append(grupisana_lista[i])
    return filtrirane_aplikacije

def ispis(filtrirane_aplikacije):
    for aplikacija in filtrirane_aplikacije:
        hh = aplikacija[1] // 60
        mm = aplikacija[1] % 60
        print(f"{aplikacija[0]} ({hh if hh > 9 else f'0{hh}'}:{mm if mm > 9 else f'0{mm}'},{aplikacija[2]}%)")

lista_podataka, min_vreme, min_procenat_baterije = ucitavanje_podataka()
if 0 <= min_vreme < 24 * 60 and min_procenat_baterije > 0:
    grupisana_lista = grupisi_aplikacije(lista_podataka)
    filtrirane_aplikacije = filtriranje_aplikacija(grupisana_lista, min_vreme, min_procenat_baterije)
    ispis(filtrirane_aplikacije)