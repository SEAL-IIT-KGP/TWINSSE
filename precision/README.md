# Precision Estimation

This repository subdirectory contains the source codes for the precision estimation experiment. The [`dis_query_process.cpp`](./dis_query_process.cpp) contains the functions to computer precision values from meta-databases. Whereas [`gen_mkw.cpp`](./gen_mkw.cpp) file contains the functions to generate meta-keywords. This is necessary to obtain unhashed meta-keywords to compute the precision values since the meta-databases store the hashes of the meta-keywords. Hence, without using the underlying OXT, precision computation requires the unhashed meta-keywords.

---

### Source Code Overview

Change the specific lines in the following files.

*main.cpp*

```C++
std::string raw_db_file = "../databases/db6k.dat";
std::string meta_db_file = "../databases/mdb6k.dat";
std::string avg_precision_file = "../results/avg_precision.dat";
```
The `raw_db_file` is given as an input database in inverted index form. The `meta_db_file` stores all the hashed meta-keywords generated from the [`gen_mkw.cpp`](./gen_mkw.cpp) file. The `avg_precision_file` file stores the average precision values for each Hamming weight (number of keywords in a disjunctive query). For example, if we generate 1000 queries for each of Hamming weights 2, 3, and 4, then it will calculate the average precision value over all 1000 queries for each Hamming weight and store the three different average values(for each Hamming weight) in the file.


---
*utils.h*

```C++
#define N_KW 6043 //Number of keywords
#define N_ID 9690 //Number of unique ids in the database
```
`N_KW` and `N_ID` are the total number of keywords and unique document ids (respectively) in a database. Change these values accordingly while changing the input database file.

---

### Building and Running Experiment
To compute the precision values of the disjunctive query results, execute the following command to compute the executable `gen_db` and run the executable.

```bash
make all
./gen_db
```

---

## Things to Remember

- Double-check the system parameters. If even one mismatch is there, the result would be incorrect.
- Check if the input files are being read correctly (otherwise, the program will terminate abruptly).
- Timings can widely vary depending upon the system configuration and load (and the version of packages used).
