# CNF Expression Evaluation
## Overview

This subdirectory contains the codes to evaluate CNF expressions of the form

$q = (w_1\lor w_2)\land(w_3\lor w_4)$.

The main code basically has two top-level files - [`sse_setup.cpp`](./sse_setup.cpp) and [`sse_search.cpp`](./sse_search.cpp) responsible for exectuing the setup and search routines respectively. The [`Makefile`](./Makefile) for this codebase has the following main routines `sse_setup` and `sse_search` that generates two executables `sse_setup` and `sse_search` that exectues the setup and search routine. Additionally, there is a [`db_util.py`](./db_util.py) utility script the generates the keyword-frequency record necessary for query processng.

Please esure that the following paratemeters are set properly before builind `sse_setup` and `sse_search`.

---

## Parameter Setting in Source Files

Ensure the following paremeters in specific files have appropriate values.

---

**size_parameters.h**

Change `N_HASH` to the number of hash functions to be used for Bloom filter. Currently we set this to the number of threads used.

```C++
#define N_HASH 24 //Set this to the number of threads
```

Change `MAX_BF_BIN_SIZE` to the power of two just above the total number of unqiue meta-keyword-document-id pairs in the meta-database. For example, if there are 1142496 unique meta-keyword-id pairs in the database, the power of two just above 1142496 is 2097152 = $2^{21}$. Hence, set this to 2097152. This is used to calculate total Bloom filter size.

```C++
#define MAX_BF_BIN_SIZE 2097152 // 2**21 just above total #kw-id pairs
```

---

**mainwindow.cpp**

Change this line in `EDB_Setup()` function to generate an address spanning the Bloom filter. The number of bits to consider is derived from the above `MAX_BF_BIN_SIZE` value. Here `MAX_BF_BIN_SIZE` is $2^{21}$, that requires 21 bits. Hence, 21 bits are extracted from `bash` value to compute an index (element of `bf_indices`).

```C++
bf_indices[j] = (bhash[64*j] & 0xFF) + ((bhash[64*j+1] & 0xFF) << 8) + ((bhash[64*j+2] & 0x1F) << 16);
```

The same modification has to be done in function `EDB_Search()`.
```C++
bf_n_indices[j][i] = (bhash[64*j] & 0xFF) + ((bhash[64*j+1] & 0xFF) << 8) + ((bhash[64*j+2] & 0x1F) << 16);
```

---

Change the following lines in both __sse_setup.cpp__ and __sse_search.cpp__.

```C++
int N_keywords = 32621;//Number of meta-keywords **NOT KEYWORDS**
int N_max_ids = 1170;//Number of maximum ids for a meta-keyword (or max frequency)
```

`N_keywords` is the total number of keywords in the plain database. `N_max_ids` is the number of maximum ids where a keyword appears (or the maximum meta-keyword frequency). This is used to reserve buffer; need not be the exact, but should be larger than then the actual max frequency.

```C++
string widxdb_file = "../databases/meta_db6k.dat";//path to plain database
```

Set `widxdb_file` to the actual database files path. See the main README.md in the upper directory for available databases and their details. Addtional files are avaialble in the Google Drive directory.

```C++
unsigned int N_threads = 24;
```

Set `N_threads` to the number of threads to use. Since Bloom Filter hashes are individually computed using separate threads, we recommend a minimum 24 threads to use.

__Remember, you have to make these changes in both files for each database separately.__

---
**./database/util.h**

Change the follwing lines in [util.h](./database/utils.h) for a choice of plain database (note that, not the meta-databse information).

```C++
#define N_KW 6043 //keywords in plain database db6k.dat
#define N_ID 9690 //unique ids in plain database db6k.dat
```

This necessary to complete prior to build the `gen_db` executable for generating the meta-keyword database.

---
**sse_search.cpp**

```C++
std::string res_query_file = "./results/res_query.csv";
std::string res_id_file = "./results/res_id.csv";
std::string res_time_file = "./results/res_time.csv";
```

These three files store the actual queries, result ids, and timing information for each queries, respectively. These are overwritten each time the `sse_search` is executed.

```C++
unsigned int NUM_CLAUSE = 2;
```

This line defines the number of conjunctive clauses in CNF test vectors. The following flies (file paths) contain the different parameters of the test vectors.

```C++
std::string queryvector_file = "./test_vectors/HW2_query/HW2_hex_queryvector.csv";
std::string testvector_file = "./test_vectors/HW2_query/HW2_testvector.csv";
std::string freqvector_file = "./test_vectors/HW2_query/HW2_freqvector.csv";
std::string binvector_file = "./test_vectors/HW2_query/HW2_binvector.csv";
```

The [`HW2_hex_queryvector.csv`](./test_vectors/HW2_query/HW2_hex_queryvector.csv) file stores the actual indices of the keywords (in the sorted keyword space) present in the each clause. The [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv) file contains the actual meta-keywords for the CNF expression cluases. [`HW2_freqvector.csv`](./test_vectors/HW2_query/HW2_freqvector.csv) contains the frequency of the meta-keywords present in [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv). The bucket index for each meta-keyword in [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv) is stored in [`HW2_binvector.csv`](./test_vectors/HW2_query/HW2_binvector.csv) file. The supplied files are for two-keyword clauses. For other multi-keyword clauses, the test vectors can be generated using the codebase inside [`database`](./database/) subdirectory. Ensure that these files are read properly before executing search.

---

## Running an Experiment

### First step

Run [`db_util.py`](./db_util.py) to generate the [`db_kw_freq.csv`](./db_kw_freq.csv) file and necessary database information including number of keywords, total kw-id pairs etc. You need to change the database name if you want to use a different database and regenerate the above file.

### Clear Redis

Clear Redis and other temporary databases

```bash
$ redis-cli
127.0.0.1:6379> flushall
127.0.0.1:6379> save
127.0.0.1:6379> quit
$ rm -rf eidxdb.csv
$ rm -rf bloom_filter.dat
```

You can also run `make clean_all` to delete the `eidxdb.csv` and `bloom_filter.dat` files. This does not clear Redis though.

### Set the parameters

Obtain the database parameters from [`db_util.py`](./db_util.py) output and the source file parameters accordingly as stated above.

### Build the executables

Before doing this ensure that all parameters have been set properly and the paths are correct.

Execute the following make command to generated `sse_setup` and `sse_search`.

```bash
make all
```

Both `sse_setup` and `sse_search` can be generate through individual Makefile rules

```bash
make sse_setup
```

and 

```bash
make sse_search
```

The following command deletes the executable files. Generated databases including Redis, Bloom filter and results are not deleted.

```bash
make clean 
```
### Run the Executables

Run the setup routine first.

```bash
./sse_setup
```

When setup is complete, execute the search,

```bash
./sse_search
```

The test-vectors are supplied as stated earlier. New test vectors can be generated using the codebase inside the `database` subdirectory.

## Troubleshooting and Remarks

- Double check the system parameters. If even one mismatch is there, the result would be incorrect.
- Check if the input files are being read correctly (otherwise, the program would terminate abruptly)
- While executing the `sse_setup`, the Bloom filter is written to disk at the end. This requires a bit of time. Please be patient for this while using large databases.
- While executing the `sse_search` the Bloom filter is read into memory from disk first. This requires a bit of time. Please be patient for this while using large databases.
- Timings can widely vary depending upon the system configuration and load (and the version of packages used).