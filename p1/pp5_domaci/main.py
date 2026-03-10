import csv
from datetime import datetime
def input_data():
    try:
        purchases = []
        date_from, date_until = input().strip().split()
        day_from, month_from, year_from = list(map(int, date_from.split('/')))
        day_until, month_untl, year_until = list(map(int, date_until.split('/')))
        with open("kupovine.csv", 'r') as f:
            csv_reader = csv.reader(f)
            for row in csv_reader:
                date, card_type, customer_name, price, material_type, time = row
                day, month, year = list(map(int, date.split('/')))
                hours, minutes = list(map(int, time.split(':')))
                purchases.append([card_type, material_type, float(price), datetime(year, month, day, hours, minutes)])
        return purchases, datetime(year_from, month_from, day_from), datetime(year_until, month_untl, day_until)
    except FileNotFoundError:
        print("DAT_GRESKA")
        return False
    except Exception:
        print("GRESKA")
        return False
def output_data(purchases, date_from, date_until):
    try:
        card_material_price = {}
        for (card_type, material_type, price, date) in purchases:
            if date_from <= date <= date_until:
                if card_type in card_material_price.keys():
                    if material_type in card_material_price[card_type].keys():
                        card_material_price[card_type][material_type].append(price)
                    else: card_material_price[card_type][material_type] = [price]
                else:
                    card_material_price[card_type] = {material_type: [price]}
        with open("kartice.csv", 'w', newline='') as f:
            writer = csv.writer(f)
            for card_type, material_price_dict in sorted(card_material_price.items()):
                for material, price_list in sorted(material_price_dict.items(), key=lambda x: (-sum(x[1]), x[0])):
                    writer.writerow([card_type, material, f"{sum(price_list):.2f}"])
    except Exception:
        print("GRESKA")
def main():
    data = input_data()
    if not data:
        return
    purchases, date_from, date_until = data
    output_data(purchases, date_from, date_until)
if __name__ == '__main__':
    main()