# Storage Overhead Estimation

The storage overhead is estimated from the actual encrypted database information obtained from generated encrypted databases (and mathematically). The Python script [`storage_estimation.py`](./storage_estimation.py) in this subdirectory estimates the storage overhead from the information using the database parameters obtianed from experiments.

---

## Instructions to Run the Script

Execute the following command. This will print database information to the terminal.

```bash
python storage_estimation.py
```

Ensure that the meta-databases are genereted using the [`db_util.py`](./db_util.py) which will also show relevant database information. To estimate the size of a new database _not already present_ in the script, first get the database statitistics by running [`db_util.py`](./db_util.py) script and use the output information in the [`storage_estimation.py`](./storage_estimation.py) script.