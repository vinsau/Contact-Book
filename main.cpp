// Gadget Store Management System

// Required Libraries

// iostream: Provides input/output stream functionality (cin, cout) for user interaction
#include <iostream>

// iomanip: Provides tools for formatting output (setw, setprecision) for table display
#include <iomanip>

// string: Provides string class and related functions for text manipulation
#include <string>

// map: Provides map container to store gadgets organized by categories
#include <map>

// vector: Provides dynamic array functionality for storing lists of gadgets
#include <vector>

// random: Provides random number generation for creating unique serial numbers
#include <random>

// algorithm: Provides algorithms like transform() for string manipulation
#include <algorithm>

// cctype: Provides character handling functions (toupper) for case conversion
#include <cctype>

// sstream: Provides stringstream for formatting strings
#include <sstream>

// optional: Provides optional class for handling optional values
#include <optional>

// functional: Provides std::function for function objects
#include <functional>

// Forward declarations
class InputValidator;

/*
 * Gadget Class: Represents a single gadget item in the store
 * Contains all properties of a gadget and methods to access/modify them
 */
class Gadget {
private:
    std::string model;          // Name/model of the gadget
    std::string category;       // Category (phone, laptop, etc.)
    std::string serialNumber;   // Unique identifier
    std::string brand;          // Manufacturer/brand name
    double price;              // Price in currency
    std::string color;         // Color of the gadget
    int stockQuantity;         // Available quantity in stock

public:
    // Default constructor
    Gadget() = default;

    // Constructor with all gadget properties
    Gadget(const std::string& model, const std::string& category, 
           const std::string& serialNumber, const std::string& brand,
           double price, const std::string& color, int stockQuantity)
        : model(model), category(category), serialNumber(serialNumber),
          brand(brand), price(price), color(color), stockQuantity(stockQuantity) {}

    // Getter methods: Return the respective property values
    std::string getModel() const { return model; }
    std::string getCategory() const { return category; }
    std::string getSerialNumber() const { return serialNumber; }
    std::string getBrand() const { return brand; }
    double getPrice() const { return price; }
    std::string getColor() const { return color; }
    int getStockQuantity() const { return stockQuantity; }

    // Setter methods: Update the respective property values
    void setModel(const std::string& model) { this->model = model; }
    void setCategory(const std::string& category) { this->category = category; }
    void setSerialNumber(const std::string& serialNumber) { this->serialNumber = serialNumber; }
    void setBrand(const std::string& brand) { this->brand = brand; }
    void setPrice(double price) { this->price = price; }
    void setColor(const std::string& color) { this->color = color; }
    void setStockQuantity(int quantity) { this->stockQuantity = quantity; }
};

// Error messages class for centralized message management
class ErrorMessages {
public:
    static std::string modelLength(size_t maxLength) {
        return "Model must be between 2 and " + 
               std::to_string(maxLength) + " characters.";
    }
    
    static std::string modelFormat() {
        return "Model cannot be purely numeric and must contain letters.";
    }
    
    static std::string brandLength(size_t maxLength) {
        return "Brand must be between 2 and " + 
               std::to_string(maxLength) + " characters.";
    }
    
    static std::string brandFormat() {
        return "Brand must contain letters, and may include numbers, spaces, hyphens, or dots.";
    }
    
    static std::string categoryFormat() {
        return "Category must contain only letters and spaces.";
    }
    
    static std::string categoryLength(size_t maxLength) {
        return "Category must be between 1 and " + 
               std::to_string(maxLength) + " characters.";
    }
    
    static std::string invalidColor() {
        return "Invalid color! Please choose from the list above.";
    }
    
    template<typename T>
    static std::string numericRange(std::string field, T min, T max) {
        // Remove trailing whitespace and colon
        size_t endPos = field.find_last_not_of(": ");
        if (endPos != std::string::npos) {
            field = field.substr(0, endPos + 1);
        }
        return field + " must be between " + std::to_string(min) + " and " + std::to_string(max) + ".";
    }
    
    static std::string invalidNumber() {
        return "Invalid number format. Please enter a valid number.";
    }
};

// Input validation class
class InputValidator {
public:
    static constexpr size_t MAX_TEXT_LENGTH = 50;
    static constexpr double MAX_PRICE = 999999.99;
    static constexpr int MAX_QUANTITY = 9999;
    static constexpr size_t MIN_MODEL_LENGTH = 2;
    static constexpr size_t MIN_BRAND_LENGTH = 2;

