# TWINSSE

This repository contains the source code used in the experiments of the paper "TWo-IN-one-SSE: Fast, Scalable and Storage-Efficient Searchable Symmetric Encryption for Conjunctive and Disjunctive Boolean Queries" (accepted for publication at PETS 2023, Issue 1).

---

# Table of Contents

1. [Repository Organisation](#repoorg)
2. [System Requirements](#sysreq)
3. [Dependencies](#dependencies)
4. [Database Format](#dbformat)
5. [Primary Makefile](#primake)
6. [Steps to Run Experiments](#runexp)
7. [Troubleshooting and Remarks](#troubleremark)

## Repository Organisation <a name="repoorg"></a>

The repository is organised according to the TWINSSE experiments of the paper's main body, covering different queries, storage and precision evaluation. The following tree depicts the repository structure with individual subdirectories.

```
TWINSSE
|--README.md
|--requirements.system
|--Makefile
|____databases
|____conjunctive
|____disjunctive
|____cnf
|____dnf
|____precision
|____scripts
|____storage
|____test_vectors
```

---

## System Requirements <a name="sysreq"></a>

The framework requires heavy CPU-intensive computation and is intended for large multi-core servers with the minimal additional workload. We recommend the following system specifications.

- Intel(R) Xeon v5 or higher server grade (Xeon Gold or higher) multi-core 64-bit CPUs

- At least 24-48 physical CPU cores (with dual thread) with base frequency higher than 2 GHz. Should work with lower number of cores (8-16 physical cores), but the performance will get affected.

- 64-128 GB RAM or higher

- 256 GB PCIe Gen4 NVMe SSD or higher

- Ubuntu 18.04 LTS or 20.04 LTS 64 bit

- ISA support needed: SSE, SSE2, SSE3, SSE4, AVX2, AVX512, AES IntrinsicsA

- Minimum system load; if possible, no GUI and no other CPU/disk access-intensive task running


If you have less number of threads available than configured in the source files, you may receive error while running the executable. Please ensure that you have the set the correct number of threads, and the value of the associated parameters (many of which in turn depend on the number of threds, please see below).

---

## Dependencies <a name="dependencies"></a>

The following packages must be installed with global linkage

- POSIX pthread

- GNU GMP Multi-precision library with C++ wrappers (including MPFR)

- Redis base package, hiredis base package, and redis++ C++ wrapper

- Blake3 hash

- CryptoPP (or Crypto++)

[Pthread] `linux-libc-dev` v4.15.0-191.202, `libpthread-stubs0-dev` v0.3-4

[GMP] `libgmp-dev` v6.1.2, `libmpfr-dev` v4.0.1, `libmpfrc++-dev` v3.6.5

[Redis] `redis` v5:4.0.9, `redis-server` v5:4.0.9, `redis-tools` v5:4.0.9, `libhiredis-dev` v0.13.3-2.2

[redis++] Install from https://github.com/sewenew/redis-plus-plus

[Blake3] Supplied with the source code, execute `make lib` inside **blake** subdirectory

[CryptoPP] `libcrypto++-dev` v5.6.4, `libcrypto++-utils` v5.6.4, `libcrypto++6` v5.6.4

---

All dependencies (except __redis++__ and __Blake3__) can be installed by executing the following command in the TWINSSE home directory. Note that, it _requires sudo access_.

```bash
sudo apt update
cat requirements.system | xargs sudo apt -y install
```

**Please note that __redis++__ has to be installed manually after obtaining the source files from Github. This process requires sudo access, which is a key reason to segregate __redis++__ installation from the above dependency configuration process. We are working on automating the __redis++__ installation process (combining with the dependency installation process).**

---

## Database Format <a name="dbformat"></a>

The parsed databases are stored in csv format (with `.dat` extension) in the following way.
```csv
kw0, id00,id01,id02,....
kw1, id10,id11,id12,....
kw2, id20,id21,id22,....
```

This is applicable to meta-keyword databases too.

```csv
mkw0, id00,id01,id02,....
mkw1, id10,id11,id12,....
mkw2, id20,id21,id22,....
```


All kw and id values are 4 byte hex values (this can be changed if required). Please keep in mind where `.dat` and `.csv` extensions are used while modifying the source files. Otherwise, the files will not be read.

---

## Primary Makefile <a name="primake"></a>

The top-level Makefile contains the rules to build each component of the framework. The built executables need to be run manually. The Makefile contains the following rules.

- __all__  - Builds conjunctive, disjunctive, dnf, cnf and precision executables

- __clean__ - Cleans conjunctive, disjunctive, dnf, cnf and precision executables

- __clean_all__ - Cleans conjunctive, disjunctive, dnf, cnf and precision executables including generated database files and Redis database

- __blake_lib__ - Builds blake3 lib for conjunctive, disjunctive, dnf, and cnf subprojects

---

Each experiment has its own Makefile or helper scripts and separate README files. Please browse through the README files for more details.

- Databases [README](./databases/README.md)
- Conjunctive [README](./conjunctive/README.md)
- Disjunctive [README](./disjunctive/README.md)
- DNF [README](./dnf/README.md)
- CNF [README](./cnf/README.md)
- Precision [README](./precision/README.md)
- Storage [README](./storage/README.md)
- Scripts [README](./scripts/README.md)
- Test vectors [README](./test_vectors/README.md)

---

## Steps to Run Experiments <a name="runexp"></a>

Compile Blake3 library
```C++
make blake_lib
```

Set parameters for each experiment as listed above. Go to each specific directory for instructions in README files.

Generate the meta-keywords databases in project *subdirectories* (not from the top-level TWINSSE directory) by executing the following command in the `database` subdirectory (not the top-level [`databases`](./databases/) directory) for with the project directory (does not apply to conjunctive and DNF).

```C++
make all //within database subdirectory
```

Set the meta-keyword database parameters in the source files of the respective projects (see associated README files). This does not apply to conjunctive and DNF experiments.

If needed, generate the test vectors for particular experiments (a set of test vectors are already supplied inside the repository).

Double-check all parameters and file paths. Ensure all necessary databases (including test vectors) are generated/available.

Clean the projects by executing the following command in the TWINSSE directory. This clears the generated databases (including the temporary ones) and clears the Redis database too.

```bash
make clean_all
```

The use can manually flush the Redis server (storing the database) using the following sequence of commands (includes commands to execute in the Redis CLI).

```bash
$ redis-cli
127.0.0.1:6379> flushall
127.0.0.1:6379> save
127.0.0.1:6379> quit
```
Alternatively, without going into ``redis-cli`` prompt.

```bash
$ redis-cli flushall
$ redis-cli save
```

Build all executables in all project subdirectories (conjunctive, disjunctive, CNF and DNF).

```bash
make all
```

Go to each project subdirectory and execute `sse_setup` first and then `sse_search`.

```bash
./sse_setup
```

```bash
./sse_search
```

Details of each experiment and how to build each individually are available in respective project READMEs. Each project can be built by executing `make all` in the respective project subdirectory.

**Clean the project and Redis database before switching from one experiment to another!**

---

## Troubleshooting and Remarks <a name="troubleremark"></a>

The codebase has absolutely no error handling, does not follow specific production-grade software development practices, and may contain bugs. Please ensure all parameters are appropriately set and the required files are available (especially those which need to be generated first) at the specific locations. Additionally, we present stripped-down versions of the main database for quick testing and debugging.

Points related to specific experiments.

- In conjunctive and DNF (or CNF) experiments, the result size might be zero (or empty) for a high percentage of the queries. This is due to the sparse nature of the database, where a very large number of keywords have very low frequency, resulting in zero intersection. This is a correct search result with no id in the final result set.

- The search time can widely vary depending upon the system configuration, parameters and load.

- If program is aboruptly termninated, check file paths and parameter values are properly set.

---

## References

OXT SSE Algorithm

```
Cash, David, et al. "Highly-scalable searchable symmetric encryption with support for boolean queries." Annual cryptology conference. Springer, Berlin, Heidelberg, 2013.
```

Blake3 Hash Development Repository

https://github.com/BLAKE3-team/BLAKE3


Crypto++ library

https://cryptopp.com/


Redis++

https://github.com/sewenew/redis-plus-plus


---

## Cite

The full vesion is available at [IACR ePrint](https://eprint.iacr.org/2022/1096/). To cite, use the following bib entry.

```bib
@misc{cryptoeprint:2022/1096,
      author = {Arnab Bag and Debadrita Talapatra and Ayushi Rastogi and Sikhar Patranabis and Debdeep Mukhopadhyay},
      title = {TWo-IN-one-SSE: Fast, Scalable and Storage-Efficient Searchable Symmetric Encryption for Conjunctive and Disjunctive Boolean Queries},
      howpublished = {Cryptology ePrint Archive, Paper 2022/1096},
      year = {2022},
      note = {\url{https://eprint.iacr.org/2022/1096}},
      url = {https://eprint.iacr.org/2022/1096}
}
```
