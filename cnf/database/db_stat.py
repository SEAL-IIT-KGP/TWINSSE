import csv
from os import write

in_file = "sample_database.csv"

kw_counter = 0
kw_id_counter = 0
max_idx_len = 0

kw_set = set()

with open(in_file) as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        row_len = (len(row) - 1)
        kw_counter += 1
        kw_id_counter += row_len
        for id in row[1:]:
            kw_set.add(id)
        if(row_len > max_idx_len):
            max_idx_len = row_len

print(kw_counter)
print(kw_id_counter)
print(max_idx_len)
print(len(kw_set))