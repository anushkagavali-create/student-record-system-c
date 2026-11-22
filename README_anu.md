# Student Record System (C)

A console program for managing student records. You can add, search, update, delete and list students. The records are stored in a text file so they are still there after the program is closed.

I made this to practise structs, arrays and file handling in C.

## Features

- Add a student (ID, name, age, course, marks)
- Search a student by ID
- Update a student's name, age, course or marks
- Delete a student (asks for confirmation first)
- Display all students in a table
- Records are saved to `students_anu.txt` after every change and loaded when the program starts
- Duplicate IDs are not allowed
- Input is checked: numbers must be numbers, age must be 15-100, marks must be 0-100, text can't be empty

## How to compile and run

You need a C compiler like gcc.

```
cd student-record-system-c-anu
gcc -Wall -o student_records student_records_anu.c
./student_records
```

On Windows (MinGW) the last line is `student_records.exe`.

Run the program from inside this folder, because `students_anu.txt` is looked for in the current directory. The folder includes 5 sample students. Delete `students_anu.txt` if you want to start empty.

## Menu

```
1. Add student
2. Search by ID
3. Update student
4. Delete student
5. Display all students
0. Exit
```

## Example output

```
ID       Name                   Age   Course                   Marks
-----------------------------------------------------------------------
1001     Aisha Khan             19    Computer Science         78.5
1002     Daniel Okafor          20    Information Technology   66.0
1003     Maria Lopez            18    Business Admin           82.0
1004     Liam Chen              21    Computer Science         71.5
1005     Sara Ahmed             19    Electronics              59.0

Total students: 5
```

## How the data is stored

One student per line, with the fields separated by `|`:

```
1001|Aisha Khan|19|Computer Science|78.5
```

I used `|` instead of a comma so names and courses can have spaces. Because of this the program doesn't allow the `|` character in text fields.

## Project files

```
student-record-system-c-anu/
├── student_records_anu.c   # all the code
├── students_anu.txt        # saved records (sample data included)
├── .gitignore
└── README_anu.md
```

## Limits

- Max 100 students (fixed size array)
- Names up to 49 characters, course up to 29 characters
- Records are kept in the order they were added

## What I learned

- How to use `fgets` and `strtol` to read input safely. `scanf` can leave a newline in the buffer and doesn't handle letters typed instead of numbers well
- Reading and writing files with `fopen`, `fgets` and `fprintf`
- To delete from an array you have to shift the other elements back
- Writing small functions (`find_student`, `read_int`) and reusing them everywhere saved a lot of repeated code
- Always check if `fopen` worked, because the file might not exist yet the first time

## Possible improvements

- Sort students by name or marks
- Use a linked list or `malloc` so there is no 100 student limit
- Search by name
