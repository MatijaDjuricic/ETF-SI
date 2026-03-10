def main():
    dictionary = {}
    with open('mejlovi.txt', 'r') as f:
        for line in f.readlines():
            found = False
            fax = line.strip().split('.')[1].upper()
            year = int(line.strip()[2:4])
            studies_type = line.strip().split('@')[0][-1].lower()
            for key, value in dictionary.items():
                if key == fax and studies_type == 'd' and year < 16:
                    found = True
                    dictionary[fax] += 1
            if not found:
                if studies_type == 'd' and year < 16:
                    dictionary[fax] = 1
    with open('statistika.txt', 'w') as f:
        for key, value in dictionary.items():
            f.write(f"{key}: {value}\n")
if __name__ == '__main__':
    main()