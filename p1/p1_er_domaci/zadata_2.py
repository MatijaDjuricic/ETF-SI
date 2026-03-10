def ucitavanje_podataka():
    linija_unosa = input().split("-")
    tim1 = linija_unosa[0]
    tim2 = linija_unosa[1]
    tim1_minuti = []
    tim2_minuti = []
    for minut in tim1.split(","):
        if minut != "":
            tim1_minuti.append(int(minut))
    for minut in tim2.split(","):
        if minut != "":
            tim2_minuti.append(int(minut))
    return tim1_minuti, tim2_minuti

def racunanje_rezultat(tim1_minuti, tim2_minuti):
    tim1_poluvreme = 0
    tim2_poluvreme = 0
    rezultat_utakmice = f"{len(tim1_minuti)}:{len(tim2_minuti)}"
    for minut in tim1_minuti:
        if 0 < minut <= 45:
            tim1_poluvreme += 1
    for minut in tim2_minuti:
        if 0 < minut <= 45:
            tim2_poluvreme += 1
    rezultat_na_poluvremenu = f"{tim1_poluvreme}:{tim2_poluvreme}"
    return rezultat_utakmice, rezultat_na_poluvremenu

def racunanje_vremenskog_opsega(tim1_minuti, tim2_minuti):
    prvi_gol_vreme = 0
    poslednji_gol_vreme = 0
    if len(tim1_minuti) > 0:
        prvi_gol_vreme = min(tim1_minuti)
        poslednji_gol_vreme = max(tim1_minuti)
    if len(tim2_minuti) != 0 and prvi_gol_vreme >= min(tim2_minuti):
        prvi_gol_vreme = min(tim2_minuti)
    if len(tim2_minuti) != 0 and poslednji_gol_vreme <= max(tim2_minuti):
        poslednji_gol_vreme = max(tim2_minuti)
    return prvi_gol_vreme, poslednji_gol_vreme

tim1_minuti, tim2_minuti = ucitavanje_podataka()

validan_opseg_minuta = True
for minut in tim1_minuti:
    if minut <= 0 or minut > 90:
        validan_opseg_minuta = False
for minut in tim2_minuti:
    if minut <= 0 or minut > 90:
        validan_opseg_minuta = False

if validan_opseg_minuta:
    rezultat_utakmice, rezultat_na_poluvremenu = racunanje_rezultat(tim1_minuti, tim2_minuti)
    prvi_gol_vreme, poslednji_gol_vreme = racunanje_vremenskog_opsega(tim1_minuti, tim2_minuti)
    print(tim1_minuti)
    print(tim2_minuti)
    print(f"{rezultat_utakmice} ({rezultat_na_poluvremenu})")
    if prvi_gol_vreme != 0 and poslednji_gol_vreme != 0:
        print(f"{prvi_gol_vreme}-{poslednji_gol_vreme}", end='')