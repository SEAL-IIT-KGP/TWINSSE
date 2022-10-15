# Parameter Configuration File
This file contains the parameter configuration file details that are read and used by the executables generated in the subprojects. A single configuration file contains the parameters for a single database configuration (including both plain and the corresponding meta-database).

## Configuration File Format

```conf
<plain_database_file_path>
<number_of_threads_to_use_with_plain_database>
<number_of_keywords_in_the_plain_database>
<number_of_maximum_document_identifiers_for_a_keyword>
<Bloom_filter_size_as_a_power_of_2_value>
<number_of_bits_required_to_address_the_Bloom_filter>
<meta_database_file_path>
<number_of_threads_to_use_with_meta_database>
<number_of_metakeywords_in_the_meta_database>
<number_of_maximum_document_identifiers_for_a_metakeyword>
<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>
<number_of_bits_required_to_address_the_Bloom_filter_for_the_meta-database>
```

The configuration file in this directory for the database [supplied](../databases/) is shown here.

```conf
../databases/db6k.dat
24
6043
1809
131072
17
../databases/meta_db6k.dat
24
32621
1170
2097152
21
```

---

## Detailed Specification

Each line contains a parameter value/string. Do not put whitespace. Try to avoid whitespaces in filepaths.

__Line 1__: _(string)_ Path to plain database file containing the inverted index of keyword and documents identifiers

__Line 2__: _(integer)_ Number of threads to use while processing (or executing any setup or search) over the plain database. The conjunctive and the DNF experiments use this value.

__Line 3__: _(integer)_ Number of unique keywords in the plain database. This is necessary for allocating memory.

__Line 4__: _(integer)_ The maximum number of document identifiers for a keyword in the plain database. This is necessary for allocating memory.

__Line 5__: _(integer)_ The size of the Bloom filter (number of addresses/elements) for the plain database. The conjunctive and the DNF experiments use this value. This value is a power of 2, just larger than the total number of unique keyword-id pairs in the plain database.

__Line 6__: _(integer)_ Number of bits required to address the above Bloom filter. For example, if the Bloom filter size is $2^n$, then number of bits required to address is $n$.

__Line 7__: _(string)_ Path to the metakeyword database file containing the metakeywords and the associated document identifiers.

__Line 8__: _(integer)_ Number of threads to use while processing (while executing setup and search) metakeyword database. The disjunctive and the CNF experiments use this value.

__Line 9__: _(integer)_ Number of metakeywords in the metakeyword database. This is necessary for allocating memory.

__Line 10__: _(integer)_ The maximum number of document identifiers for a metakeyword in the metakeyword database. This is necessary for allocating memory.

__Line 11__: _(integer)_ The size of the Bloom filter (number of addresses/elements) for the metakeyword database. The disjunctive and the CNF experiments use this value. This value is a power of 2, just larger than the total number of unique metakeyword-id pairs in the metakeyword database.

__Line 12__: _(integer)_ Number of bits required to address the above Bloom filter. For example, if the Bloom filter size is $2^n$, then number of bits required to address is $n$.