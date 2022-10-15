# CNF Expression Evaluation
## Overview

This subdirectory contains the codes to evaluate CNF expressions of the form

$q = (w_1\lor w_2)\land(w_3\lor w_4)$.

The main code has two top-level files - [`sse_setup.cpp`](./sse_setup.cpp) and [`sse_search.cpp`](./sse_search.cpp) responsible for executing the SSE setup and search routines, respectively. The [`Makefile`](./Makefile) for this codebase has the following main routines, `sse_setup` and `sse_search`, that generate two executables - `sse_setup` and `sse_search`, which execute the SSE setup and search routine. Additionally, a [`db_util.py`](./db_util.py) utility script generates the keyword-frequency record necessary for query processing.

---

## Setting Parameters in the Configuration and Source Files

Please esure that the necessary paratemeters are set properly in the configuration file [here](../configuration/) before building `sse_setup` and `sse_search`.

Set the following parameters in the configuration file.

```conf
.
.
.
<meta_database_file_path>
<number_of_threads_to_use_with_meta_database>
<number_of_metakeywords_in_the_meta_database>
<number_of_maximum_document_identifiers_for_a_metakeyword>
<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>
<number_of_bits_required_to_address_the_Bloom_filter_for_the_meta-database>
```

---

Set `<meta_database_file_path>` to the actual database files path. See the main README.md in the upper directory for available databases and their details. Additional files are available in the Google Drive directory.

Set `<number_of_threads_to_use_with_meta_database>` to the number of threads to use. Since Bloom Filter hashes are individually computed using separate threads, we recommend a minimum of 24 threads to use. The number of hashes used for Bloom filter indexing equals the number of threads used. This is set automatically in the code.

Set `<number_of_metakeywords_in_the_meta_database>` to the total number of meta-keywords in the meta-database. 

`<number_of_maximum_document_identifiers_for_a_metakeyword>` is the number of maximum ids where a keyword appears (or the maximum meta-keyword frequency). This is used to reserve buffer; it need not be exact but should be larger than the actual max frequency.

Change `<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>` to the power of two just above the total number of unique meta-keyword-document-id pairs in the meta-database. For example, if there are 1142496 unique meta-keyword-id pairs in the database, the power of two just above 1142496 is 2097152 = $2^{21}$. Hence, set this to 2097152. This is used to calculate the total Bloom filter size.

Set `<number_of_bits_required_to_address_the_Bloom_filter_for_the_meta-database>` to the number of bits required to specify the address range of the Bloom Filter (for metakeyword database). The number of bits to consider is derived from the above `<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>` value. Here `<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>` is $2^{21}$, that requires 21 bits. Hence, 21 bits are extracted from `bash` value to compute an index.

---
### Specific parameters in **./database/util.h**

This part is not automated yet; however, needs to be done only once for each database.

Change the follwing lines in [util.h](./database/utils.h) for a choice of plain database (note that, not the meta-databse information).

```C++
#define N_KW 6043 //keywords in plain database db6k.dat
#define N_ID 9690 //unique ids in plain database db6k.dat
```

This is necessary to complete prior to building the `gen_db` executable for generating the meta-keyword database.

---
### Specific parameters in **sse_search.cpp**

```C++
std::string res_query_file = "./results/res_query.csv";
std::string res_id_file = "./results/res_id.csv";
std::string res_time_file = "./results/res_time.csv";
```

These three files store the actual queries, result ids, and timing information for each query, respectively. These are overwritten each time the `sse_search` is executed.

```C++
unsigned int NUM_CLAUSE = 2;
```

This line defines the number of conjunctive clauses in CNF test vectors. The following files (file paths) contain the different parameters of the test vectors.

```C++
std::string queryvector_file = "./test_vectors/HW2_query/HW2_hex_queryvector.csv";
std::string testvector_file = "./test_vectors/HW2_query/HW2_testvector.csv";
std::string freqvector_file = "./test_vectors/HW2_query/HW2_freqvector.csv";
std::string binvector_file = "./test_vectors/HW2_query/HW2_binvector.csv";
```

The [`HW2_hex_queryvector.csv`](./test_vectors/HW2_query/HW2_hex_queryvector.csv) file stores the actual indices of the keywords (in the sorted keyword space) present in each clause. The [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv) file contains the actual meta-keywords for the CNF expression clauses. [`HW2_freqvector.csv`](./test_vectors/HW2_query/HW2_freqvector.csv) contains the frequency of the meta-keywords present in [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv). The bucket index for each meta-keyword in [`HW2_testvector.csv`](./test_vectors/HW2_query/HW2_testvector.csv) is stored in [`HW2_binvector.csv`](./test_vectors/HW2_query/HW2_binvector.csv) file. The supplied files are for two-keyword clauses. For other multi-keyword clauses, the test vectors can be generated using the codebase inside [`database`](./database/) subdirectory. Ensure that these files are read properly before executing a search.

---

## Running an Experiment

### First step

Run [`db_util.py`](./db_util.py) to generate the [`db_kw_freq.csv`](./db_kw_freq.csv) file, and necessary database information, including the number of keywords, total kw-id pairs etc. Change the database name to obtain the same information for a different database and regenerate the above file.

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

Alternatively, run `make clean_all` to delete the `eidxdb.csv` and `bloom_filter.dat` files. This also clears the Redis database; however, this does not remove the result files.

### Set the parameters

Obtain the database parameters from [`db_util.py`](./db_util.py) output and the source file parameters accordingly, as stated above.

### Build the executables

Before doing this, ensure that all parameters have been set correctly and the paths are correct.

Execute the following command to generate `sse_setup` and `sse_search`.

```bash
make all
```

Both `sse_setup` and `sse_search` can be built using individual Makefile rules

```bash
make sse_setup
```

and 

```bash
make sse_search
```

The following command deletes the executable files. Generated databases, including Redis, Bloom filter and results are not deleted.

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

The test vectors are supplied as stated earlier. New test vectors can be generated using the codebase inside the `database` subdirectory.

## Troubleshooting and Remarks

- Double-check the system parameters. If even one mismatch is there, the result would be incorrect.
- Check if the input files are being read correctly (otherwise, the program will terminate abruptly)
- While executing the `sse_setup`, the Bloom filter is written to disk at the end. This requires a bit of time. Please be patient with this while using large databases.
- While executing the `sse_search`, the Bloom filter is read into memory from the disk first. This requires a bit of time. Please be patient with this while using large databases.
- Timings can widely vary depending upon the system configuration and load (and the version of packages used).