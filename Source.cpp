#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;
// to increase number of staff 
// add staff, attendance and application detail in respective txt files
// increase MAX_STAFF
// add prompt to prompt_set
string extractClassName(const std::string& fullName) {
    // Check if the prefix "class " exists and remove it
    const std::string prefix = "class ";
    if (fullName.find(prefix) == 0) {
        return fullName.substr(prefix.size());
    }
    return fullName; // Return as-is if no prefix
}

vector<vector<char>> prompt_set;
const int MAX_STAFF = 12;
const int MAX_EMPLOYEES = 100;
const int MAX_APPLICATIONS = 10; // for 1 staff member
const int MAX_LEAVES = 100;       // for 1 staff member
const int MAX_ATTENDANCE_RECORDS = 365; // for 1 staff member
const int MAX_WORK_HOURS = 12;
ostringstream report;

class Observer
{
public:
    virtual void update() = 0;
};

class Subject
{
    Observer* obs;

public:
    void add(Observer* obs)
    {
        this->obs = obs;
    }

    void notify()
    {
        obs->update();
    }
};

// Employee Interface for Main
class IEmployee
{
public:
    virtual void display() const = 0;
    virtual ~IEmployee() {}
};

class Employee : public IEmployee
{
protected:
    string name;
    int id;

public:
    Employee(const string& name, int id) : name(name), id(id) {}

    int get_id()
    {
        return id;
    }

    string get_name()
    {
        return name;
    }

    virtual void display() const
    {
        cout << id << "\t||\t" << name;
    }

    virtual ~Employee() {}
};

class Staff;

// Application Interface for Supervisor/Director
class IApplication_for_Manager
{
public:
    virtual bool approve(Staff* s, bool action = true) = 0;
};

class Director : public Employee
{
    IApplication_for_Manager* iAd[MAX_APPLICATIONS];
    int interfaceCNT;

public:
    Director(const string& name, int id) : Employee(name, id), interfaceCNT(0)
    {
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iAd[i] = NULL;
    }

    void set_applicationInterface(IApplication_for_Manager* i)
    {
        if (interfaceCNT < MAX_APPLICATIONS)
            iAd[interfaceCNT++] = i;
    }

    void resetInterfaceCNT()
    {
        interfaceCNT = 0;
    }

    bool approve(int applicationNumber, Staff* s, bool action = true) const
    {
        if (iAd[applicationNumber]) 
        {
            iAd[applicationNumber]->approve(s, action);
            return true;
        }
        return false;
    }

    ~Director()
    {
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iAd[i] = NULL; // Clear pointers
    }
};

class Supervisor : public Employee
{
    IApplication_for_Manager* iAs[MAX_APPLICATIONS];
    int interfaceCNT;

public:
    Supervisor(const string& name, int id) : Employee(name, id), interfaceCNT(0)
    {
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iAs[i] = NULL;
    }

    void set_applicationInterface(IApplication_for_Manager* i)
    {
        if (interfaceCNT < MAX_APPLICATIONS)
            iAs[interfaceCNT++] = i;
    }

    void resetInterfaceCNT()
    {
        interfaceCNT = 0;
    }

    bool approve(int applicationNumber, Staff* s, bool action = true) const
    {
        if (iAs[applicationNumber]) 
        {
            iAs[applicationNumber]->approve(s, action);
            return true;
        }
        return false;
    }

    ~Supervisor()
    {
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iAs[i] = NULL; // Clear pointers
    }
};

// Attendance Interface for Guard
class IAttendance_for_Guard
{
public:
    virtual void set_clockIn(double hr, const string& date) = 0;
    virtual void set_clockOut(double hr) = 0;
};

class Guard : public Employee
{
    IAttendance_for_Guard* iAg;

public:
    Guard(const string& name, int id) : Employee(name, id), iAg(NULL) {}

    void set_attendanceInterface(IAttendance_for_Guard* i)
    {
        iAg = i;
    }

    void set_clockIn(double hr, const string& date)
    {
        if (iAg)
            iAg->set_clockIn(hr, date);
    }

    void set_clockOut(double hr)
    {
        if (iAg)
            iAg->set_clockOut(hr);
    }

    ~Guard()
    {
        iAg = NULL; // Nullify the pointer for safety
    }
};

// Attendance Interface for Staff
class IAttendance_for_Staff
{
public:
    virtual void displayAttendance() const = 0;
};

class Leave;

// Application Interface for Staff
class IApplication_for_Staff
{
public:
    virtual void applyForLeave(const string& writtenDate, Leave* le) = 0;
    virtual void displayLeave() const = 0;
};

class Staff : public Employee, public Observer
{
    IAttendance_for_Staff* iAtts;
    IApplication_for_Staff* iApps[MAX_APPLICATIONS];
    int curr_causalBalance;
    int curr_earnedBalance;
    int interfaceCNT_app;
    int paidCred;

public:
    Staff(const string& name, int id) : Employee(name, id), curr_causalBalance(15), curr_earnedBalance(21), interfaceCNT_app(0), paidCred(0)
    {
        iAtts = NULL;
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iApps[i] = NULL;
    }

    void set_attendanceInterface(IAttendance_for_Staff* i)
    {
        iAtts = i;
    }

