// Contact Book Management System

// Required Libraries
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <optional>
#include <functional>
#include <regex>

// Forward declarations
class InputValidator;

/*
 * Contact Class: Represents a single contact in the address book
 */
class Contact {
private:
    std::string name;           // Full name of the contact
    std::string phoneNumber;    // Phone number
    std::string email;          // Email address
    std::string address;        // Physical address
    std::string birthdate;      // Birthdate

public:
    // Default constructor
    Contact() = default;

    // Constructor with all contact properties
    Contact(const std::string& name, const std::string& phoneNumber, 
           const std::string& email, const std::string& address,
           const std::string& birthdate)
        : name(name), phoneNumber(phoneNumber), email(email),
          address(address), birthdate(birthdate) {}

    // Getter methods
    std::string getName() const { return name; }
    std::string getPhoneNumber() const { return phoneNumber; }
    std::string getEmail() const { return email; }
    std::string getAddress() const { return address; }
    std::string getBirthdate() const { return birthdate; }

    // Setter methods
    void setName(const std::string& name) { this->name = name; }
    void setPhoneNumber(const std::string& phoneNumber) { this->phoneNumber = phoneNumber; }
    void setEmail(const std::string& email) { this->email = email; }
    void setAddress(const std::string& address) { this->address = address; }
    void setBirthdate(const std::string& birthdate) { this->birthdate = birthdate; }
};

// Error messages class for centralized message management
class ErrorMessages {
public:
    static std::string nameLength(size_t maxLength) {
        return "Name must be between 2 and " + 
               std::to_string(maxLength) + " characters.";
    }
    
    static std::string nameFormat() {
        return "Name must contain only letters and spaces.";
    }
    
    static std::string phoneFormat() {
        return "Phone number must be 11 digits starting with '09' (e.g., 09244561530)";
    }
    
    static std::string emailFormat() {
        return "Invalid email format. Example: user@domain.com";
    }
    
    static std::string birthdateFormat() {
        return "Birthdate must be in format: DD/MM/YYYY";
    }
    
    static std::string addressLength(size_t maxLength) {
        return "Address must be between 5 and " + 
               std::to_string(maxLength) + " characters.";
    }
};

// Input validation class
class InputValidator {
public:
    static constexpr size_t MAX_TEXT_LENGTH = 100;
    static constexpr size_t MIN_NAME_LENGTH = 2;
    static constexpr size_t MIN_ADDRESS_LENGTH = 5;

    // Validate name (letters and spaces only)
    static bool isValidName(const std::string& name) {
        if (name.length() < MIN_NAME_LENGTH || name.length() > MAX_TEXT_LENGTH) return false;
        return std::all_of(name.begin(), name.end(), 
            [](char c) { return std::isalpha(c) || std::isspace(c); });
    }

    // Validate phone number format (Philippine format)
    static bool isValidPhoneNumber(const std::string& phone) {
        // Check if it's exactly 11 digits and starts with '09'
        if (phone.length() != 11 || phone.substr(0, 2) != "09") return false;
        return std::all_of(phone.begin(), phone.end(), ::isdigit);
    }

    // Format phone number for display (convert 09XXXXXXXXX to +63 (XXX) XXX XXXX)
    static std::string formatPhoneNumber(const std::string& phone) {
        if (phone.length() != 11 || phone.substr(0, 2) != "09") return phone;
        
        std::string areaCode = phone.substr(1, 3);
        std::string firstPart = phone.substr(4, 3);
        std::string secondPart = phone.substr(7, 4);
        
        return "+63 (" + areaCode + ") " + firstPart + " " + secondPart;
    }

    // Validate email format
    static bool isValidEmail(const std::string& email) {
        std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, emailPattern);
    }

    // Validate birthdate format (DD/MM/YYYY)
    static bool isValidBirthdate(const std::string& date) {
        std::regex datePattern(R"((\d{2})/(\d{2})/(\d{4}))");
        if (!std::regex_match(date, datePattern)) return false;
        
        int day = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(3, 2));
        int year = std::stoi(date.substr(6, 4));
        
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        if (year < 1900 || year > 2025) return false;
        
        return true;
    }

    // Validate address length
    static bool isValidAddress(const std::string& address) {
        return address.length() >= MIN_ADDRESS_LENGTH && address.length() <= MAX_TEXT_LENGTH;
    }

    // Template for getting valid input with custom validation
    template<typename Validator>
    static std::string getValidInput(
        const std::string& prompt,
        Validator validator,
        const std::string& errorMsg
    ) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (validator(input)) break;
            std::cout << "\nError: " << errorMsg << "\n\n";
        }
        return input;
    }
};

/*
 * ContactBook Class: Manages the entire contact book operations
 */
class ContactBook {
private:
    std::vector<Contact> contacts;

