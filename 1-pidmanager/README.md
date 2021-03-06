# Pid Manager
An operating system's **pid manager** is responsible for managing process identifiers. When a process is first created, it is assigned a unique pid by the pid manager. The pid is returned to the pid manager when the process completes execution, and the manager may later reassign this pid. Process identifiers must be unique; no two active processes may have the same pid.

## Installation
- Run `make` or `make pidmang`
- Run executable `./pidmang`
- Remove object files with `make clean`
- Remove object files and executable with `make clean-full`