    // Predefined list of valid colors
    static const inline std::vector<std::string> validColors = {
        "Red", "Blue", "Green", "Yellow", "Black", "White", "Purple", "Orange",
        "Pink", "Brown", "Gray", "Silver", "Gold", "Navy", "Teal", "Maroon",
        "Violet", "Magenta", "Cyan", "Turquoise", "Indigo", "Crimson", "Beige",
        "Ivory", "Olive", "Coral", "Burgundy", "Lavender", "Plum", "Khaki"
    };

    // Check if string is purely numeric
    static bool isPurelyNumeric(const std::string& str) {
        return !str.empty() && std::all_of(str.begin(), str.end(), 
            [](char c) { return std::isdigit(c); });
    }

    // Validate text that should contain only letters
    static bool isLettersOnly(const std::string& text, size_t minLength = 1) {
        if (text.length() < minLength || text.length() > MAX_TEXT_LENGTH) return false;
        return std::all_of(text.begin(), text.end(), 
            [](char c) { return std::isalpha(c) || std::isspace(c); });
    }

    // Validate text that can contain letters and numbers
    static bool isAlphanumeric(const std::string& text, size_t minLength = 1) {
        if (text.length() < minLength || text.length() > MAX_TEXT_LENGTH) return false;
        return std::all_of(text.begin(), text.end(), 
            [](char c) { return std::isalnum(c) || std::isspace(c) || c == '-' || c == '.'; });
    }

    // Template function for numeric input validation
    template<typename T>
    static bool isValidNumber(const T& value, T minValue, T maxValue) {
        return value >= minValue && value <= maxValue;
    }

    // Validate model (alphanumeric, not purely numeric)
    static bool isValidModel(const std::string& model) {
        if (model.length() < MIN_MODEL_LENGTH || model.length() > MAX_TEXT_LENGTH) return false;
        if (isPurelyNumeric(model)) return false;
        return isAlphanumeric(model);
    }

    // Validate category (letters only)
    static bool isValidCategory(const std::string& category) {
        return isLettersOnly(category);
    }

    // Validate brand (alphanumeric with special chars)
    static bool isValidBrand(const std::string& brand) {
        if (brand.length() < MIN_BRAND_LENGTH || brand.length() > MAX_TEXT_LENGTH) return false;
        return isAlphanumeric(brand);
    }

    // Normalize text to title case
    static std::string toTitleCase(std::string text) {
        if (text.empty()) return text;
        
        // Convert first character to uppercase
        text[0] = std::toupper(text[0]);
        
        // Convert rest to lowercase
        for (size_t i = 1; i < text.length(); ++i) {
            text[i] = std::tolower(text[i]);
            // Capitalize letter after space
            if (i > 0 && text[i-1] == ' ') {
                text[i] = std::toupper(text[i]);
            }
        }
        return text;
    }

    // Validate color against predefined list
    static bool isValidColor(const std::string& color) {
        std::string lowerColor = color;
        std::transform(lowerColor.begin(), lowerColor.end(), lowerColor.begin(), ::tolower);
        
        // Convert valid colors to lowercase for comparison
        std::vector<std::string> lowerValidColors;
        std::transform(validColors.begin(), validColors.end(), 
                      std::back_inserter(lowerValidColors),
                      [](const std::string& s) {
                          std::string lower = s;
                          std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                          return lower;
                      });
        
        return std::find(lowerValidColors.begin(), lowerValidColors.end(), lowerColor) 
               != lowerValidColors.end();
    }

    // Get valid color input
    static std::string getValidColorInput(const std::string& prompt) {
        displayValidColors();
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (input.empty()) return input;
            
            std::string lowerInput = input;
            std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);
            
            // Find matching color (case-insensitive)
            auto it = std::find_if(validColors.begin(), validColors.end(),
                [&lowerInput](const std::string& validColor) {
                    std::string lowerValid = validColor;
                    std::transform(lowerValid.begin(), lowerValid.end(), 
                                 lowerValid.begin(), ::tolower);
                    return lowerValid == lowerInput;
                });
                
