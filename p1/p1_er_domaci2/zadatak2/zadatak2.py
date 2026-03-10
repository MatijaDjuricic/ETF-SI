import re
def ucitavanje_podataka():
    try:
        mejlovi_poruke = {}
        ulazna_datoteka = input()
        with open(ulazna_datoteka, 'r', encoding='utf-8') as f:
            for linija in f:
                linija = linija.strip()
                mejl1, mejl2, poruka = re.split(r"[|-]", linija, maxsplit=2)

                if (mejl1, mejl2) in mejlovi_poruke.keys():
                    mejlovi_poruke[(mejl1, mejl2)].append(poruka)
                elif (mejl2, mejl1) in mejlovi_poruke.keys():
                    mejlovi_poruke[(mejl2, mejl1)].append(poruka)
                else:
                    mejlovi_poruke[(mejl1, mejl2)] = [poruka]
        izlazna_datoteka = input()
        return mejlovi_poruke, izlazna_datoteka
    except FileNotFoundError:
        print("DAT_GRESKA", end="")
        return False
    except Exception:
        print("GRESKA", end="")
        return False

def formatiranje_mejla(mejl):
    naziv_mejla = mejl.split('@')[0]
    fax = re.search(r"(?:@student\.|@)([a-z]{3})\.", mejl).group(1)
    return f"{naziv_mejla}-{fax}"

def obrada_mjelova_poruka(mejlovi_poruke, izlazna_datoteka):
    try:
        komunikacije = {}
        filtrirani_mejlovi = []

        for mejlovi in mejlovi_poruke.keys():
            filtrirani_mejlovi.extend([formatiranje_mejla(mejl) for mejl in mejlovi])
        filtrirani_mejlovi = set(filtrirani_mejlovi)

        for mejl in sorted(filtrirani_mejlovi):
            komunikacije[mejl] = []
            broj_karaktera_poruka = 0
            for (mejl1, mejl2), poruke in mejlovi_poruke.items():
                if mejl == formatiranje_mejla(mejl1):
                    for poruka in poruke:
                        broj_karaktera_poruka += len(poruka)
                    komunikacije[mejl].append(formatiranje_mejla(mejl2))
                elif mejl == formatiranje_mejla(mejl2):
                    for poruka in poruke:
                        broj_karaktera_poruka += len(poruka)
                    komunikacije[mejl].append(formatiranje_mejla(mejl1))
            komunikacije[(mejl, broj_karaktera_poruka)] = komunikacije[mejl]
            del komunikacije[mejl]

        with open(izlazna_datoteka, 'w', encoding='utf-8') as f:
            for (mejl, broj_karaktera_poruka), mejlovi in komunikacije.items():
                f.write(f"{mejl}({broj_karaktera_poruka}):{' '.join([mejl for mejl in sorted(mejlovi)])}\n")
    except Exception:
        print("GRESKA", end="")

podaci = ucitavanje_podataka()
if podaci:
    mejlovi_poruke, izlazna_datoteka = podaci
    obrada_mjelova_poruka(mejlovi_poruke, izlazna_datoteka)