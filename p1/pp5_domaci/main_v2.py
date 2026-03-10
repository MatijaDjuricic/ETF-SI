import csv
def ucitaj_podatke():
    try:
        kupovine = []
        unos_od, unos_do = input().strip().split(' ')
        dan_mesec_godina_od = unos_od.split('/')
        dan_od = int(dan_mesec_godina_od[0])
        mesec_od = int(dan_mesec_godina_od[1])
        godina_od = int(dan_mesec_godina_od[2])
        dan_mesec_godina_do = unos_do.split('/')
        dan_do = int(dan_mesec_godina_do[0])
        mesec_do = int(dan_mesec_godina_do[1])
        godina_do = int(dan_mesec_godina_do[2])
        if not (1 <= (dan_od or dan_do) <= 31 and 1 <= (mesec_od or mesec_do) <= 12 and (godina_od or godina_do) > 0):
            print("GRESKA")
            return False
        with open("kupovine.csv", "r") as f:
            fajl = csv.reader(f)
            for red in fajl:
                datum, kartica, ime, cena, materijal, vreme = red
                dan_mesec_godina = datum.split('/')
                dan = int(dan_mesec_godina[0])
                mesec = int(dan_mesec_godina[1])
                godina = int(dan_mesec_godina[2])
                sat_minut = vreme.split(':')
                sat = int(sat_minut[0])
                minut = int(sat_minut[1])
                if not (1 <= dan <= 31 and 1 <= mesec <= 12 and godina > 0 and 0 <= sat <= 23 and 0 <= minut <= 59):
                    print("GRESKA")
                    return False
                kupovine.append([kartica, materijal, float(cena), godina, mesec, dan, sat, minut])
        return kupovine, (godina_od, mesec_od, dan_od, 0, 0), (godina_do, mesec_do, dan_do, 23, 59)
    except FileNotFoundError:
        print("DAT_GRESKA")
        return False
    except Exception:
        print("GRESKA")
        return False
def sacuvaj_rezultat(kupovine, datum_od, datum_do):
    try:
        kartice_materijali_cene = {}
        for (kartica, materijal, cena, god, mes, dan, sat, minut) in kupovine:
            if datum_od <= (god, mes, dan, sat, minut) <= datum_do:
                if kartica not in kartice_materijali_cene.keys():
                    kartice_materijali_cene[kartica] = {}
                if materijal not in kartice_materijali_cene[kartica].keys():
                    kartice_materijali_cene[kartica][materijal] = []
                kartice_materijali_cene[kartica][materijal].append(cena)
        with open("kartice.csv", "w", newline="") as f:
            fajl = csv.writer(f)
            for kartica, materijali_cene in sorted(kartice_materijali_cene.items()):
                sortirani_materijali_cene = sorted(materijali_cene.items(), key=lambda x: (-sum(x[1]), x[0]))
                for materijal, cene in sortirani_materijali_cene:
                    red = [kartica, materijal, f"{sum(cene):.2f}"]
                    fajl.writerow(red)
    except Exception:
        print("GRESKA")
podaci = ucitaj_podatke()
if podaci:
    kupovine, datum_od, datum_do = podaci
    sacuvaj_rezultat(kupovine, datum_od, datum_do)