#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EXPENSES 200
#define DAYS_IN_A_WEEK 7
#define PASSWORD "admin123"  // Define a default password

typedef struct {
    char date[11];         // Format: DD.MM.YYYY
    char category[20];     // e.g., Food, Rent, Entertainment, etc.
    float amount;          // Amount spent
} Expense;

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;
float totalIncome = 0;
float runningBalance = 0;
float savings = 0;  // New variable to track savings
char currencySymbol[5] = "₹";  // Default currency is INR

// ANSI escape codes for colors
#define RESET_COLOR "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

// Function to get the current date in DD.MM.YYYY format
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d.%02d.%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// Function to set total income in rupees
void setIncome() {
    printf("Enter your total income for the month (in %s):%s ",currencySymbol,  currencySymbol);
    scanf("%f", &totalIncome);
    runningBalance = totalIncome;
    savings = 0;  // Initialize savings
    printf("Your total income is: %s%.2f\n", currencySymbol, totalIncome);
}

// Function to add an expense
void addExpense() {
    if (expenseCount >= MAX_EXPENSES) {
        printf("Expense limit reached!\n");
        return;
    }

    Expense *e = &expenses[expenseCount];
    printf("\nEnter expense details:\n");

    // Automatically filling the date
    getCurrentDate(e->date);
    printf("Date (Automatically filled as %s): %s\n", currencySymbol, e->date);

    // Displaying the new list of categories
    printf("Category(\nFood,\nRent,\nEntertainment,\nBills,\nTransportation,\nLoads,\nHealthcare,\nEducation,\nTax,\nGroceries): ");
    scanf(" %[^\n]", &e->category);
    printf("Amount (in %s):%s",currencySymbol,currencySymbol);
    scanf("%f", &e->amount);

    if (e->amount > runningBalance) {
        printf("Warning: This expense exceeds your remaining balance!\n");
    }

    runningBalance -= e->amount;
    savings = runningBalance;  // Update savings after expense
    expenseCount++;

    // Providing suggestions based on expenses
    if (runningBalance > 0) {
        printf("You have remaining savings: %s%.2f\n", currencySymbol, savings);
    }

    if (runningBalance > totalIncome * 0.15) {
        printf(GREEN "Good job! You are managing your expenses well.\n" RESET_COLOR);
    }

    if (runningBalance <= totalIncome * 0.15 && runningBalance > 0) {
        printf(YELLOW "Caution: You are spending a bit much. Try to save more.\n" RESET_COLOR);
    }

    if (runningBalance <= 0) {
        printf(RED "Warning: You have exceeded your budget! Try to reduce your expenses.\n" RESET_COLOR);
    }

    printf("Expense added successfully! Remaining balance: %s%.2f\n", currencySymbol, runningBalance);
}

// Function to view monthly summary and remaining balance
void viewSummary() {
    float totalExpenses = 0;
    for (int i = 0; i < expenseCount; i++) {
        totalExpenses += expenses[i].amount;
    }

    printf("\nMonthly Summary:\n");
    printf("Total Income     : %s%.2f\n", currencySymbol, totalIncome);
    printf("Total Expenses   : %s%.2f\n", currencySymbol, totalExpenses);
    printf("Remaining Balance: %s%.2f\n", currencySymbol, runningBalance);
    printf("Total Savings    : %s%.2f\n", currencySymbol, savings);  // Display savings

    // Suggestions based on total expenses
    if (totalExpenses < totalIncome) {
        printf(GREEN "Great! You have saved some money.\n" RESET_COLOR);
    } else if (totalExpenses >= totalIncome * 0.85) {
        printf(YELLOW "Warning: You have spent a large portion of your income. Consider saving more.\n" RESET_COLOR);
    } else if (totalExpenses > totalIncome) {
        printf(RED "Warning: Your expenses exceed your income. Try cutting back.\n" RESET_COLOR);
    }

    // Suggest saving more if savings are too low
    if (savings < totalIncome * 0.10) {
        printf(RED "Warning: Your savings are low! Consider cutting back on expenses and saving more.\n" RESET_COLOR);
    }
}

// Function to display weekly summary of expenses
void viewWeeklySummary() {
    float weeklyExpenses = 0;
    char currentDate[11];
    getCurrentDate(currentDate);
    int currentDay = atoi(&currentDate[0]); // Get current day

    // Calculate total expenses for the last 7 days
    for (int i = expenseCount - 1; i >= 0; i--) {
        int expenseDay = atoi(&expenses[i].date[0]); // Get day of the expense date
        if (currentDay - expenseDay < 7) {
            weeklyExpenses += expenses[i].amount;
        } else {
            break;
        }
    }

    printf("\nWeekly Summary:\n");
    printf("Total Weekly Expenses: %s%.2f\n", currencySymbol, weeklyExpenses);
    printf("Remaining Balance: %s%.2f\n", currencySymbol, runningBalance);
    
    printf("\n");
    printf("\nTotal Savings:\n %s%.2f\n", currencySymbol, savings);  // Display weekly savings
}

// Function to analyze spending and give category-specific suggestions
void analyzeSpending() {
    float categoryTotals[10] = {0};
    char *categories[10] = {"Food", "Rent", "Entertainment", "Bills", "Transportation", "Loads", "Healthcare", "Education", "Tax", "Groceries"};

    // Calculate totals for each category
    for (int i = 0; i < expenseCount; i++) {
        for (int j = 0; j < 10; j++) {
            if (strcmp(expenses[i].category, categories[j]) == 0) {
                categoryTotals[j] += expenses[i].amount;
            }
        }
    }

    // Display spending by category
    printf("\nSpending Patterns:\n");
    for (int i = 0; i < 10; i++) {
        printf("%-15s: %s%.2f\n", categories[i], currencySymbol, categoryTotals[i]);
    }

    // Suggestions based on category
    printf("\nSuggestions:\n");
    for (int i = 0; i < 10; i++) {
        if (categoryTotals[i] > 0) {
            printf("You spent %s%.2f on %s. Considering spending less .\n", currencySymbol, categoryTotals[i], categories[i]);
        }
    }
}

// Function to change currency symbol
void changeCurrency() {
    printf("Enter the currency symbol you want to use (e.g., ₹, $, €, £): ");
    scanf(" %s", currencySymbol);
    printf("Currency changed to: %s\n", currencySymbol);
}

// Function to authenticate user with password
int authenticate() {
    char enteredPassword[50];
    printf("Enter password: ");
    scanf("%s", enteredPassword);
    if (strcmp(enteredPassword, PASSWORD) == 0) {
        return 1;  // Authentication successful
    } else {
        printf("Incorrect password!\n");
        return 0;  // Authentication failed
    }
}

int main() {
    // Authenticate user
    if (!authenticate()) {
        return 0;  // Exit the program if authentication fails
    }

    // Allow user to change currency symbol
    changeCurrency();

    // Set total income first
    setIncome();

    while (1) {
        // Ask for expense details
        addExpense();

        // Option to view weekly summary
        char choice;
        printf("\nDo you want to view weekly summary? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            viewWeeklySummary();
        }

        // Ask if the user wants to add another expense
        printf("\nDo you want to add another expense? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    // Display final monthly summary, spending analysis, and suggestions
    viewSummary();
    analyzeSpending();

    return 0; }