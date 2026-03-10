def ucitavanje_podataka():
    try:
        letovi = {}
        unos_grad_od, unos_grad_do = input().split('->')

        with open('flights.txt', 'r') as f:
            for linija in f:
                linija = linija.strip()
                avio_kompanija = linija.split('|')[0]
                par_gradova = linija.split('|')[1]
                grad_od = par_gradova.split('->')[0]
                grad_do = par_gradova.split('->')[1]
                satnice_leta = linija.split('|')[2].split(';')
                lista_satnice_cena = []
                for satnica in satnice_leta:
                    satnica_od = satnica.split(',')[0].split('-')[0]
                    satnica_do = satnica.split(',')[0].split('-')[1]
                    cena = satnica.split(',')[1]
                    lista_satnice_cena.append([satnica_od, satnica_do, cena])
                if (grad_od, grad_do) in letovi:
                    letovi[(grad_od, grad_do)].append({avio_kompanija: lista_satnice_cena})
                else:
                    letovi[(grad_od, grad_do)] = [{avio_kompanija: lista_satnice_cena}]

        return letovi, unos_grad_od, unos_grad_do
    except FileNotFoundError:
        print("DAT_GRESKA", end="")
        return False
    except Exception:
        print("GRESKA", end="")
        return False

def obrada_direktnih_linija(letovi):
    try:
        for _, lista_kompanija_satnica_cena in letovi.items():
            lista_kompanija_satnica_cena.sort(key=lambda x: list(x.keys()))
        with open('flights_direct.txt', 'w') as f:
            for (grad_od, grad_do), lista_kompanija_satnica_cena in sorted(letovi.items()):
                f.write(f"{grad_od}->{grad_do}\n")
                for kompanija_stanice_cene in lista_kompanija_satnica_cena:
                    for kompanija, satnice_cene in kompanija_stanice_cene.items():
                        f.write(f"\t{kompanija}\n")
                        for satnica_cena in satnice_cene:
                            satnica_od, satnica_do, cena = satnica_cena
                            f.write(f'\t\t{satnica_od}-{satnica_do} | {cena}\n')
    except Exception:
        print("GRESKA", end="")
        return
def obrada_indirektnih_linija(letovi, grad_polaska, grad_dolaska):
    try:
        with open("flights_indirect.txt", "w") as f:
            for (polazni_grad, srednji_grad) in sorted(letovi.keys()):
                if polazni_grad == grad_polaska and (srednji_grad, grad_dolaska) in letovi:
                    f.write(f"{grad_polaska}->{srednji_grad}->{grad_dolaska}\n")
                    prvi_letovi = []
                    for kompanije in letovi[(grad_polaska, srednji_grad)]:
                        for kompanija, satnice in kompanije.items():
                            for pol1, dol1, cena1 in satnice:
                                t_pol1 = int(pol1[:2]) * 60 + int(pol1[3:])
                                t_dol1 = int(dol1[:2]) * 60 + int(dol1[3:])
                                trajanje1 = t_dol1 - t_pol1
                                prvi_letovi.append(
                                    (pol1, dol1, t_dol1, trajanje1, kompanija, cena1)
                                )

                    prvi_letovi.sort(key=lambda x: (x[0], x[3], x[4]))

                    drugi_letovi = []
                    for kompanije in letovi[(srednji_grad, grad_dolaska)]:
                        for kompanija, satnice in kompanije.items():
                            for pol2, dol2, cena2 in satnice:
                                t_pol2 = int(pol2[:2]) * 60 + int(pol2[3:])
                                t_dol2 = int(dol2[:2]) * 60 + int(dol2[3:])
                                trajanje2 = t_dol2 - t_pol2
                                drugi_letovi.append((pol2, dol2, t_pol2, trajanje2, kompanija, cena2))

                    drugi_letovi.sort(key=lambda x: (x[0], x[3], x[4]))

                    for pol1, dol1, vreme_dolaska1, _, komp1, cena1 in prvi_letovi:
                        validni_drugi = []
                        for pol2, dol2, vreme_polaska2, _, komp2, cena2 in drugi_letovi:
                            if vreme_polaska2 > vreme_dolaska1:
                                validni_drugi.append((pol2, dol2, komp2, cena2))

                        if not validni_drugi:
                            continue

                        f.write(f"\t{pol1}-{dol1} | {komp1} | {cena1}\n")
                        ispisani = set()
                        for pol2, dol2, komp2, cena2 in validni_drugi:
                            kljuc = (pol2, dol2, komp2, cena2)
                            if kljuc not in ispisani:
                                f.write(f"\t\t{pol2}-{dol2} | {komp2} | {cena2}\n")
                                ispisani.add(kljuc)
    except Exception:
        print("GRESKA", end="")

podaci = ucitavanje_podataka()
if podaci:
    letovi, grad_od, grad_do = podaci
    obrada_direktnih_linija(letovi)
    obrada_indirektnih_linija(letovi, grad_od, grad_do)