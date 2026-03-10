def ucitavanje_podataka():
    try:
        telefon_trajanje = {}
        ulazna_datoteka = input()
        with open(ulazna_datoteka, 'r') as f:
            for linija in f:
                linija = linija.strip()
                broj, pozivi = linija.split("|")
                lista_poziva = pozivi.split(",")
                brojevi_telefona_trajanje = {}
                for telefon_min_sec in lista_poziva:
                    minuti_sekunde = telefon_min_sec.split("(")[1].strip(")").split(":")
                    minuti = int(minuti_sekunde[0])
                    sekunde = int(minuti_sekunde[1])
                    telefon = telefon_min_sec.split('(')[0].replace("+381", "0")
                    if telefon in brojevi_telefona_trajanje:
                        brojevi_telefona_trajanje[telefon].append((minuti, sekunde))
                    else:
                        brojevi_telefona_trajanje[telefon] = [(minuti, sekunde)]
                broj = broj.replace("+381", "0")
                telefon_trajanje[broj] = brojevi_telefona_trajanje
        izlazna_datoteka = input()
        return telefon_trajanje, izlazna_datoteka
    except FileNotFoundError:
        print("DAT_GRESKA", end="")
        return False
    except Exception:
        print("GRESKA", end="")
        return False
def obrada_poziva(telefon_trajanje, izlazna_datoteka):
    try:
        brojevi_razgovori = {}
        filtrirani_brojevi = []

        for broj, brojevi_telefona_trajanje in telefon_trajanje.items():
            for telefon, _ in brojevi_telefona_trajanje.items():
                filtrirani_brojevi.append(telefon)
            filtrirani_brojevi.append(broj)
        filtrirani_brojevi = set(filtrirani_brojevi)

        for broj in sorted(filtrirani_brojevi):
            brojevi_razgovori[broj] = []
            vreme_razgovora_sec = 0
            for broj_kljuc, brojevi_telefona_trajanje in telefon_trajanje.items():
                if broj == broj_kljuc:
                    for telefon, lista_min_sec in brojevi_telefona_trajanje.items():
                        for (minut, sekund) in lista_min_sec:
                            vreme_razgovora_sec += minut * 60 + sekund
                        brojevi_razgovori[broj].append(telefon)
                elif broj in brojevi_telefona_trajanje:
                    for telefon, lista_min_sec in brojevi_telefona_trajanje.items():
                        if broj == telefon:
                            for (minut, sekund) in lista_min_sec:
                                vreme_razgovora_sec += minut * 60 + sekund
                            brojevi_razgovori[broj].append(broj_kljuc)

            brojevi_razgovori[(broj, vreme_razgovora_sec)] = set(brojevi_razgovori[broj])
            del brojevi_razgovori[broj]

        with open(izlazna_datoteka, 'w') as f:
            for (broj, vreme_razgovora_sec), lista_poziva in brojevi_razgovori.items():
                f.write(f"{broj}({vreme_razgovora_sec}):{','.join(telefon for telefon in sorted(lista_poziva))}\n")
    except Exception:
        print("GRESKA", end="")

podaci = ucitavanje_podataka()
if podaci:
    telefon_trajanje, izlazna_datoteka = podaci
    obrada_poziva(telefon_trajanje, izlazna_datoteka)