#include <iostream>
#include<Windows.h>
#include<string.h>
#include <bits/stdc++.h>
#include<conio.h>
#include<fstream>
using namespace std;

// Define Employee structure
struct Employee {
    string name;
    int id;
    double salary;
    Employee* left;
    Employee* right;

    Employee(string n, int i, double s) : name(n), id(i), salary(s), left(nullptr), right(nullptr) {}
};

class EmployeeBST {
private:
    Employee* root;

    // Helper function to add an employee to the BST
    Employee* insert(Employee* node, string name, int id, double salary) {
        if (node == nullptr) {
            return new Employee(name, id, salary);
        }

        if (id < node->id) {
            node->left = insert(node->left, name, id, salary);
        } else if (id > node->id) {
            node->right = insert(node->right, name, id, salary);
        }

        return node;
    }

    // Helper function to write employee details to a file
    void writeEmployeeToFile(Employee* node, ofstream& outFile) const {
        if (node != nullptr) {
            writeEmployeeToFile(node->left, outFile);
            outFile <<"ID:"<< node->id << " NAME: " << node->name << " SALARY: " << node->salary << endl;
            writeEmployeeToFile(node->right, outFile);
        }
    }

    // Helper function to search for an employee by ID
    Employee* searchEmployeeByID(Employee* node, int id) const {
        if (node == nullptr || node->id == id) {
            return node;
        }

        if (id < node->id) {
            return searchEmployeeByID(node->left, id);
        } else {
            return searchEmployeeByID(node->right, id);
        }
    }

    Employee* updateEmployee(Employee* node, int id, double newSalary) {
        if (node == nullptr) {
            return nullptr; // Employee not found
        }

        if (id < node->id) {
            node->left = updateEmployee(node->left, id, newSalary);
        } else if (id > node->id) {
            node->right = updateEmployee(node->right, id, newSalary);
        } else {
            // Employee found, update details
            node->salary = newSalary;
            cout << endl;
            cout << "Employee details updated successfully.\n";
            cout << endl;
        }

        return node;
    }