            if (it != validColors.end()) {
                return *it;  // Return the original case from validColors
            }
            std::cout << ErrorMessages::invalidColor() << "\n";
        }
    }

    // Validate price
    static bool isValidPrice(double price) {
        return isValidNumber(price, 0.0, MAX_PRICE);
    }

    // Validate quantity
    static bool isValidQuantity(int quantity) {
        return isValidNumber(quantity, 0, MAX_QUANTITY);
    }

    // Get valid input with retry mechanism
    static std::string getValidInput(const std::string& prompt, 
                                   const std::function<bool(const std::string&)>& validator,
                                   const std::string& errorMessage) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (input.empty() || validator(input)) break;
            std::cout << errorMessage << "\n";
        }
        return input;
    }

    // Get valid numeric input
    template<typename T>
    static std::optional<T> getValidNumericInput(const std::string& prompt, T minValue, T maxValue) {
        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);
            if (input.empty()) return std::nullopt;
            
            try {
                T value;
                if constexpr (std::is_same_v<T, int>) {
                    value = std::stoi(input);
                } else if constexpr (std::is_same_v<T, double>) {
                    value = std::stod(input);
                }
                if (isValidNumber(value, minValue, maxValue)) return value;
                std::cout << ErrorMessages::numericRange(prompt, minValue, maxValue) << "\n";
            } catch (...) {
                std::cout << ErrorMessages::invalidNumber() << "\n";
            }
        }
    }

    // Format price with 2 decimal places
    static std::string formatPrice(double price) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << price;
        return ss.str();
    }

    // Display available colors
    static void displayValidColors() {
        std::cout << "Available colors:\n";
        int count = 0;
        for (const auto& color : validColors) {
            std::cout << std::setw(12) << std::left << color;
            if (++count % 5 == 0) std::cout << "\n";
        }
        if (count % 5 != 0) std::cout << "\n";
    }
};

/*
 * GadgetStore Class: Manages the entire gadget store operations
 * Handles all CRUD operations and user interface
 */
class GadgetStore {
private:
    // Store gadgets organized by category
    std::map<std::string, std::vector<Gadget>> gadgetsByCategory;
    
    // Serial number tracking
    std::map<std::string, int> categoryCounters;

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

    // Generate structured serial number
    std::string generateSerialNumber(const std::string& category) {
        // Initialize counter for new category
        if (categoryCounters.find(category) == categoryCounters.end()) {
            categoryCounters[category] = 0;
        }
        
        // Increment counter
        int counter = ++categoryCounters[category];
        
        // Get first two letters of category (or pad with 'X')
        std::string prefix = toUpper(category.substr(0, 2));
        if (prefix.length() < 2) prefix.append(2 - prefix.length(), 'X');
        
        // Format: CCYYNNNNN (CC=Category, YY=Year, NNNNN=Sequential)
        std::time_t now = std::time(nullptr);
        std::tm* ltm = std::localtime(&now);
        
        std::stringstream ss;
        ss << prefix
           << std::setfill('0') << std::setw(2) << (ltm->tm_year + 1900) % 100
           << std::setfill('0') << std::setw(5) << counter;
           
        return ss.str();
    }

    // Validate category (should not be empty or purely numeric)
    bool isValidCategory(const std::string& category) const {
        return !category.empty() && !std::all_of(category.begin(), category.end(), 
            [](char c) { return std::isdigit(c); });
    }

    // Displays gadgets in a formatted table
    void displayGadgetTable(const std::vector<Gadget>& gadgets) const {
        std::cout << std::string(100, '-') << '\n';
        std::cout << std::left 
                  << std::setw(8) << "SERIAL#" << " | "
                  << std::setw(15) << "BRAND" << " | "
                  << std::setw(15) << "MODEL" << " | "
                  << std::setw(10) << "CATEGORY" << " | "
                  << std::setw(8) << "PRICE" << " | "
                  << std::setw(10) << "COLOR" << " | "
                  << std::setw(5) << "STOCK" << '\n';
        std::cout << std::string(100, '-') << '\n';

        for (const auto& gadget : gadgets) {
            std::cout << std::left
                      << std::setw(8) << toUpper(gadget.getSerialNumber()) << " | "
                      << std::setw(15) << toUpper(gadget.getBrand()) << " | "
                      << std::setw(15) << toUpper(gadget.getModel()) << " | "
                      << std::setw(10) << toUpper(gadget.getCategory()) << " | "
                      << std::setw(8) << std::fixed << std::setprecision(2) << gadget.getPrice() << " | "
                      << std::setw(10) << toUpper(gadget.getColor()) << " | "
                      << std::setw(5) << gadget.getStockQuantity() << '\n';
        }
        std::cout << std::string(100, '-') << '\n';
    }

public:
    // Initialize random number generator
    GadgetStore() {}