    // Helper function to get input
    std::string getInput(const std::string& prompt) const {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    // Converts string to uppercase for case-insensitive comparisons
    std::string toUpper(const std::string& str) const {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        return upper;
    }

    // Clears the console screen for better UI
    void clearScreen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    // Displays centered header for each menu section
    void displayHeader(const std::string& title) const {
        clearScreen();
        std::cout << std::string(50, '=') << '\n';
        std::cout << std::setw((50 + title.length()) / 2) << std::right << title << '\n';
        std::cout << std::string(50, '=') << '\n';
    }

    struct ColumnWidths {
        size_t nameWidth = 4;      // "NAME"
        size_t phoneWidth = 5;     // "PHONE"
        size_t emailWidth = 5;     // "EMAIL"
        size_t addressWidth = 7;   // "ADDRESS"
        size_t birthdateWidth = 9; // "BIRTHDATE"
        
        // Add minimum padding to each column
        static constexpr size_t MIN_PADDING = 4; // 2 spaces on each side
        
        void updateWidths(const Contact& contact) {
            nameWidth = std::max(nameWidth, contact.getName().length() + MIN_PADDING);
            // Phone number will always be in format "+63 (XXX) XXX XXXX"
            phoneWidth = std::max(phoneWidth, size_t(20) + MIN_PADDING);
            emailWidth = std::max(emailWidth, contact.getEmail().length() + MIN_PADDING);
            addressWidth = std::max(addressWidth, contact.getAddress().length() + MIN_PADDING);
            birthdateWidth = std::max(birthdateWidth, contact.getBirthdate().length() + MIN_PADDING);
        }
        
        size_t getTotalWidth() const {
            // Add 3 for each " | " separator and 2 for the outer borders
            return nameWidth + phoneWidth + emailWidth + addressWidth + birthdateWidth + (4 * 3) + 2;
        }
    };

    // Displays contacts in a formatted table
    void displayContactTable(const std::vector<Contact>& contacts) const {
        // Calculate required column widths
        ColumnWidths widths;
        for (const auto& contact : contacts) {
            widths.updateWidths(contact);
        }
        
        // Create the separator line
        std::string separator(widths.getTotalWidth(), '-');
        
        // Display header
        std::cout << separator << '\n';
        std::cout << "| " << std::left
                  << std::setw(widths.nameWidth) << "NAME" << " | "
                  << std::setw(widths.phoneWidth) << "PHONE" << " | "
                  << std::setw(widths.emailWidth) << "EMAIL" << " | "
                  << std::setw(widths.addressWidth) << "ADDRESS" << " | "
                  << std::setw(widths.birthdateWidth) << "BIRTHDATE" << " |\n";
        std::cout << separator << '\n';

        // Display contacts
        for (const auto& contact : contacts) {
            std::cout << "| " << std::left
                      << std::setw(widths.nameWidth) << contact.getName() << " | "
                      << std::setw(widths.phoneWidth) << InputValidator::formatPhoneNumber(contact.getPhoneNumber()) << " | "
                      << std::setw(widths.emailWidth) << contact.getEmail() << " | "
                      << std::setw(widths.addressWidth) << contact.getAddress() << " | "
                      << std::setw(widths.birthdateWidth) << contact.getBirthdate() << " |\n";
        }
        std::cout << separator << '\n';
    }

public:
    // Add new contact
    void addContact() {
        displayHeader("ADD NEW CONTACT");
        
        std::string name = InputValidator::getValidInput(
            "Enter name: ",
            InputValidator::isValidName,
            ErrorMessages::nameLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
            ErrorMessages::nameFormat()
        );

        std::string phone = InputValidator::getValidInput(
            "Enter phone number (11 digits starting with '09'): ",
            InputValidator::isValidPhoneNumber,
            ErrorMessages::phoneFormat()
        );

        std::string email = InputValidator::getValidInput(
            "Enter email: ",
            InputValidator::isValidEmail,
            ErrorMessages::emailFormat()
        );

        std::string address = InputValidator::getValidInput(
            "Enter address: ",
            InputValidator::isValidAddress,
            ErrorMessages::addressLength(InputValidator::MAX_TEXT_LENGTH)
        );

        std::string birthdate = InputValidator::getValidInput(
            "Enter birthdate (DD/MM/YYYY): ",
            InputValidator::isValidBirthdate,
            ErrorMessages::birthdateFormat()
        );

        Contact contact(name, phone, email, address, birthdate);
        contacts.push_back(contact);
        
        std::cout << "\nContact added successfully!\n";
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Search for contacts (recursive matching)
    void searchContact() const {
        displayHeader("SEARCH CONTACT");
        
        std::string searchTerm = toUpper(getInput("Enter search term: "));
        if (searchTerm.empty()) {
            std::cout << "\nSearch term cannot be empty!\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            return;
        }

        std::vector<Contact> results;
        
        for (const auto& contact : contacts) {
            // Check all fields for partial matches
            if (toUpper(contact.getName()).find(searchTerm) != std::string::npos ||
                toUpper(contact.getPhoneNumber()).find(searchTerm) != std::string::npos ||
                toUpper(contact.getEmail()).find(searchTerm) != std::string::npos ||
                toUpper(contact.getAddress()).find(searchTerm) != std::string::npos ||
                toUpper(contact.getBirthdate()).find(searchTerm) != std::string::npos) {
                results.push_back(contact);
            }
        }
        
        if (results.empty()) {
            std::cout << "\nNo contacts found matching your search.\n";
        } else {
            std::cout << "\nFound " << results.size() << " matching contact(s):\n\n";
            displayContactTable(results);
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Delete a contact
    bool deleteContact() {
        while (true) {
            displayHeader("DELETE CONTACT");
            
            if (contacts.empty()) {
                std::cout << "\nNo contacts in address book!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return false;
            }

            std::cout << "\nCurrent Contacts:\n\n";
            displayContactTable(contacts);
            
            std::string name = getInput("\nEnter contact name to delete (or 'Q' to go back): ");
            
            if (toUpper(name) == "Q") {
                return false;
            }

            auto it = std::find_if(contacts.begin(), contacts.end(),
                [&name](const Contact& c) { return c.getName() == name; });

            if (it != contacts.end()) {
                contacts.erase(it);
                std::cout << "\nContact deleted successfully!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return true;
            }
            
            std::cout << "\nContact not found!\n";
            std::string retry = toUpper(getInput("Would you like to try again? (Y/N): "));
            if (retry != "Y") {
                return false;
            }
        }
    }

    // Modify existing contact
    bool modifyContact() {
        while (true) {
            displayHeader("MODIFY CONTACT");
            
            if (contacts.empty()) {
                std::cout << "\nNo contacts in address book!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return false;
            }

            std::cout << "\nCurrent Contacts:\n\n";
            displayContactTable(contacts);
            
            std::string name = getInput("\nEnter contact name to modify (or 'Q' to go back): ");
            
            if (toUpper(name) == "Q") {
                return false;
            }

            auto it = std::find_if(contacts.begin(), contacts.end(),
                [&name](const Contact& c) { return c.getName() == name; });

            if (it != contacts.end()) {
                std::cout << "\nSelected contact details:\n";
                std::vector<Contact> currentContact = {*it};
                displayContactTable(currentContact);
                
                std::cout << "\nEnter new details (press Enter to keep current value):\n";
                
                std::string input;
                
                input = InputValidator::getValidInput(
                    "Name [" + it->getName() + "]: ",
                    InputValidator::isValidName,
                    ErrorMessages::nameLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
                    ErrorMessages::nameFormat()
                );
                if (!input.empty()) it->setName(input);
                
                input = InputValidator::getValidInput(
                    "Phone [" + it->getPhoneNumber() + "]: ",
                    InputValidator::isValidPhoneNumber,
                    ErrorMessages::phoneFormat()
                );
                if (!input.empty()) it->setPhoneNumber(input);
                
                input = InputValidator::getValidInput(
                    "Email [" + it->getEmail() + "]: ",
                    InputValidator::isValidEmail,
                    ErrorMessages::emailFormat()
                );
                if (!input.empty()) it->setEmail(input);
                
                input = InputValidator::getValidInput(
                    "Address [" + it->getAddress() + "]: ",
                    InputValidator::isValidAddress,
                    ErrorMessages::addressLength(InputValidator::MAX_TEXT_LENGTH)
                );
                if (!input.empty()) it->setAddress(input);
                
                input = InputValidator::getValidInput(
                    "Birthdate [" + it->getBirthdate() + "]: ",
                    InputValidator::isValidBirthdate,
                    ErrorMessages::birthdateFormat()
                );
                if (!input.empty()) it->setBirthdate(input);
                
                std::cout << "\nContact modified successfully!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return true;
            }
            
            std::cout << "\nContact not found!\n";
            std::string retry = toUpper(getInput("Would you like to try again? (Y/N): "));
            if (retry != "Y") {
                return false;
            }
        }
    }

    // Display all contacts
    void listContacts() const {
        displayHeader("LIST ALL CONTACTS");
        
        if (contacts.empty()) {
            std::cout << "\nNo contacts in address book!\n";
        } else {
            displayContactTable(contacts);
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Display main menu options
    void displayMenu() const {
        displayHeader("CONTACT BOOK MANAGEMENT SYSTEM");
        std::cout << "\n1. Add Contact";
        std::cout << "\n2. Search Contact";
        std::cout << "\n3. Delete Contact";
        std::cout << "\n4. Modify Contact";
        std::cout << "\n5. List All Contacts";
        std::cout << "\n6. Exit";
        std::cout << "\n\nEnter your choice (1-6): ";
    }

    // Main program loop
    void run() {
        while (true) {
            displayMenu();
            std::string choice = getInput("");
            
            switch (choice[0]) {
                case '1':
                    addContact();
                    break;
                case '2':
                    searchContact();
                    break;
                case '3':
                    deleteContact();
                    break;
                case '4':
                    modifyContact();
                    break;
                case '5':
                    listContacts();
                    break;
                case '6':
                    std::cout << "\nThank you for using Contact Book Management System!\n";
                    return;
                default:
                    std::cout << "\nInvalid choice! Press Enter to continue...";
                    std::cin.get();
            }
        }
    }
};

// Program entry point
int main() {
    ContactBook contactBook;
    contactBook.run();
    return 0;
}