    void set_applicationInterface(IApplication_for_Staff* i2)
    {
        if (interfaceCNT_app < MAX_APPLICATIONS)
            iApps[interfaceCNT_app++] = i2;
    }

    void writeApplication(int applicationNumber, const string& writtenDate, Leave* le)
    {
        iApps[applicationNumber]->applyForLeave(writtenDate, le);
    }

    void viewAttendance()
    {
        iAtts->displayAttendance();
    }

    void viewApplication()
    {
        for (int i = 0; i < interfaceCNT_app; i++)
            iApps[i]->displayLeave();
    }

    void update()
    {
        cout << "Your Leave Status: ";
    }

    void upd_casualBalance(int n)
    {
        curr_causalBalance -= n;
    }

    void upd_earnedBalance(int n)
    {
        curr_earnedBalance -= n;
    }

    void incrementCred(int n)
    {
        paidCred += n;
    }

    int get_paidCred()
    {
        return paidCred;
    }

    IAttendance_for_Staff* get_iAtts() const
    {
        return iAtts;
    }

    IApplication_for_Staff** get_iApps()
    {
        return iApps;
    }

    int get_interfaceCNT_app() const
    {
        return interfaceCNT_app;
    }

    int get_casualBalance() const
    {
        return curr_causalBalance;
    }

    int get_earnedBalance() const
    {
        return curr_earnedBalance;
    }

    void display() const
    {
        Employee::display();
    }

    ~Staff()
    {
        iAtts = NULL; // Nullify the attendance pointer for safety
        for (int i = 0; i < MAX_APPLICATIONS; i++)
            iApps[i] = NULL; // Nullify application pointers for safety
    }
};

class State;

class Leave : public Subject
{
protected:
    State* st;

public:
    string startDate, endDate, reason, address;
    bool active;

    Leave(const string& startDate, const string& endDate, const string& reason, const string& address);

    void set_state(State* s)
    {
        st = s;
    }

    State* get_state()
    {
        return st;
    }

    virtual void ret()
    {
        notify();
    }

    virtual void display()
    {
        report << "\t" << startDate << "\t" << endDate << "\t" << reason << "\t" << address << "\n";
    }

    virtual ~Leave() {}
};

class State
{
protected:
    Leave* l;

public:
    State(Leave* le) : l(le) {}
    virtual void pending() = 0;
    virtual void rejected() = 0;
    virtual void approved() = 0;
};

class Pending : public State
{
public:
    Pending(Leave* le) : State(le) {}

    void pending()
    {
        cout << "~";
    }

    void rejected();
    void approved();
};

class Rejected : public State
{
public:
    Rejected(Leave* le) : State(le) {}

    void pending()
    {
        l->set_state(new Pending(l));
    }

    void rejected()
    {
        cout << "~";
    }

    void approved();
};

class Approved : public State
{
public:
    Approved(Leave* le) : State(le) {}

    void pending()
    {
        l->set_state(new Pending(l));
    }

    void rejected()
    {
        l->set_state(new Rejected(l));
    }

    void approved()
    {
        cout << "~";
    }
};

void Pending::rejected()
{
    l->set_state(new Rejected(l));
}

void Pending::approved()
{
    l->set_state(new Approved(l));
}

void Rejected::approved()
{
    l->set_state(new Approved(l));
}

Leave::Leave(const string& startDate, const string& endDate, const string& reason, const string& address)
    : startDate(startDate), endDate(endDate), reason(reason), address(address)
{
    st = new Pending(this);
    active = false;
}

class CasualLeave : public Leave
{
public:
    CasualLeave(const string& startDate, const string& endDate, const string& reason, const string& address)
        : Leave(startDate, endDate, reason, address) {}

    void ret()
    {
        Leave::ret();
        cout << extractClassName(typeid(*st).name()) << "!\n";
    }

    void display()
    {
        report << "Casual";
        Leave::display();
    }

    ~CasualLeave() {}
};

class EarnedLeave : public Leave
{
public:
    EarnedLeave(const string& startDate, const string& endDate, const string& reason, const string& address)
        : Leave(startDate, endDate, reason, address) {}

    void ret()
    {
        Leave::ret();
        cout << extractClassName(typeid(*st).name()) << "!\n";
    }

    void display()
    {
        report << "Earned";
        Leave::display();

    }
    ~EarnedLeave() {}
};

class OfficialLeave : public Leave
{
public:
    OfficialLeave(const string& startDate, const string& endDate, const string& reason, const string& address)
        : Leave(startDate, endDate, reason, address) {}

    void ret()
    {
        Leave::ret();
        cout << extractClassName(typeid(*st).name()) << "!\n";

    }

    void display()
    {
        report << "Official";
        Leave::display();

    }

    ~OfficialLeave() {}
};

class UnpaidLeave : public Leave
{
public:
    UnpaidLeave(const string& startDate, const string& endDate, const string& reason, const string& address)
        : Leave(startDate, endDate, reason, address) {}

    void ret()
    {
        Leave::ret();
        cout << extractClassName(typeid(*st).name()) << "!\n";
    }

    void display()
    {
        report << "Unpaid";
        Leave::display();
    }

    ~UnpaidLeave() {}
};

class Attendance : public IAttendance_for_Guard, public IAttendance_for_Staff
{
    struct AttendanceRecord
    {
        double hoursWorked = 0.0;
        string date;
    };
    double clockIn;
    AttendanceRecord records[MAX_ATTENDANCE_RECORDS];
    int recordCount;

