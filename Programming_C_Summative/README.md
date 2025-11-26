Assessment Outcomes
Students will be able to:
- Manipulate strings using C language
- Use function pointers properly in C language
- Design circuits and deploy the right program
- Implement multiple threads
- Manipulate file system using structures
- Write sound bash script

Instructions
- The assessment project will be completed on an individual basis
- The assessment will be graded out of 32pts
- Each student will be required to a video presenting their work
- There be no extension once the due date is passed
- The presentation should not exceed 8 minutes and should be complete and comprehensive enough
- Keep your github repo public until grades are published by the RO

Project 1. Smart Traffic Control and Monitoring System

Description:  Design and implement an embedded smart traffic control system that simulates an intelligent traffic intersection. The system should:

Detect vehicles (using IR sensors or push buttons as input),

Adjust signal timings dynamically based on real-time traffic density,

Record traffic data for later analysis, and

Handle multiple control tasks simultaneously using simulated concurrency.

This project reinforces dynamic data handling, real-time control, file logging, and concurrency concepts.

Key Features
Traffic Signal Management

Simulate at least two intersections with LEDs (Red, Yellow, Green).

Adjust timing dynamically based on “vehicle presence” detected by sensors (IR sensor or push button).

Dynamic Data Handling

Use structures and pointers to represent traffic lanes, signal timers, and vehicle counts.

Use dynamic memory allocation ( malloc(), realloc(), free() ) to manage varying traffic data (e.g., queues of waiting vehicles).

Logging and File Handling

Log traffic data (e.g., lane ID, wait time, number of vehicles, timestamp) to a file on the host computer (if using serial output redirected to a file).

Simulate “data persistence” by appending each cycle’s summary.

Simulated Multithreading

Use separate functions to represent concurrent processes (e.g., signal control, vehicle detection, and logging).

In Arduino (Tinkercad), simulate concurrency using millis() for non-blocking operations.

Error Handling and Robustness

Detect invalid input states (e.g., simultaneous green lights).

Handle dynamic memory safely (check for leaks or failed allocations).

System Interface

Provide a menu or serial interface to:

Display current signal states.

Show recent traffic statistics.

Trigger manual override of lights (for emergency mode).

 

Project 2: Automated System Monitoring Shell Script

Description

Develop a shell script that automatically monitors key system resources such as CPU usage, memory utilization, disk space, and running processes. The script should log system information, issue alerts when user-defined thresholds are exceeded, and provide an interactive command-line interface to view system status, adjust settings, and manage logs. This project reinforces core concepts in shell scripting, including process management, redirection, conditionals, loops, automation, and error handling.

Key Features:

System Monitoring

Use standard Linux utilities (top, free, df, ps) to display system information.

Extract and format the most relevant data (CPU %, memory used, and disk % full).

Automation and Threshold Alerts

Allow users to set threshold limits for CPU, memory, and disk usage.

Automatically log a warning or display an alert message when a threshold is crossed.

Logging and Redirection

Redirect all output to a log file with timestamps.

Maintain logs for recent system activity and alerts.

Support clearing or viewing the log file.

Scheduling / Periodic Execution

Allow the script to run periodically using a loop with sleep intervals (e.g., every 60 seconds).

Interactive Menu Interface

Provide a text-based menu with options such as:

View system status

Set alert thresholds

View logs

Clear logs

Exit

Error Handling and Robustness

Handle invalid user inputs or missing system commands gracefully.

Display clear and user-friendly error messages.

 

Project 3: Student Management System using Structures

Description:

Design and implement an advanced student management system in C that not only stores and manages student records but also performs data analytics and reporting. The system should use structures and dynamic memory allocation to handle records efficiently, store data persistently using file handling, and provide analytical functions such as computing averages, ranking students, and identifying top performers by course. Students should implement searching, sorting, and statistical analysis algorithms manually

Key Features:

Dynamic Data Management

Use structures to store student details:

ID, Name, Age, Course, Grades (array for multiple subjects), GPA, etc.

Use dynamic memory allocation (malloc, realloc, and free) to handle an unknown number of records at runtime.

Persistent Storage

Implement file handling for reading and writing records to files (binary or text format).

Ensure data integrity during save/load operations and handle file-related errors gracefully.

CRUD Operations

Support adding, displaying, updating, and deleting student records.

Ensure each operation updates both memory and file data correctly.

Search and Sorting Algorithms

Implement search by ID or name using binary search or linear search.

Sort records by GPA, name, or ID using a manual sorting algorithm (Bubble Sort, Insertion Sort, or Merge Sort).

Statistical and Analytical Features

Compute class average, median, and highest/lowest GPA.

Generate reports:

Top N students overall

Top student per course

Course-wise average GPA

Error Handling and Validation

Handle invalid inputs (e.g., negative age, invalid grade entry).

Validate unique student IDs.

User Interface

Create a menu-driven interface that allows the user to:

Manage student data (CRUD)

Search/sort records

Generate reports

Save and load from file

 

Project 4: Dynamic Math and Data Processing Engine (/5)

Description:

You will design and implement a Dynamic Math and Data Processing Engine in C.
The system will use function pointers to dynamically select and execute different mathematical and data processing operations at runtime. It will also use dynamic memory allocation (malloc, realloc, and free) to manage datasets whose size is determined during program execution.

This project gives you hands-on experience with function pointers, runtime function selection, dynamic memory management, and algorithmic operations in C.

Key Features:

Dynamic Function Dispatch

Use function pointers to dynamically select and execute operations at runtime based on user input.

Example operations:

Compute sum of elements

Compute average of elements

Find maximum and minimum values

Sort dataset in ascending or descending order

Search for a specific value in the dataset

Dynamic Memory Management

Use malloc, realloc, and free for memory management.

Allow users to add, modify, or delete data dynamically.

User-Defined Handlers

Provide a menu-driven interface allowing users to:

Input or modify datasets

Select operations dynamically

View results interactively

File I/O Integration

Allow users to load data from a file and save processed results to another file.

Demonstrate how function pointers can dispatch operations on file-based datasets.

Error Handling and Robustness

Handle invalid user input (non-numeric data, invalid menu choice, empty datasets).

Ensure all dynamically allocated memory is properly freed before exiting.

Code Modularity and Documentation

Structure code into multiple functions and use clear modular design.

Include detailed documentation explaining function pointer usage and memory management logic.

 

Project 5: Multi-threaded Web Scraper

Descriptions: Implement a multi-threaded web scraper that fetches data from multiple URLs in parallel without requiring thread synchronization. The scraper should take a list of URLs and download their HTML content concurrently using multiple threads.

Key Features:

Use POSIX threads (pthreads) to fetch multiple web pages in parallel
Save the downloaded content to separate text files (one file per thread)
Implement basic error handling (e.g., if a URL is unreachable)
Rubric
