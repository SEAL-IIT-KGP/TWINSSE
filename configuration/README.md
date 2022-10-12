# Parameter Configuration File
This file contains the parameter configuration file details that is read and used by the executables generated in the subprojects. A single configuration file contains the parameters for a single database configuration (including both plain and the corresponding meta-database).

## Configuration file layout

```conf
<plain_database_file_path>
<number_of_threads_to_use_with_plain_database>
<number_of_keywords_in_the_plain_database>
<number_of_maximum_document_identifiers_for_a_keyword>
<Bloom_filter_size_as_a_power_of_2_value>
<number_of_bits_required_to_address_the_Bloom_filter>
<meta_database_file_path>
<number_of_threads_to_use_with_meta_database>
<number_of_keywords_in_the_meta_database>
<number_of_maximum_document_identifiers_for_a_metakeyword>
<Bloom_filter_size_as_a_power_of_2_value_for_the_meta-database>
<number_of_bits_required_to_address_the_Bloom_filter_for_the_meta-database>
```

The configuration file for the database [supplied](../databases/) as provided in this directory is shown here.

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