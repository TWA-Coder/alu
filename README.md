# 📧 Student Email Manager in C

This project is a simple C program that demonstrates the use of *dynamic memory allocation* to manage email addresses for a student cohort. It reads email inputs, displays them, optimizes memory usage, and ensures all dynamically allocated memory is properly freed to avoid memory leaks.

---

## 🔧 Features

- Dynamically allocates memory for 10 student email addresses (up to 49 characters each)
- Accepts and stores user input securely using fgets
- Displays stored email addresses
- Efficiently reduces the cohort to 6 students using realloc
- Frees all allocated memory properly
- Organized and modular code with function encapsulation

---

##  Compile the program

gcc -Wall -o email_manager email_manager.c

./email_manager