    void calculateHoursWorked(double hrs)
    {
        double hoursWorked = hrs - clockIn;
        if (hoursWorked > MAX_WORK_HOURS)
            hoursWorked = MAX_WORK_HOURS;
        records[recordCount++].hoursWorked = hoursWorked;
    }
public:
    Attendance() : clockIn(0), recordCount(0) {}

    void set_clockIn(double hr, const string& date)
    {
        if (recordCount < MAX_ATTENDANCE_RECORDS)
        {
            records[recordCount].date = date;
            clockIn = hr;
        }
    }

    void set_clockOut(double hr)
    {
        calculateHoursWorked(hr);
    }

    int get_recordCount()
    {
        return recordCount;
    }

    double calculateTotalHoursWorked() const
    {
        double totalHours = 0;
        for (int i = 0; i < recordCount; ++i)
            totalHours += records[i].hoursWorked;
        return totalHours;
    }

    void displayAttendance() const
    {
        report << "Date\t\t||\tHours Worked\n";
        report << "================||========================\n";
        for (int i = 0; i < recordCount; ++i)
            report << records[i].date << "\t||\t" << records[i].hoursWorked << " hours" << endl;
    }

    ~Attendance() {}
};

class Application : public IApplication_for_Staff, public IApplication_for_Manager
{
    Leave* l;
    string leaveType;
    string startDate;
    string endDate;
    string reason;
    string leaveAddress;
    string writtenDate;
    int duration;
    bool isFiled;

    bool enoughBalance(Staff* staff)
    {
        int cB = staff->get_casualBalance();
        int eB = staff->get_earnedBalance();
        if (leaveType == "class CasualLeave" && cB > 0 && duration <= 4 && cB - duration >= 0)
            return true;
        else if (leaveType == "class EarnedLeave" && eB > 0 && eB - duration >= 0)
            return true;
        else if (leaveType == "class UnpaidLeave" || leaveType == "class OfficialLeave")
            return true;
        else
            return false;
    }
public:
    Application()
    {
        l = NULL;
        isFiled = false;
        leaveType = startDate = endDate = reason = leaveAddress = writtenDate = "";
    }

    int calculateDaysBetween(const string& startDate, const string& endDate)
    {
        // Variables to hold the parsed date components
        int startYear, startMonth, startDay;
        int endYear, endMonth, endDay;

        // Use sscanf_s to parse the dates (requires width specifiers for each part)
        sscanf_s(startDate.c_str(), "%4d-%2d-%2d", &startYear, &startMonth, &startDay);
        sscanf_s(endDate.c_str(), "%4d-%2d-%2d", &endYear, &endMonth, &endDay);

        // Create tm structs for each date
        struct tm startTm = { 0 }, endTm = { 0 };
        startTm.tm_year = startYear - 1900; // Year since 1900
        startTm.tm_mon = startMonth - 1;    // Month [0-11]
        startTm.tm_mday = startDay;         // Day of the month [1-31]

        endTm.tm_year = endYear - 1900;
        endTm.tm_mon = endMonth - 1;
        endTm.tm_mday = endDay;

        // Convert to time_t
        time_t startTime = mktime(&startTm);
        time_t endTime = mktime(&endTm);

        // Calculate the difference in seconds, then convert to days
        double secondsDifference = difftime(endTime, startTime);
        int daysDifference = secondsDifference / (60 * 60 * 24);

        return daysDifference;
    }

    void set_leaveInterface(Leave* i)
    {
        l = i;
    }

    void applyForLeave(const string& writtenDate, Leave* le)
    {
        isFiled = true;
        this->leaveType = (typeid(*le).name());
        this->startDate = le->startDate;
        this->endDate = le->endDate;
        this->reason = le->reason;
        this->leaveAddress = le->address;
        this->writtenDate = writtenDate;
        duration = calculateDaysBetween(startDate, endDate);
    }

    bool approve(Staff* s, bool action = true)
    {
        if (action && enoughBalance(s))
        {
            l->get_state()->approved();
            upd_balance(s);
        }
        else
        {
            l->get_state()->rejected();
        }
        return true;
    }

    void upd_balance(Staff* s, int d = 0, string str = "")
    {
        if (d != 0)
            duration = d;
        if (str != "")
            leaveType = str;
        if (leaveType == "class CasualLeave")
            s->upd_casualBalance(duration);
        else if (leaveType == "class EarnedLeave")
            s->upd_earnedBalance(duration);
        else if (leaveType == "class OfficialLeave")
        {
            s->incrementCred(duration);
        }
    }

    bool get_isFiled()
    {
        return isFiled;
    }

    Leave* getLeave()
    {
        return l;
    }

    void displayLeave() const
    {
        if (l->active)
            l->display();
    }

    ~Application()
    {
        delete l; // Ensure memory allocated to 'l' is freed
    }
};

class LeaveFactory 
{
public:
    static Leave* createLeave(const string& leaveType, const string& startDate, const string& endDate, const string& reason, const string& address) 
    {
        if (leaveType == "Casual")
            return new CasualLeave(startDate, endDate, reason, address);
        else if (leaveType == "Earned")
            return new EarnedLeave(startDate, endDate, reason, address);
        else if (leaveType == "Unpaid")
            return new UnpaidLeave(startDate, endDate, reason, address);
        else if (leaveType == "Official")
            return new OfficialLeave(startDate, endDate, reason, address);
        else
            throw invalid_argument("Invalid leave type");
    }
};

