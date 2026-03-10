import re
def read_input_txt():
    try:
        filename = input()
        with open(filename, mode='r', encoding='utf-8') as f:
            return f.readlines()
    except FileNotFoundError:
        return "DAT_GRESKA"
    except ValueError:
        return "GRESKA"

def read_dict_txt():
    dictionary = {}
    try:
        filename = input()
        with open(filename, mode='r', encoding='utf-8') as f:
            for line in f.readlines():
                line = line.strip()
                dictionary[line.split(' ')[0]] = line.split(' ')[1]
            f.seek(0)
            return f.readlines(), dictionary
    except FileNotFoundError:
        return "DAT_GRESKA", dictionary
    except ValueError:
        return "GRESKA", dictionary

def remove_empty_space(lines_list):
    correct_lines = []
    for line in lines_list:
        line = line.strip()
        line = re.sub(r'\s+', ' ', line)
        if line != '':
            correct_lines.append(line)
    return correct_lines

def replace_letter(lines_list):
    replaced_letters_list = []
    letters_dict = {
        "Č": "C", "Ć": "C", "Đ": "Dj", "Š": "S", "Ž": "Z",
        "č": "c", "ć": "c", "đ": "dj", "š": "s", "ž": "z",
    }
    for line in lines_list:
        for key, value in letters_dict.items():
            line = line.replace(key, value)
        replaced_letters_list.append(line)
    return replaced_letters_list

def find_and_replace(lines_list, dictionary):
    replaced_lines_list = []
    for line in lines_list:
        for key, value in dictionary.items():
            pattern = rf'\b{re.escape(key)}'
            line = re.sub(pattern, value, line)
        replaced_lines_list.append(line)
    return replaced_lines_list

def main():
    file_content = read_input_txt()
    dict_content, dictionary = read_dict_txt()
    if type(file_content) == str:
        print(file_content, end='')
        return
    if type(dict_content) == str:
        print(dict_content, end='')
        return
    if len(file_content) == 0 or len(dict_content) == 0:
        print("GRESKA", end='')
        return
    correct_lines_list = remove_empty_space(file_content)
    replaced_letters_list = replace_letter(correct_lines_list)
    try:
        with open("preslovljen.txt", mode='w', encoding='utf-8') as f:
            for line in replaced_letters_list:
                f.write(line + '\n')
    except Exception:
        print("GRESKA", end='')
        return
    replaced_content_list = find_and_replace(replaced_letters_list, dictionary)
    try:
        with open("preveden.txt", mode='w', encoding='utf-8') as f:
            for line in replaced_content_list:
                f.write(line + '\n')
    except Exception:
        print("GRESKA", end='')
        return
    for line in file_content:
        print(line, end='')
    for word in dict_content:
        print(word, end='')
    for line in correct_lines_list:
        print(line)
if __name__ == '__main__':
    main()