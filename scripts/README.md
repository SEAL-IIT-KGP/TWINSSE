# Helper Scripts

This subdirectory hosts a number of helper scripts to obtain database parameters and system information.

---

Run the `gen_conjunctive_testvectors.py` to generate keyword frequency information file from which the search routines in conjunctive and DNF experiments generate random test vectors.

```bash
python gen_conjunctive_testvectors.py
```

---

Compile `get_sysinfor.cpp` and run the executable to find out the number of logical CPU cores available on the system.

```bash
g++ get_sysinfo.cpp -o get_sysinfo
```