class fileHandling
{
    struct attInfo
    {
        int clockIn = 0;
        int clockOut = 0;
        string date;
    };
    struct leaveInfo
    {
        string leaveType;
        string startDate;
        string endDate;
        string reason;
        string leaveAddress;
        string dateWritten;
    };
    struct empInfo
    {
        string name;
        string id;
        vector<attInfo> attendanceRecords;
        vector<leaveInfo> leaveApplications;
    };
    vector<empInfo> employees;
public:
    void readEmployeeData(const string& filename)
    {
        ifstream file(filename);  // Open the file for reading

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string temp;
                empInfo emp;

                // Skip the index (e.g., "1.", "2.", etc.)
                getline(ss, temp, '.');

                // Read name and ID
                getline(ss, emp.name, '-');
                ss >> temp;  // Skip the "ID:"
                ss >> emp.id;  // Read the ID

                employees.push_back(emp);  // Store the employee data
            }
            file.close();
            //cout << "Employee data has been read from " << filename << "." << endl;
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    void readAttendanceData(const string& filename)
    {
        ifstream file(filename);  // Open the file for reading

        if (file.is_open()) {
            string line;
            size_t employeeIndex = 0;  // To track which employee we're adding attendance for

            while (getline(file, line)) {
                // Check if the line contains employee name and ID (start with index or name)
                if (line.find('.') != string::npos) {
                    // Parse employee index, name, and ID
                    stringstream ss(line);
                    string temp;
                    string id;

                    // Read the employee name and ID
                    getline(ss, temp, '-');
                    ss >> temp;  // Skip the "ID:"
                    ss >> id;  // Read the ID

                    // Find the employee in the vector
                    for (size_t i = 0; i < employees.size(); ++i) {
                        if (employees[i].id == id) {
                            employeeIndex = i;  // Set the correct employee to associate attendance with
                            break;
                        }
                    }
                }
                // If the line contains attendance data (no index or name, just time and date)
                else if (!line.empty() && line.find(' ') != string::npos) {
                    stringstream ss(line);
                    attInfo att;

                    // Parse clock-in, clock-out, and date from the attendance record
                    if (ss >> att.clockIn >> att.clockOut >> att.date) {
                        // Add this attendance to the correct employee
                        employees[employeeIndex].attendanceRecords.push_back(att);
                    }
                    else {
                        cout << "Error parsing line: " << line << endl;
                    }
                }
            }

            file.close();
            //cout << "Attendance data has been read from " << filename << "." << endl;
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    void readLeaveApplications(const string& filename) {
        
        size_t old_vectorSize = employees.size();

        ifstream file(filename);

        if (file.is_open()) {
            string line;
            empInfo currentEmployee;
            bool readingLeaveData = false;

            while (getline(file, line)) {
                // Check if the line contains employee name and ID
                if (line.find(" - ID:") != string::npos) {
                    // If there's an existing employee, push the current data to employees vector
                    if (!currentEmployee.name.empty()) {
                        employees.push_back(currentEmployee);  // Add current employee to the vector
                    }

                    // Reset currentEmployee completely
                    currentEmployee = empInfo();

                    // Parse employee ID and name
                    size_t idPos = line.find(" - ID:") + 6;  // Skip " - ID:"
                    currentEmployee.id = line.substr(idPos, 4);  // Assuming ID is 4 characters long
                    currentEmployee.name = line.substr(0, idPos - 7);  // Extract the name part (before " - ID:")

                    readingLeaveData = true;  // Start reading leave data for this employee
                }
                // If we're reading leave application data (skip empty lines)
                else if (readingLeaveData && !line.empty()) {
                    stringstream ss(line);
                    leaveInfo leaveApp;

                    // Manually split the line into words (split by space)
                    vector<string> words;
                    string word;
                    while (ss >> word) {
                        words.push_back(word);
                    }

                    // Check if the words vector has the correct number of parts
                    if (words.size() >= 6) {
                        // Assign the parsed data to the leaveApp structure
                        leaveApp.leaveType = words[0];
                        leaveApp.startDate = words[1];
                        leaveApp.endDate = words[2];
                        leaveApp.reason = words[3];
                        leaveApp.leaveAddress = words[4];
                        leaveApp.dateWritten = words[5];

                        // Add to current employee's leave applications
                        currentEmployee.leaveApplications.push_back(leaveApp);
                    }



                    else {
                        cout << "Warning: Malformed leave application line: " << line << endl;
                    }
                }
            }


            // Add the last employee after reading all lines
            if (!currentEmployee.name.empty()) {
                employees.push_back(currentEmployee);
            }
            size_t new_vectorSize = employees.size();
            if (new_vectorSize > old_vectorSize) {  // Ensure there are enough employees to perform the copy
                for (size_t i = 0; i < old_vectorSize; ++i) {
                    if (i + old_vectorSize < new_vectorSize) {  // Check bounds to prevent out-of-range errors
                        employees[i].leaveApplications = employees[i + old_vectorSize].leaveApplications;
                    }
                }
            }
            employees.erase(employees.begin() + old_vectorSize, employees.end());
            file.close();
            //cout << "Leave application data has been read from " << filename << "." << endl;
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    void save(int fileNo, bool flag = false, int id = -999)
    {
        if (flag)
        {
            string txt[] = { "reports/1/LowAttendanceEmployees.txt", "reports/2/OutstandingLeaves.txt",
                "reports/3/Attendance_" + std::to_string(id) + ".txt", "reports/4/LeaveDetails_" + std::to_string(id) + ".txt" };
            std::ofstream file(txt[fileNo]);
            file << report.str();
            file.close();
        }
        report.str("");
        report.clear();
    }

    vector<empInfo> get_empsVector()
    {
        return employees;
    }
};

class MainSystem
{
    Leave* leaves[MAX_LEAVES];
    IEmployee* employees[MAX_EMPLOYEES];
    int employeeCount;
    int leaveCount;
    fileHandling* fd;

    Staff* doesEmpExist(int empID)
    {
        for (int i = 0; i < employeeCount; i++)
        {
            Staff* staff = dynamic_cast<Staff*>(employees[i]);
            if (staff && staff->get_id() == empID)
                return staff;
        }
        return NULL;
    }
public:
    MainSystem()
    {
        for (int i = 0; i < MAX_LEAVES; i++)
            leaves[i] = NULL;
        for (int i = 0; i < MAX_EMPLOYEES; i++)
            employees[i] = NULL;
        employeeCount = leaveCount = 0;

        fd = new fileHandling();
        fd->readEmployeeData("Employee.txt");
        fd->readAttendanceData("Attendance.txt");
        fd->readLeaveApplications("Application.txt");
    }

    void addEmployee(IEmployee* employee)
    {
        if (employeeCount < MAX_EMPLOYEES)
            employees[employeeCount++] = employee;
    }

    void showLowAttendanceEmployees(double thresholdPercentage, bool saveToFile = false)
    {

        report << "ID\t||\tName\t||\tAttendance\n";
        report << "========||==============||======================\n";
        const double expectedDailyHours = 8.0;

        for (int i = 0; i < employeeCount; i++)
        {
            Staff* st = dynamic_cast<Staff*>(employees[i]);
            if (st)
            {
                IAttendance_for_Staff* Iatt = st->get_iAtts();
                Attendance* att = dynamic_cast<Attendance*>(Iatt);
                int totalDays = att->get_recordCount();

                if (totalDays > 0)
                {
                    int extra = st->get_paidCred() * 8;
                    double totalHoursWorked = att->calculateTotalHoursWorked();
                    double expectedTotalHours = totalDays * expectedDailyHours;
                    double attendancePercentage = ((totalHoursWorked + extra) / (expectedTotalHours + extra)) * 100;

                    if (attendancePercentage < thresholdPercentage)
                    {
                        report << st->get_id() << "\t||\t" << st->get_name() << "\t||\t" << attendancePercentage << "%" << std::endl;
                    }
                }
            }
        }

        // Display report to the console
        std::cout << report.str() << endl;
        fd->save(0, saveToFile);

    }

    void showOutstandingLeaves(bool saveToFile = false)
    {

        report << "ID\t||\tName\t||\t\tPoint of Concern\n";
        report << "========||==============||===============================================\n";

        for (int i = 0; i < employeeCount; i++)
        {
            Staff* staff = dynamic_cast<Staff*>(employees[i]);
            if (staff)
            {
                IApplication_for_Staff** Iapp = staff->get_iApps();
                int numberApplication_written = staff->get_interfaceCNT_app();
                for (int j = 0; j < numberApplication_written; j++)
                {
                    Application* app = dynamic_cast<Application*>(Iapp[j]);
                    //bool approved = app->get_isApproved();
                    Leave* l = app->getLeave();
                    bool isFiled = app->get_isFiled();

                    State* st = l->get_state();
                    if (l->active)
                    {
                        if (typeid(*l) == typeid(CasualLeave) && !isFiled)
                            report << staff->get_id() << "\t||\t" << staff->get_name() << "\t||\t\t" << "Casual Leave Not Filed\n";
                        else if (typeid(*l) == typeid(EarnedLeave) && typeid(*st) != typeid(Approved))
                            report << staff->get_id() << "\t||\t" << staff->get_name() << "\t||\t\t" << "Earned Leave Not Approved\n";
                    }
                }
            }
        }

        // Display report to the console
        std::cout << report.str() << endl;
        fd->save(1, saveToFile);

    }

    void showAttendance(int id, bool saveToFile = false)
    {

        Staff* staff = doesEmpExist(id);
        if (staff)
        {
            report << "Attendance details for " << staff->get_name() << " (ID: " << id << "):" << std::endl;
            staff->viewAttendance();  // Assumes viewAttendance prints details to console
        }
        else
        {
            report << "Employee with ID " << id << " not found or does not have attendance records." << std::endl;
        }

        // Display report to console
        std::cout << report.str() << endl;
        fd->save(2, saveToFile, id);

    }

    void showLeaveDetails(int id, bool saveToFile = false)
    {
        Staff* staff = doesEmpExist(id);
        if (staff)
        {
            report << "Leave details for " << staff->get_name() << " (ID: " << id << "):" << std::endl;
            staff->viewApplication();  // Assumes viewApplication prints details to console
            report << "Remaining Casual / Earned Balance: ";
            report << staff->get_casualBalance() << " / " << staff->get_earnedBalance() << std::endl;
        }
        else
        {
            report << "Employee with ID " << id << " not found or does not have attendance records." << std::endl;
        }

        // Display report to console
        std::cout << report.str() << endl;
        fd->save(3, saveToFile, id);

    }

    Leave* chooseLeave(const string& leaveType, const string& startDate, const string& endDate, const string& reason, const string& address)
    {
        // Use the LeaveFactory to create the leave
        Leave* l = NULL;
        l = LeaveFactory::createLeave(leaveType, startDate, endDate, reason, address);
        if (l)
        {
            leaves[leaveCount++] = l;  // Store the leave
        }
        return l;
    }

    void forward(Director* dir, Supervisor* sup, Application* app, int appNumber, Staff* s, bool action = true)
    {
        //Director* dir = dynamic_cast<Director*>(employees[employeeCount - 1]);
        ////dir->set_applicationInterface(app);
        //Supervisor* sup = dynamic_cast<Supervisor*>(employees[employeeCount - 2]);
        //sup->set_applicationInterface(app);
        if (typeid(*(app->getLeave())) == typeid(EarnedLeave))
        {
            dir->approve(appNumber, s, action);
        }
        else
        {
            sup->approve(appNumber, s, action);
        }
    }

    void takeLeave(Leave* l)
    {
        l->active = true;
    }

    void markBulk(Guard* g, Staff* s)
    {
        IApplication_for_Staff** iApps = s->get_iApps();
        int n = s->get_interfaceCNT_app();
        for (int i = 0; i < n; i++)
        {
            Application* app = dynamic_cast<Application*>(iApps[i]);
            Leave* l = app->getLeave();

            if (l != NULL && l->active && typeid(*(l->get_state())) != typeid(Approved)) //system notifies guard to take action
            {
                g->set_attendanceInterface(dynamic_cast<IAttendance_for_Guard*>(s->get_iAtts()));
                string date = l->startDate;
                int n = app->calculateDaysBetween(date, l->endDate);

                int day = stoi(date.substr(8, 2));
                for (int i = 0; i < n; i++)
                {
                    g->set_clockIn(0, date);
                    g->set_clockOut(0);
                    day++;
                    ostringstream oss;
                    oss << setw(2) << setfill('0') << day;
                    date.replace(8, 2, oss.str());
                }
                app->upd_balance(s, n, typeid(*l).name());
            }
        }

    }

    fileHandling* get_fd()
    {
        return fd;
    }

    ~MainSystem()
    {
        for (int i = 0; i < employeeCount; ++i)
            delete employees[i];
        for (int i = 0; i < leaveCount; i++)
            delete leaves[i];
    }
};

void simulate()
{
    MainSystem main;
    auto employees = main.get_fd()->get_empsVector();
    size_t cnt = employees.size();

    Director* d = new Director(employees[cnt - 1].name, stoi(employees[cnt - 1].id));
    Supervisor* s1 = new Supervisor(employees[cnt - 2].name, stoi(employees[cnt - 2].id));
    Supervisor* s2 = new Supervisor(employees[cnt - 3].name, stoi(employees[cnt - 3].id));
    Guard* g = new Guard(employees[cnt - 4].name, stoi(employees[cnt - 4].id));


    const int staff_size = MAX_STAFF;
    Staff* staff_arr[staff_size] = { NULL };

    for (int i = 0; i < staff_size; i++)
    {
        Staff* s = new Staff(employees[i].name, stoi(employees[i].id));
        staff_arr[i] = s;
        main.addEmployee(s);
    }

    main.addEmployee(g);
    main.addEmployee(s2);
    main.addEmployee(s1);
    main.addEmployee(d);

    Attendance* atten[staff_size] = {NULL};
    for (int i = 0; i < staff_size; i++)
    {
        atten[i] = new Attendance();
    }
    for (int i = 0; i < staff_size; i++)
    {
        g->set_attendanceInterface(atten[i]);
        staff_arr[i]->set_attendanceInterface(atten[i]);
        for (const auto& att : employees[i].attendanceRecords)
        {
            g->set_clockIn(att.clockIn, att.date);
            g->set_clockOut(att.clockOut);
        }
    }



    for (int i = 0; i < staff_size; i++)
    {
        int j = 0;
        for (const auto& leave : employees[i].leaveApplications)
        {
            cout << "<" << staff_arr[i]->get_name() << ": Use Count " << j + 1 << " >\n";
            Application* app10 = new Application();
            if (i == staff_size - 3)
                cout << "";
            Leave* l10 = main.chooseLeave(leave.leaveType, leave.startDate, leave.endDate, leave.reason, leave.leaveAddress);
            app10->set_leaveInterface(l10);
            staff_arr[i]->set_applicationInterface(app10);
            l10->add(staff_arr[i]);

            d->set_applicationInterface(app10);
            s1->set_applicationInterface(app10);
            s2->set_applicationInterface(app10);

            char write = 'y';
            cout << "Write Application for " << leave.leaveType << " Leave? [y/n] ";
            //cin >> write;
            write = prompt_set[i + j][0];
            cout << write << endl;

            if (write == 'y')
            {
                staff_arr[i]->writeApplication(j, leave.dateWritten, l10);
                char apr = 'y';
                cout << "Wait for Approval? [y/n] ";
                //cin >> apr;
                apr = prompt_set[i + j][1];
                cout << apr << endl;
                if (apr == 'y')
                {
                    if (staff_arr[i]->get_id() == 202)
                    {
                        cout << "";
                    }

                    //bool option = 0;
                    //cout << "\t\t\t\tSupervisor/Director -> Approve(1) -> Reject(0): ";
                    //cin >> option;
                    main.forward(d, s1, app10, j, staff_arr[i]);
                    l10->ret();
                }
            }
            main.takeLeave(l10);
            j++;
            cout << endl;
        }
        cout << "==================================================================\n";
        main.markBulk(g, staff_arr[i]);
        d->resetInterfaceCNT();
        s1->resetInterfaceCNT();
        s2->resetInterfaceCNT();
    }

    cout << endl << endl << endl << endl<<"Reports\n";

    cout << "\t< Report 1 >\n";
    main.showLowAttendanceEmployees(96, true);

    cout << "\t< Report 2>\n";
    main.showOutstandingLeaves(true);

    cout << "\t< Report 3 >\n";
    main.showAttendance(101, true);
    main.showAttendance(202, true);
    main.showAttendance(303, true);
    main.showAttendance(404, true);
    main.showAttendance(505, true);
    main.showAttendance(606, true);
    main.showAttendance(707, true);
    main.showAttendance(808, true);
    main.showAttendance(909, true);
    main.showAttendance(10, true);
    main.showAttendance(111, true);
    main.showAttendance(212, true);


    cout << "\t< Report 4 >\n";
    main.showLeaveDetails(101, true);
    main.showLeaveDetails(202, true);
    main.showLeaveDetails(303, true);
    main.showLeaveDetails(404, true);
    main.showLeaveDetails(505, true);
    main.showLeaveDetails(606, true);
    main.showLeaveDetails(707, true);
    main.showLeaveDetails(808, true);
    main.showLeaveDetails(909, true);
    main.showLeaveDetails(10, true);
    main.showLeaveDetails(111, true);
    main.showLeaveDetails(212, true);
}

int main()
{
    vector<char> prompt_1;
    prompt_1.push_back('y'); //write app
    prompt_1.push_back('y'); //wait for approval

    vector<char> prompt_2;
    prompt_2.push_back('y'); //write app
    prompt_2.push_back('n'); //dont wait for approval

    vector<char> prompt_3;
    prompt_3.push_back('n'); //dont write app

    prompt_set.push_back(prompt_3);
    prompt_set.push_back(prompt_2);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);
    prompt_set.push_back(prompt_1);

    simulate();

    system("pause");
}

//int main() {
//
//    MainSystem mainSystem;
//
//    Director* dir = new Director("Ricardo", -2);
//    Supervisor* sup = new Supervisor("Dave", -1);
//    Guard* g = new Guard("Roberto", 0);
//
//    Staff* s = new Staff("Smith", 1);
//    Staff* s2 = new Staff("John", 2);
//    Staff* s3 = new Staff("Orton", 3);
//    Staff* s4 = new Staff("Savage", 4);
//    Staff* stt_arr[] = { s,s2,s3,s4 };
//
//    mainSystem.addEmployee(s);
//    mainSystem.addEmployee(s2);
//    mainSystem.addEmployee(s3);
//    mainSystem.addEmployee(s4);
//    mainSystem.addEmployee(g);
//    mainSystem.addEmployee(sup);
//    mainSystem.addEmployee(dir);
//
//
//    Attendance* att = new Attendance();
//    Attendance* att2 = new Attendance();
//    Attendance* att3 = new Attendance();
//    Attendance* att4 = new Attendance();
//    Attendance* att_arr[] = { att, att2, att3, att4 };
//
//    //Leave* l = new Leave("","","","");
//    //Leave* l2 = new Leave("", "", "", "");
//    //Leave* l3 = new Leave("", "", "", "");
//    //Leave* l4 = new Leave("", "", "", "");
//
//
//    s->set_attendanceInterface(att); //staff id 1 arrives at gate
//    g->set_attendanceInterface(att); //guard brings up staff's attendance sheet
//    g->set_clockIn(7, "2024-01-21");
//    g->set_clockOut(7 + 12);
//    g->set_clockIn(7, "2024-01-22");
//    g->set_clockOut(7 + 12);
//    g->set_clockIn(7, "2024-01-23");
//    g->set_clockOut(7 + 12);
//    g->set_clockIn(7, "2024-01-24");
//    g->set_clockOut(7 + 1);
//    g->set_clockIn(0, "2024-01-25");
//    g->set_clockOut(0);
//
//    s2->set_attendanceInterface(att2); //staff id 2 arrives at gate
//    g->set_attendanceInterface(att2); //guard brings up staff's attendance sheet
//    g->set_clockIn(10, "2024-01-24");
//    g->set_clockOut(10 + 5);
//
//    s3->set_attendanceInterface(att3); //staff id 3 arrives at gate
//    g->set_attendanceInterface(att3); //guard brings up staff's attendance sheet
//    g->set_clockIn(10, "2024-01-19");
//    g->set_clockOut(10 + 6);
//    g->set_clockIn(8, "2024-01-20");
//    g->set_clockOut(8 + 12);
//    g->set_clockIn(13, "2024-01-21");
//    g->set_clockOut(13 + 10);
//
//    s4->set_attendanceInterface(att4); //staff id 4 arrives at gate
//    g->set_attendanceInterface(att4); //guard brings up staff's attendance sheet
//    g->set_clockIn(9, "2024-01-17");
//    g->set_clockOut(9 + 7);
//    g->set_clockIn(10, "2024-01-18");
//    g->set_clockOut(10 + 10);
//    g->set_clockIn(9, "2024-01-29");
//    g->set_clockOut(9 + 10);
//
//    Application* app10 = new Application();
//    Application* app20 = new Application();
//    Application* app21 = new Application();
//    Application* app30 = new Application();
//    Application* app11 = new Application();
//    Application* app40 = new Application();
//    Application* app_arr[] = { app10,app20,app21,app30, app11, app40 };
//
//    //start
//    {
//        //staff id 1 writes 2 applications, both get accepted
//        Leave* l10 = mainSystem.chooseLeave("Casual", "2024-11-01", "2024-11-02", "Sick", "Home");
//        app10->set_leaveInterface(l10);
//        s->set_applicationInterface(app10); //staff id 1 uses app interface
//        s->writeApplication(0, "2024-10-30", l10);
//
//        l10->add(s);
//
//        mainSystem.forward(app10, 0, s);
//
//        l10->ret();
//        //sup->set_applicationInterface(app); //supervisor sees the application
//        //sup->approve(0, s); //supervisor approves application
//
//        mainSystem.takeLeave(l10); //staff takes leave
//
//        Leave* l11 = mainSystem.chooseLeave("Earned", "2024-11-01", "2024-11-08", "Sick", "Yes");
//        app11->set_leaveInterface(l11);
//        s->set_applicationInterface(app11);
//        s->writeApplication(1, "2024-10-30", l11);
//
//        l11->add(s);
//
//        mainSystem.forward(app11, 1, s);
//        //sup->set_applicationInterface(app5);
//        //sup->approve(1, s);
//
//        l11->ret();
//
//        mainSystem.takeLeave(l11); //staff takes leave
//
//        sup->resetInterfaceCNT();
//        mainSystem.markBulk(g, s);
//    }
//    //END
//    //start
//    {
//        //staff id 2 writes 2 applications, both get rejected
//        Leave* l20 = mainSystem.chooseLeave("Earned", "2024-11-01", "2024-11-02", "Sick", "Home");
//        s2->set_applicationInterface(app20);
//        s2->writeApplication(0, "2024-10-30", l20);
//        l20->add(s2);
//        mainSystem.forward(app20, 0, s2, false);
//        l20->ret();
//
//        Leave* l21 = mainSystem.chooseLeave("Unpaid", "2025-01-20", "2025-01-22", "Personal reasons", "Home");
//        s2->set_applicationInterface(app21);
//        s2->writeApplication(1, "2025-01-18", l21);
//        l21->add(s2);
//        mainSystem.forward(app21, 1, s2, false);
//        l21->ret();
//
//        sup->resetInterfaceCNT();
//        mainSystem.markBulk(g, s2);
//    }
//    //END
//    //start
//    {
//        //staff id 3 writes 1 applications, doesn't wait to approval and takes leave
//        Leave* l30 = mainSystem.chooseLeave("Earned", "2024-12-10", "2024-12-15", "Family Vacation", "Hawaii");
//        app30->set_leaveInterface(l30);
//        s3->set_applicationInterface(app30);
//        s3->writeApplication(0, "2024-12-01", l30);
//        l30->add(s3);
//        l30->ret();
//        mainSystem.takeLeave(l30);
//
//        mainSystem.markBulk(g, s3);
//
//        sup->resetInterfaceCNT();
//    }
//    //END
//    //start
//    {
//        //staff id 4 doesn't write an applications, and takes leave
//        Leave* l40 = mainSystem.chooseLeave("Casual", "2024-12-5", "2024-12-15", "Army", "Hawaii");
//        app40->set_leaveInterface(l40);
//        s4->set_applicationInterface(app40);
//        s4->set_attendanceInterface(att4);
//        l40->add(s4);
//        l40->ret();
//        mainSystem.takeLeave(l40);
//
//        mainSystem.markBulk(g, s4);
//
//        sup->resetInterfaceCNT();
//    }
//    //END
//
//
//
//    //Report Generation
//    cout << "Show employees whose attendance for a month is less than a given percentage (e.g. 80%)" << endl;
//    mainSystem.showLowAttendanceEmployees(80, true);
//    cout << endl;
//
//    cout << "Show employees with any outstanding leaves (e.g. earned leaves without approval, or casual leaves which are not filed)" << endl;
//    mainSystem.showOutstandingLeaves(1);
//    cout << endl;
//
//    cout << "Show attendance details for a given employee" << endl;
//    for (int i = 1; i < 5; i++)
//    {
//        mainSystem.showAttendance(i, true);
//        cout << endl;
//    }
//
//
//    cout << "Show leave details for a given employee (including leave balance)" << endl;
//    for (int i = 1; i < 5; i++)
//    {
//        mainSystem.showLeaveDetails(i, true);
//        cout << endl;
//    }
//
//    //system("pause");
//    return 0;
//
//}