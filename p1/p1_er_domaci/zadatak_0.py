def ucitavanje_podataka():
    lista_autobusa = [x for x in input().split(",")]
    satnica = list(input().split(":"))
    polasci = []
    HH = satnica[0]
    MM = satnica[1]
    if HH[0] == "0":
        HH = int(HH[1])
    else:
        HH = int(HH)
    if MM[0] == "0":
        MM = int(MM[1])
    else:
        MM = int(MM)
    while True:
        unos = input()
        if unos == "":
            break
        linija_autobusa = unos.split(",")[0]
        broj_autobusa = unos.split(",")[1]
        vreme_polaska = unos.split(",")[2].split(":")
        HH_polaska = vreme_polaska[0]
        MM_polaska = vreme_polaska[1]
        if HH_polaska[0] == "0":
            HH_polaska = int(HH_polaska[1])
        else:
            HH_polaska = int(HH_polaska)
        if MM_polaska[0] == "0":
            MM_polaska = int(MM_polaska[1])
        else:
            MM_polaska = int(MM_polaska)
        polasci.append([linija_autobusa, broj_autobusa, HH_polaska, MM_polaska])
    return polasci, lista_autobusa, HH, MM

def podudaranje(polasci, autobusi, HH, MM):
    polasci_lista = []
    for polazak in polasci:
        for bus in autobusi:
            if bus == polazak[0] and polazak[2] * 60 + polazak[3] >= HH * 60 + MM:
                polasci_lista.append(polazak)
    return polasci_lista

def sortiranje(polasci, lista_autobusa, HH, MM):
    prvi_polazak = 0
    borj_polazaka = 0
    polasci.sort(key=lambda x: x[2] * 60 + x[3])
    podudaran_polasci = podudaranje(polasci, lista_autobusa, HH, MM)
    if len(podudaran_polasci) == 0:
        return prvi_polazak, borj_polazaka
    borj_polazaka = len(podudaran_polasci)
    podudaran_polasci.sort(key=lambda x: x[2] * 60 + x[3])
    prvi_polazak = podudaran_polasci[0]
    return prvi_polazak, borj_polazaka

def ispis(prvi_polazak, borj_polazaka):
    print(borj_polazaka)
    if borj_polazaka == 0 and len(prvi_polazak) == 0:
        print("Nema autobusa.", end='')
    else:
        print(
            f"{prvi_polazak[0]}-{prvi_polazak[1]} "
            f"({prvi_polazak[2] if prvi_polazak[2] > 9 else f'0{prvi_polazak[2]}'}:"
            f"{prvi_polazak[3] if prvi_polazak[3] > 9 else f'0{prvi_polazak[3]}'})"
        ,end='')

polasci, lista_autobusa, HH, MM = ucitavanje_podataka()
if 0 <= HH < 24 and 0 <= MM < 60 and len(polasci) > 0 and len(lista_autobusa) > 0:
    prvi_polazak, broj_polazaka = sortiranje(polasci, lista_autobusa, HH, MM)
    ispis(prvi_polazak, broj_polazaka)