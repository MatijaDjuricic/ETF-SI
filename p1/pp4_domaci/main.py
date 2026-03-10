import re
def string_input():
    string = input().strip()
    if not re.match(r"^[a-zA-Z0-9.\s]+$", string):
        return False
    return string.split(' ')

def replace_digits(string_arr):
    digits_dict = {
        '0': 'nula', '1': 'jedan', '2': 'dva', '3': 'tri',
        '4': 'cetiri', '5': 'pet', '6': 'sest', '7': 'sedam',
        '8': 'osam', '9': 'devet',
    }
    digit_pattern = re.compile(rf"[{'|'.join(digits_dict.keys())}]")
    digit_name_pattern = re.compile(rf"^({'|'.join(digits_dict.values())})(-({'|'.join(digits_dict.values())}))*$")
    digit_dot_pattern = re.compile(rf"\.[{'|'.join(digits_dict.values())}]")

    for i in range(len(string_arr)):
        string_arr[i] = re.sub(r"\d{2,}", f"{'-'.join(string_arr[i])}", string_arr[i])
        string_arr[i] = re.sub(r"^\d+[a-zA-Z]+\d\.?$", f"{string_arr[i][0:6]} {string_arr[i][6:]}", string_arr[i])
        string_arr[i] = re.sub(digit_pattern, lambda x: digits_dict[x.group(0)], string_arr[i])
        string_arr[i] = re.sub(r"-\.\.$", ".", string_arr[i])
        if re.match(digit_name_pattern, string_arr[i]) and (i == 0 or re.search(pattern=r'\.$', string=string_arr[i-1])):
            string_arr[i] = string_arr[i].capitalize()
        if re.search(digit_dot_pattern, string_arr[i]):
            string_arr[i] = '.'.join([string.capitalize() if j != 0 else string for j, string in enumerate(string_arr[i].split('.'))])

    return string_arr

def main():
    string_arr = string_input()
    if not string_arr:
        return
    result = replace_digits(string_arr)
    print(*result, end='')

if __name__ == '__main__':
    main()