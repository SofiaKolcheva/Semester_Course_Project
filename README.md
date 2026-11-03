# Semester_Course_Project
E-Student – Student Records Management (C)

A console application written in C for maintaining student records, modelled on the "E-Student" system of the Technical University of Sofia. All data is persisted in a binary file, and the program supports adding and removing students, managing semesters and grades, calculating averages, and browsing or searching the catalog.

Features
Add and remove students
Change the enrolled semester and individual grades (valid range 2–6)
Automatic calculation of the average grade from the entered grades
Notification when grades are missing and still expected
Catalog of all students
Catalog filtered by department
Search by faculty number
Persistent storage in a binary file, with automatic initialization on first launch
Student Record
Field	Description
Full name	Up to 30 characters, format: First Middle Last
Faculty number	Unique 10-digit number
Average grade	Real number, calculated from the entered grades
Enrolled semester	Integer, maximum 8
Enrollment date	Format DD.MM.YYYY
Department	One of: Обща, СУ, ИУТ, ИИТ, АЕЗ, БЕИ
Grades	Integer array with 3 grades per semester (e.g. 3rd semester = 9 grades)
Operations
Operation	Behavior
Add student	Validates every field before the record is stored
Remove student	Deletes the record; all following records shift one position back so numbering stays continuous
Change semester / grade	Grades must be in the range 2–6
Calculate average	Based on entered grades; if some are missing, a message states that grade entry is expected
Show catalog	Lists all students
Show catalog by department	Lists only students from the selected department
Search	Finds a student by faculty number
Technical Requirements
All processing operations are implemented as separate functions.
A specialized string input function accepts the string to fill and a buffer size (maximum number of characters), and guarantees that the string is correctly null-terminated.
Buffer overflow protection for all arrays and input.
Date validation: the entered date must be a valid calendar date in the DD.MM.YYYY format (leap years are not checked).
Initialization on first launch: the input data and the binary file are correctly initialized when the program starts for the first time.
Faculty number uniqueness is enforced when adding students.
Data Storage

Student records are stored in a binary file. Because each record has a fixed size, the whole list can be written and read in a single operation, which keeps file handling simple and fast.

Concepts Demonstrated
Structures and arrays of structures
Binary file I/O
String handling and safe input
Input validation (dates, ranges, uniqueness)
Modular program design with functions
