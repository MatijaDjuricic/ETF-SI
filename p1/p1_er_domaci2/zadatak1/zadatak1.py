def ucitavanje_podataka():
    try:
        mange = {}
        with open("manga.txt", 'r', encoding='utf-8') as f:
            for linija in f:
                linija = linija.strip()

                naziv, izdavac, datum, broj_strana_toma, *broj_str_poglavlja = linija.split(', ')
                broj_strana_toma = int(broj_strana_toma)
                brojevi_str_poglavlja = list(map(int, broj_str_poglavlja))

                if (naziv, izdavac) not in mange:
                    mange[(naziv, izdavac)] = []

                mange[(naziv, izdavac)].append((datum, broj_strana_toma, brojevi_str_poglavlja))

        naziv_izdavaca = input().strip()
        return naziv_izdavaca, mange

    except FileNotFoundError:
        print("DAT_GRESKA", end="")
        return False
    except Exception:
        print("GRESKA", end="")
        return False

def izlaz_manga_info(naziv_izdavaca, mange):
    try:
        naziv_fajla = naziv_izdavaca.replace(" ", "_").lower()
        with open(f"{naziv_fajla}.txt", 'w', encoding='utf-8') as f:
            for (naziv_mange, izdavac) in sorted(mange.keys()):
                if izdavac == naziv_izdavaca:
                    tomovi = []
                    for datum, broj_strana, poglavlja in mange[(naziv_mange, izdavac)]:
                        mesec, godina = map(int, datum.strip('.').split('.'))
                        tomovi.append((godina, mesec, broj_strana, poglavlja))

                    tomovi.sort()

                    duzine_poglavlja = []
                    for _, _, broj_strana, poglavlja in tomovi:
                        for i in range(len(poglavlja)):
                            if i + 1 < len(poglavlja):
                                duzine_poglavlja.append(poglavlja[i + 1] - poglavlja[i])
                            else:
                                duzine_poglavlja.append(broj_strana - poglavlja[i])

                    broj_tomova = len(tomovi)
                    ukupan_broj_poglavlja = len(duzine_poglavlja)
                    prosek = sum(duzine_poglavlja) / ukupan_broj_poglavlja

                    f.write(f"{naziv_mange}, {broj_tomova}, {ukupan_broj_poglavlja}, {prosek:.2f}\n")
                    f.write(', '.join(map(str, duzine_poglavlja)) + '\n')

    except Exception:
        print("GRESKA", end="")

def izlaz_poglavlja_mange(naziv_mange, mange):
    try:
        tomovi = []
        for (naziv, _), lista_tomova in mange.items():
            if naziv == naziv_mange:
                for datum, broj_strana, poglavlja in lista_tomova:
                    mesec, godina = map(int, datum.strip('.').split('.'))
                    tomovi.append((godina, mesec, broj_strana, poglavlja))

        tomovi.sort()
        rezultati = []
        globalni_broj_poglavlja = 0

        for i, (_, _, broj_strana, poglavlja) in enumerate(tomovi):
            duzine = []
            for j in range(len(poglavlja)):
                if j + 1 < len(poglavlja):
                    duzine.append(poglavlja[j + 1] - poglavlja[j])
                else:
                    duzine.append(broj_strana - poglavlja[j])

            min_duzina = min(duzine)
            for idx, d in enumerate(duzine):
                if d == min_duzina:
                    rezultati.append((i + 1, globalni_broj_poglavlja + idx + 1, d))

            globalni_broj_poglavlja += len(duzine)

        najmanja = min(r[2] for r in rezultati)

        with open("chapters.txt", 'w', encoding='utf-8') as f:
            for tom, poglavlje, d in sorted(rezultati, key=lambda x: x[1]):
                if d == najmanja:
                    f.write(f"{tom}.{poglavlje}\n")

            f.write(f"{najmanja}str")

    except Exception:
        print("GRESKA", end="")

podaci = ucitavanje_podataka()
if podaci:
    naziv_izdavaca, mange = podaci
    izlaz_manga_info(naziv_izdavaca, mange)
    naziv_mange = input().strip()
    if naziv_mange != '':
        izlaz_poglavlja_mange(naziv_mange, mange)