    // Add new gadget to the store with user input
    void addGadget() {
        displayHeader("ADD NEW GADGET");
        
        // Get model
        std::string model = InputValidator::getValidInput(
            "Enter gadget model: ",
            InputValidator::isValidModel,
            ErrorMessages::modelLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
            ErrorMessages::modelFormat()
        );

        // Get category
        std::string category = InputValidator::getValidInput(
            "Enter gadget category: ",
            InputValidator::isValidCategory,
            ErrorMessages::categoryFormat() + "\n" + 
            ErrorMessages::categoryLength(InputValidator::MAX_TEXT_LENGTH)
        );

        // Get brand
        std::string brand = InputValidator::getValidInput(
            "Enter brand name: ",
            InputValidator::isValidBrand,
            ErrorMessages::brandLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
            ErrorMessages::brandFormat()
        );
        
        // Get price
        double price = 0.0;
        auto priceInput = InputValidator::getValidNumericInput(
            "Enter price: ", 0.0, InputValidator::MAX_PRICE
        );
        if (priceInput) price = *priceInput;
        
        // Get color
        std::string color = InputValidator::getValidColorInput("Enter color: ");
        
        // Get quantity
        int quantity = 0;
        auto quantityInput = InputValidator::getValidNumericInput(
            "Enter stock quantity: ", 0, InputValidator::MAX_QUANTITY
        );
        if (quantityInput) quantity = *quantityInput;

        std::string serialNumber = generateSerialNumber(category);
        Gadget gadget(model, category, serialNumber, brand, price, color, quantity);
        gadgetsByCategory[category].push_back(gadget);
        
        std::cout << "\nGadget added successfully!\n";
        std::cout << "Generated Serial Number: " << serialNumber << "\n";
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Search for gadgets by category, brand, or model
    void searchGadget() const {
        displayHeader("SEARCH GADGET");
        
        std::string searchTerm = toUpper(getInput("Enter search term (brand/model/category): "));
        if (searchTerm.empty()) {
            std::cout << "\nSearch term cannot be empty!\n";
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            return;
        }

        std::vector<Gadget> results;
        
        // Search in categories first
        for (const auto& category : gadgetsByCategory) {
            if (toUpper(category.first).find(searchTerm) != std::string::npos) {
                std::cout << "\nFound gadgets in category '" << toUpper(category.first) << "':\n\n";
                displayGadgetTable(category.second);
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return;
            }
        }

        // Then search in brands
        bool foundBrand = false;
        for (const auto& category : gadgetsByCategory) {
            for (const auto& gadget : category.second) {
                if (toUpper(gadget.getBrand()).find(searchTerm) != std::string::npos) {
                    if (!foundBrand) {
                        std::cout << "\nFound gadgets of brand '" << toUpper(searchTerm) << "':\n\n";
                        foundBrand = true;
                    }
                    results.push_back(gadget);
                }
            }
        }

        if (foundBrand) {
            displayGadgetTable(results);
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            return;
        }

        // Finally search in models
        results.clear();
        for (const auto& category : gadgetsByCategory) {
            for (const auto& gadget : category.second) {
                if (toUpper(gadget.getModel()).find(searchTerm) != std::string::npos) {
                    results.push_back(gadget);
                }
            }
        }
        
        if (results.empty()) {
            std::cout << "\nNo gadgets found matching your search.\n";
        } else {
            std::cout << "\nFound " << results.size() << " matching gadget(s) by model:\n\n";
            displayGadgetTable(results);
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Delete a gadget using its serial number
    bool deleteGadget() {
        while (true) {
            displayHeader("DELETE GADGET");
            
            if (gadgetsByCategory.empty()) {
                std::cout << "\nNo gadgets in store!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return false;
            }

            std::cout << "\nCurrent Gadgets in Store:\n\n";
            for (const auto& category : gadgetsByCategory) {
                displayGadgetTable(category.second);
                std::cout << '\n';
            }
            
            std::string serialNumber = toUpper(getInput("\nEnter gadget serial number to delete (or 'Q' to go back): "));
            
            if (serialNumber == "Q") {
                return false;
            }

            bool found = false;
            for (auto& category : gadgetsByCategory) {
                auto& gadgets = category.second;
                for (auto it = gadgets.begin(); it != gadgets.end(); ++it) {
                    if (toUpper(it->getSerialNumber()) == serialNumber) {
                        gadgets.erase(it);
                        
                        // Remove the category if it's empty
                        if (gadgets.empty()) {
                            gadgetsByCategory.erase(category.first);
                        }
                        
                        std::cout << "\nGadget deleted successfully!\n";
                        std::cout << "\nPress Enter to continue...";
                        std::cin.get();
                        return true;
                    }
                }
            }
            
            std::cout << "\nGadget not found!\n";
            std::string retry = toUpper(getInput("Would you like to try again? (Y/N): "));
            if (retry != "Y") {
                return false;
            }
        }
    }

    // Modify existing gadget properties
    bool modifyGadget() {
        while (true) {
            displayHeader("MODIFY GADGET");
            
            if (gadgetsByCategory.empty()) {
                std::cout << "\nNo gadgets in store!\n";
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                return false;
            }

            std::cout << "\nCurrent Gadgets in Store:\n\n";
            for (const auto& category : gadgetsByCategory) {
                displayGadgetTable(category.second);
                std::cout << '\n';
            }
            
            std::string serialNumber = toUpper(getInput("\nEnter gadget serial number to modify (or 'Q' to go back): "));
            
            if (serialNumber == "Q") {
                return false;
            }

            bool found = false;
            for (auto& category : gadgetsByCategory) {
                auto& gadgets = category.second;
                for (auto& gadget : gadgets) {
                    if (toUpper(gadget.getSerialNumber()) == serialNumber) {
                        found = true;
                        std::cout << "\nSelected gadget details:\n";
                        std::vector<Gadget> currentGadget = {gadget};
                        displayGadgetTable(currentGadget);
                        
                        std::cout << "\nEnter new details (press Enter to keep current value):\n";
                        
                        std::string input;
                        
                        input = InputValidator::getValidInput(
                            "Model [" + gadget.getModel() + "]: ",
                            InputValidator::isValidModel,
                            ErrorMessages::modelLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
                            ErrorMessages::modelFormat()
                        );
                        if (!input.empty()) gadget.setModel(input);
                        
                        input = InputValidator::getValidInput(
                            "Brand [" + gadget.getBrand() + "]: ",
                            InputValidator::isValidBrand,
                            ErrorMessages::brandLength(InputValidator::MAX_TEXT_LENGTH) + "\n" + 
                            ErrorMessages::brandFormat()
                        );
                        if (!input.empty()) gadget.setBrand(input);
                        
                        input = InputValidator::getValidColorInput(
                            "Color [" + gadget.getColor() + "]: "
                        );
                        if (!input.empty()) gadget.setColor(input);
                        
                        auto priceInput = InputValidator::getValidNumericInput(
                            "Price [" + InputValidator::formatPrice(gadget.getPrice()) + "]: ",
                            0.0, InputValidator::MAX_PRICE
                        );
                        if (priceInput) gadget.setPrice(*priceInput);
                        
                        auto quantityInput = InputValidator::getValidNumericInput(
                            "Stock Quantity [" + std::to_string(gadget.getStockQuantity()) + "]: ",
                            0, InputValidator::MAX_QUANTITY
                        );
                        if (quantityInput) gadget.setStockQuantity(*quantityInput);
                        
                        std::cout << "\nNote: Category cannot be modified. Create a new gadget with the desired category.\n";
                        
                        std::cout << "\nGadget modified successfully!\n";
                        std::cout << "\nPress Enter to continue...";
                        std::cin.get();
                        return true;
                    }
                }
            }
            
            if (!found) {
                std::cout << "\nGadget not found!\n";
                std::string retry = toUpper(getInput("Would you like to try again? (Y/N): "));
                if (retry != "Y") {
                    return false;
                }
            }
        }
    }

    // Display all gadgets in the store
    void listGadgets() const {
        displayHeader("LIST ALL GADGETS");
        
        if (gadgetsByCategory.empty()) {
            std::cout << "\nNo gadgets in store!\n";
        } else {
            for (const auto& category : gadgetsByCategory) {
                std::cout << "\nCategory: " << toUpper(category.first) << "\n\n";
                displayGadgetTable(category.second);
                std::cout << '\n';
            }
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

    // Display main menu options
    void displayMenu() const {
        displayHeader("GADGET STORE MANAGEMENT SYSTEM");
        std::cout << "\n1. Add Gadget";
        std::cout << "\n2. Search Gadget";
        std::cout << "\n3. Delete Gadget";
        std::cout << "\n4. Modify Gadget";
        std::cout << "\n5. List All Gadgets";
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
                    addGadget();
                    break;
                case '2':
                    searchGadget();
                    break;
                case '3':
                    deleteGadget();
                    break;
                case '4':
                    modifyGadget();
                    break;
                case '5':
                    listGadgets();
                    break;
                case '6':
                    std::cout << "\nThank you for using Gadget Store Management System!\n";
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
    GadgetStore store;
    store.run();
    return 0;
}