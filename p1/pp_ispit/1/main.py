import csv
def main():
    dictionary = {}
    with open("poeni.csv", mode="r") as file:
        reader = csv.reader(file)
        for line_list in reader:
            found = False
            inner_dict = {}
            letter1 = line_list[1].strip().split(' ')[1][0].lower()
            letter2 = line_list[1].strip().split(' ')[0][0].lower()
            index_year = int(line_list[0].strip().split('/')[1][2:])
            index_num = int(line_list[0].strip().split('/')[0])
            index = index_year * 10000 + index_num
            filename = f"{letter1}{letter2}{index}"
            quizzes = [int(x) for x in line_list[3:-1]]
            k = line_list[2].strip()
            task = int(line_list[-1].strip())
            points = 0
            for q in quizzes:
                if q == 1 and points < 45:
                    points += 15
                elif q == -1:
                    points -= float(3.75)
            inner_dict[k] = points + (task / 100 * 55)
            for key, value in dictionary.items():
                if filename == key:
                    found = True
                    dictionary[filename][k] = inner_dict[k]
            if not found:
                dictionary[filename] = inner_dict
    for filename, inner_dict in dictionary.items():
        sum_result = 0
        for key, value in inner_dict.items():
            if key == 'K1':
                sum_result += inner_dict[key] * 0.3
            elif key == 'K2' or k == 'K3':
                sum_result += inner_dict[key] * 0.35
        dictionary[filename]['UK'] = sum_result
    for filename, inner_dict in dictionary.items():
        with open(f"{filename}.txt", mode="w") as f:
            for key, value in inner_dict.items():
                f.write(f"{key}:  {value:.2f}\n")
if __name__ == "__main__":
    main()