    Employee* findMinValueNode(Employee* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Employee* deleteEmployee(Employee* node, int id) {
        if (node == nullptr) {
            return nullptr; // Employee not found
        }

        if (id < node->id) {
            node->left = deleteEmployee(node->left, id);
        } else if (id > node->id) {
            node->right = deleteEmployee(node->right, id);
        } else {
            // Employee found, perform deletion
            if (node->left == nullptr) {
                Employee* temp = node->right;
                cout << " Name: " << node->name << " Salary: " << node->salary << endl;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Employee* temp = node->left;
                cout << " Name: " << node->name << " Salary: " << node->salary << endl;
                delete node;
                return temp;
            }

            // Node with two children: Get the inorder successor (smallest
            // in the right subtree)
            Employee* temp = findMinValueNode(node->right);

            // Copy the inorder successor's content to this node
            node->id = temp->id;
            node->name = temp->name;
            node->salary = temp->salary;

            // Delete the inorder successor
            node->right = deleteEmployee(node->right, temp->id);
        }

        return node;
    }

public:
    EmployeeBST() : root(nullptr) {}

    // Function to add an employee to the BST
    void addEmployee() {
        int n;
        do {
            string name;
            int id;
            double salary;

            cout << "Enter employee name: ";
            cin >> name;

            cout << "Enter employee ID: ";
            cin >> id;

            cout << "Enter employee salary: ";
            cin >> salary;

            root = insert(root, name, id, salary);
            cout << endl;
            cout << "Employee added successfully.\n";
            cout << endl;
            cout << "Do you want to add more employee (1/0): ";
            cin >> n;
            cout << endl;
        } while (n == 1);
    }

    // Function to display all employees in-order
    void displayAllEmployees(Employee* node) const {
        if (node != nullptr) {
            displayAllEmployees(node->left);
            cout << "ID: " << node->id << " Name: " << node->name << " Salary: " << node->salary << endl;
            displayAllEmployees(node->right);
        }
    }

    // Function to display all employees (public interface)
    void displayAllEmployees() const {
        if (root == nullptr) {
            cout << endl;
            cout << "No employees in the system.\n";
            cout << endl;
        } else {
            cout << endl;
            cout << "Employee List:\n";
            cout << endl;
            displayAllEmployees(root);
        }
    }

    // Function to search for an employee by ID
    void searchEmployeeByID(int id) const {
        Employee* result = searchEmployeeByID(root, id);
        if (result == nullptr) {
            cout << endl;
            cout << "Employee with ID " << id << " not found.\n";
            cout << endl;
        } else {
            cout << endl;
            cout << "Employee found - ID: " << result->id << " Name: " << result->name << " Salary: " << result->salary << endl;
            cout << endl;
        }
    }

    void updateEmployeeDetails() {
        int id;
        double newSalary;

        cout << endl;
        cout << "Enter employee ID to update: ";
        cin >> id;

        cout << "Enter new salary: ";
        cin >> newSalary;

        root = updateEmployee(root, id, newSalary);
    }

    void deleteEmployeeById() {
        int id;

        cout << endl;
        cout << "Enter employee ID to delete: ";
        cin >> id;

        root = deleteEmployee(root, id);
        if (root != nullptr)
            cout << "No employee found.\n";
        else
        {
            cout << endl;
            cout << "Employee deleted successfully" << endl;
            cout << endl;
        }
    }

    // Function to save employee details to a file
    void saveEmployeesToFile(const string& filename) const {
        ofstream outFile(filename, ios::out);  // Use std::ofstream for an output file stream
        if (outFile.is_open()) {
            writeEmployeeToFile(root, outFile);
            outFile.close();
            cout << "Employee details saved to " << filename << " successfully." << endl;
        } else {
            cout << "Unable to open file " << filename << " for writing." << endl;
        }
    }

    void loadEmployeesFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            clearEmployees(); // Assuming you have a function to clear existing employee data
            readEmployeeFromFile(root, inFile);
            inFile.close();
            cout << "Employee details loaded from " << filename << " successfully." << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void clearEmployees() {
        clearEmployees(root);
        root = nullptr;
    }

    void clearEmployees(Employee* node) {
        if (node != nullptr) {
            clearEmployees(node->left);
            clearEmployees(node->right);
            delete node;
        }
    }

    void readEmployeeFromFile(Employee*& node, ifstream& inFile) {
        if (!inFile.eof()) {
            // Read employee details from the file
            int id;
            string name;
            double salary;
            inFile >> id >> name >> salary;

            // Create a new node for the employee
            node = new Employee(name, id, salary);
            node->left = node->right = nullptr;

            // Recursively read employee data for the left and right subtrees
            readEmployeeFromFile(node->left, inFile);
            readEmployeeFromFile(node->right, inFile);
        }
    }
};

int main() {
    cout << endl;
    cout << "                                            EMPLOYEE MANAGEMENT SYSTEM" << endl;
        cout<<"________________________________________________________________________________________________________________________"<<endl;
    char password[11], username[] = "rajankita@", us[11], ch;
    int i = 0;
    bool isincorrect = false;
    while (!isincorrect) {
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << "                                               ENTER USERNAME: ";
        cin >> us;
        if (strcmp(username, us) == 0) {
            cout << "                                               ENTER PASSWORD: ";
            isincorrect = true;
            for (i = 0; i < 10; i++) {
                ch = getch();
                if (ch == 13) {
                    password[i] = '\0';
                    break;
                } else {
                    password[i] = ch;
                    cout << "*";
                }
            }
            if (strcmp(password, "ankita") == 0) {
                system("COLOR 0e");
                system("cls");
                printf("\e[?251");
                SetConsoleCP(437);
                SetConsoleOutputCP(437);
                int bar1 = 177, bar2 = 219;
                cout << "\n\n\n\t\t\t\t LLoading...";
                cout << "\n\n\n\t\t\t\t";
                for (int i = 0; i < 25; i++)
                    cout << (char)bar1;
                cout << "\r";
                cout << "\t\t\t\t";
                for (int i = 0; i < 25; i++) {
                    cout << (char)bar2;
                    Sleep(150);
                }
                cout << "\n\t\t\t\t" << (char)1 << "!";
                system("Pause");

                EmployeeBST employeeBST;

                cout << endl;
                cout << endl;

                cout << "                                                     LOGIN SUCCESSFUL" << endl;
        cout<<"________________________________________________________________________________________________________________________"<<endl;

                cout << endl;

                int x;
                while (x) {
                    cout << "  ->Press 1 if want to add employee details "
                         << endl;
                    cout << "  ->Press 2 if want display employee details "
                         << endl;
                    cout << "  ->Press 3 if want to search employee by id "
                         << endl;
                    cout << "  ->Press 4 if want to update employee details "
                         << endl;
                    cout << "  ->Press 5 if want to delete employee details "
                         << endl;
                    cout << "  ->Press 6 to save data to the file"
                         << endl;
                    cout << "  ->Press 7 to load data to the file"
                         << endl;
                    cout << "  ->Else press 0 for exit \n"
                         << endl;

                    cout << endl;

                    int n;
                    cout << "  Enter your choice: " << endl;
                    cin >> n;

                    system("COLOR 0e");
                    printf("\e[?251");
                    SetConsoleCP(437);
                    SetConsoleOutputCP(437);
                    int bar1 = 177, bar2 = 219;
                    cout << "\n\n\n\t\t\t\t LLoading...";
                    cout << "\n\n\n\t\t\t\t";
                    for (int i = 0; i < 25; i++)
                        cout << (char)bar1;
                    cout << "\r";
                    cout << "\t\t\t\t";
                    for (int i = 0; i < 25; i++) {
                        cout << (char)bar2;
                        Sleep(150);
                    }
                    cout << "\n\t\t\t\t" << (char)1 << "!";
                    system("Pause");

                    switch (n) {
                        // Adding employees
                        case 1: {
                            employeeBST.addEmployee();
                            break;
                }

                // Displaying all employees
                case 2:
                    {
                        employeeBST.displayAllEmployees();
                        break;
                    }
                case 3:
                    {
                        int n=1;
                        while(n)
                        {
                            int searchID;
                            cout<<endl;
                            cout << "Enter employee ID to search: ";
                            cout<<endl;
                            cin >> searchID;
                            employeeBST.searchEmployeeByID(searchID);
                            cout<<endl;
                            cout<<"Do u want to continue searching(1/0): ";
                            cin>>n;
                            cout<<endl;

                        }
                        break;
                    }
                case 4:
                    {
                        while(n)
                        {
                            employeeBST.updateEmployeeDetails();
                            cout<<endl;
                            cout<<"Do u want to continue updating(1/0): ";
                            cout<<endl;
                            cin>>n;
                        }
                        break;
                    }
                 case 5:
                    {
                        while(n)
                        {
                            employeeBST.deleteEmployeeById();
                            cout<<endl;
                            cout<<"Do u want to continue deleting(1/0): ";
                            cout<<endl;
                            cin>>n;
                        }
                        break;
                    }
                 case 6:
                    {
                        employeeBST.saveEmployeesToFile("ds.txt");
                        break;
                    }
                    case 7:
                    {
                        employeeBST.loadEmployeesFromFile("ds.txt");
                        break;
                    }
                    default :
                        {
                           cout << endl;
        cout << "Invalid choice. Please enter a valid option." << endl;
                        }

                }
                cout<<endl;
                cout<<"Do u want to explore more options(1/0): ";
                cin>>x;
                cout<<endl;
        }
    }
                else
                {
                    cout << "                                              \nWrong password\n";
                }
        }
                else
                {
                    cout << "                                               Wrong username\n";
                }
            i++;
        }


    return 0;


}
