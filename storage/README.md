# Storage Overhead Estimation

The storage overhead is estimated from generated encrypted databases (and projected mathematically). In this subdirectory, the Python script [`storage_estimation.py`](./storage_estimation.py) estimates the storage overhead from the information using the database parameters obtained from experiments.

---

## Instructions to Run the Script

Execute the following command. This will print database information to the terminal.

```bash
python storage_estimation.py
```

Ensure that the meta-databases are generated using the [`db_util.py`](./db_util.py), which will also show relevant database information. To estimate the size of a new database _not already present_ in the script, first get the database statistics by running [`db_util.py`](./db_util.py) script and use the output information in the [`storage_estimation.py`](./storage_estimation.py) script.