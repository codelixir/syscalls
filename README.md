[![forthebadge](https://forthebadge.com/images/badges/fo-real.svg)](https://forthebadge.com)

# System Calls

The problem statements are given in [Assignment.pdf](Assignment.pdf).

Some sample files are given in `sample` directory with [`test.txt`](sample/test.txt) as the input. The outputs are generated using:

```bash
$ gcc q1.c
$ ./a.out test.txt
```
```bash
$ gcc q2.c
$ ./a.out test.txt 6 3
```
```bash
$ gcc q1.c
$ ./a.out test.txt > 3.txt
```

## Assumptions and Remarks

### Question 1

* In case of missing arguments, program exits. In case of additional arguments, program continues by ignoring extra arguments. In both cases, error messages are output to the terminal.
* If the directory `Assignment/` already exists, permissions aren't being overridden because the assignment mentioned to use the existing directory. Same for files; content is being overridden, permissions aren't.

### Question 2

* Same as question 1
* Last 2 arguments are single digit positive integers. (Throws error for non-integers and negative integers).
* The file is exactly divisible into the input number of parts

### Question 3

* Same as Q1 Point 1
* `Assignment/` directory, if exists, is present inside the working directory.