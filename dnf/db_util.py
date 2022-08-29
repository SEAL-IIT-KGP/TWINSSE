import csv
from os import write

in_file = "../databases/db6k.csv"
out_file = "db_kw_freq.csv"

out_file_handle = open(out_file,"w")

kw_counter = 0
kw_id_counter = 0
max_idx_len = 0

id_set = set()

with open(in_file) as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        row_len = (len(row) - 1)
        kw = row[0]
        kw_freq_str = kw+","+str(row_len-1)+",\n"
        kw_counter += 1
        kw_id_counter += row_len
        out_file_handle.write(kw_freq_str)
        for id in row[1:]:
            id_set.add(id)
        if(row_len > max_idx_len):
            max_idx_len = row_len

out_file_handle.close()

print("Number of keywords: "),
print(kw_counter)

print("Number of keyword-id pairs: "),
print(kw_id_counter)

print("Number of maximum ids per kw: "),
print(max_idx_len)

print("Number of unqiue ids: "),
print(len(id_set))

print("Keyword, frequency file information written to "),
print(out_file)