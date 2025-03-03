# Contact Book Management System

A simple and efficient contact management system implemented in C++, designed specifically for users in the Philippines. This system allows you to store and manage contact information with a user-friendly interface and proper input validation.

## Features

- **Contact Management**
  - Add new contacts
  - Search existing contacts
  - Modify contact details
  - Delete contacts
  - List all contacts

- **Contact Information Fields**
  - Name (letters and spaces only)
  - Phone Number (Philippine format)
  - Email Address
  - Physical Address
  - Birthdate

- **Smart Phone Number Handling**
  - Accepts simple 11-digit Philippine mobile numbers (e.g., 09244561530)
  - Automatically formats display as international format: +63 (XXX) XXX XXXX
  - Validates proper Philippine mobile number format (must start with '09')

- **Dynamic Table Display**
  - Automatically adjusts column widths based on content
  - Maintains minimum padding (2 spaces on each side)
  - Clean and organized table borders
  - Responsive to long text inputs

## Requirements

- C++ compiler with C++11 support or higher
- Standard C++ libraries

## Compilation

To compile the program, use the following command in your terminal:

```bash
g++ -o contact_book main.cpp
```

## Usage

1. Run the compiled program:
   ```bash
   ./contact_book
   ```

2. Use the menu options:
   - Press 1: Add a new contact
   - Press 2: Search for contacts
   - Press 3: Delete a contact
   - Press 4: Modify existing contact
   - Press 5: List all contacts
   - Press 6: Exit the program

## Input Guidelines

- **Name**: Letters and spaces only
- **Phone Number**: 11 digits starting with '09' (e.g., 09244561530)
- **Email**: Valid email format (e.g., user@domain.com)
- **Address**: Minimum 5 characters
- **Birthdate**: DD/MM/YYYY format

## Search Functionality

The search feature performs a recursive search across all contact fields:
- Matches any part of any field
- Case-insensitive searching
- Shows all contacts that match the search term in any field

## Example Usage

1. **Adding a Contact**:
   - Select option 1
   - Enter name (e.g., Juan Dela Cruz)
   - Enter phone number (e.g., 09244561530)
   - Enter email (e.g., juan@email.com)
   - Enter address (e.g., 123 Main St, Manila)
   - Enter birthdate (e.g., 15/03/1990)

2. **Searching**:
   - Select option 2
   - Enter any search term
   - System will show all contacts matching that term in any field

## Notes

- All inputs are validated to ensure data integrity
- Phone numbers are automatically formatted for display
- Table display adjusts dynamically to content length
- The system provides clear error messages for invalid inputs