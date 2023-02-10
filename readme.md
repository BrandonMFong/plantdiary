# Plant Diary

An central space where you can track your planting needs

## Software Achitecture 

### backend
Holds all user and plant data.  Each front end user will be represented as a session object to the backend.  This session object, if valid, shall be the main connection between the user and there data

### frontend
ideally driven by some wordpress application

## Design Choices
- Wrote background process with an cli : I wanted to learn how I can communicate between two processes using named pipes
- Used C/C++ language : My preferred language
- MySQL backedn database : I wanted to learn how to connect a C++ written tool with a MySQL database

