#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Course {
public:
    string courseName;
    float grade;
    int creditHours;

    Course(const string& name, float grd, int credits)
        : courseName(name), grade(grd), creditHours(credits) {}
};


class Student {
public:
    string studentName;
    vector<Course> courses;

    Student(const string& name) : studentName(name) {}

     //Add a course to the student's course list
    void addCourse(const Course& course) {
        courses.push_back(course);
    }

    // Calculate the CGPA
    float calculateCGPA() const {
        float totalGradePoints = 0.0;
        int totalCredits = 0;

        for (const auto& course : courses) {
            totalGradePoints += course.grade * course.creditHours;
            totalCredits += course.creditHours;
        }

        if (totalCredits == 0) {
            return 0.0;
        }

        return totalGradePoints / totalCredits;
    }

     //Display student's course details and CGPA
    void displayDetails() const {
        cout << "\n\nStudent: " << studentName << "\n";
        cout << "Individual Grades and Credits:\n";
        for (const auto& course : courses) {
            cout << "Course: " << course.courseName << ", Grade: " << course.grade << ", Credit Hours: " << course.creditHours << "\n";
        }

        float cgpa = calculateCGPA();
        int totalCredits = 0;
        float totalGradePoints = 0.0;

        for (const auto& course : courses) {
            totalCredits += course.creditHours;
            totalGradePoints += course.grade * course.creditHours;
        }

        cout << "\nTotal Credits: " << totalCredits << "\n";
        cout << "Total Grade Points: " << totalGradePoints << "\n";
        cout << "CGPA: " << cgpa << "\n";
    }
};

 //Validate grade input
bool validateGrade(float grade) {
    return grade >= 0.0 && grade <= 4.0;
}

 //Validate credit hours input
bool validateCreditHours(int creditHours) {
    return creditHours > 0;
}

// Clear input buffer to handle invalid inputs
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

int main() {
    int numStudents;
    cout << "Enter the number of students: ";
    cin >> numStudents;

    if (numStudents <= 0) {
        cout << "Invalid number of students. Exiting.\n";
        return 1;
    }

    vector<Student> students;

    for (int i = 0; i < numStudents; ++i) {
        string studentName;
        cout << "Enter the name of student " << i + 1 << ": ";
        cin.ignore();
        getline(cin, studentName);

        Student student(studentName);

        int numCourses;
        cout << "Enter the number of courses for " << studentName << ": ";
        cin >> numCourses;

        if (numCourses <= 0) {
            cout << "Invalid number of courses. Exiting.\n";
            return 1;
        }

        for (int j = 0; j < numCourses; ++j) {
            string courseName;
            float grade;
            int creditHours;

            cout << "Enter the name of course " << j + 1 << ": ";
            cin.ignore();
            getline(cin, courseName);

            cout << "Enter the grade for " << courseName << " (0.0 - 4.0): ";
            while (!(cin >> grade) || !validateGrade(grade)) {
                cout << "Invalid grade. Enter a value between 0.0 and 4.0: ";
                clearInputBuffer();
            }

            cout << "Enter the credit hours for " << courseName << ": ";
            while (!(cin >> creditHours) || !validateCreditHours(creditHours)) {
                cout << "Invalid credit hours. Enter a positive value: ";
                clearInputBuffer();
            }

            student.addCourse(Course(courseName, grade, creditHours));
        }

        students.push_back(student);
    }

    for (const auto& student : students) {
        student.displayDetails();
       cout << "-------------------------------------\n";
    }

    return 0;
}
