#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

#define MAX_TOKEN_LENGTH 100

struct symbol_table {
    char *lexeme;
    char *token;
    int position; // Add a position column to store the token's position
    symbol_table *next;
};

bool oper(const char *str) {
    if (!strcmp(str, "+") || !strcmp(str, "-") || !strcmp(str, "*") || !strcmp(str, ">") || !strcmp(str, "<") || !strcmp(str, "/") || !strcmp(str, "="))
        return true;
    return false;
}

bool iden(const char *str) {
    if (str[0] >= '0' && str[0] <= '9')
        return false;
    return true;
}

bool keyw(const char *str) {
    const char *keywords[] = {"if", "else", "while", "do", "break", "continue", "int", "double", "float", "return", "char", "case", "sizeof", "long", "short", "typedef", "switch", "unsigned", "void", "static", "struct", "goto"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        if (!strcmp(str, keywords[i]))
            return true;
    }
    return false;
}

bool symbol(const char *str) {
    if (!strcmp(str, "(") || !strcmp(str, ")"))
        return true;
    return false;
}

bool no(const char *str) {
    int i, len = strlen(str);
    bool hasDecimal = false;
    if (len == 0)
        return false;
    for (i = 0; i < len; i++) {
        if ((str[i] < '0' || str[i] > '9') && (str[i] != '.' || (str[i] == '-' && i > 0)))
            return false;
        if (str[i] == '.')
            hasDecimal = true;
    }
    return hasDecimal;
}

void insert_into_symbol_table(symbol_table *table, const char *lexeme, const char *token, int position) {
    symbol_table *new_entry = (symbol_table *)malloc(sizeof(symbol_table));
    new_entry->lexeme = strdup(lexeme);
    new_entry->token = strdup(token);
    new_entry->position = position;
    new_entry->next = table->next;
    table->next = new_entry;
}

const char *search_symbol_table(symbol_table *table, const char *lexeme) {
    symbol_table *current_entry = table->next;
    while (current_entry != NULL) {
        if (strcmp(current_entry->lexeme, lexeme) == 0) {
            return current_entry->token;
        }
        current_entry = current_entry->next;
    }
    return nullptr;
}


struct TreeNode {
    std::string value;
    int precedence; // Operator precedence level
    TreeNode *left;
    TreeNode *right;
};

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int getOperatorPrecedence(char op) {
    if (op == '$') return 6; // Lowest precedence
    if (op == '+') return 4;
    if (op == '-') return 5;
    if (op == '*') return 3;
    if (op == '/') return 2;
    return 1; // Variables have the highest precedence
}

TreeNode *createNode(std::string value, int precedence) {
    TreeNode *node = new TreeNode;
    node->value = value;
    node->precedence = precedence;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

TreeNode *buildSyntaxTree(std::string expression) {
    TreeNode *root = nullptr;
    TreeNode *current = nullptr;
    for (int i = 0; i < expression.length(); i++) {
        std::string currentValue(1, expression[i]);
        if (isOperator(expression[i])) {
            current = createNode(currentValue, getOperatorPrecedence(expression[i]));
            current->right = createNode(std::string(1, expression[i + 1]), getOperatorPrecedence(expression[i + 1])); // Create the right node
            current->left = root;
            root = current;
            i++; // Skip the next character
        } else {
            if (isalnum(expression[i])) {
                if (current == nullptr) {
                    root = createNode(currentValue, 1); // Variables have the highest precedence
                } else {
                    if (current->right == nullptr) {
                        current->right = createNode(currentValue, 1);
                    } else {
                        TreeNode *temp = createNode(currentValue, 1);
                        temp->left = current->right;
                        current->right = temp;
                    }
                }
            }
        }
    }
    return root;
}

void printSyntaxTree(TreeNode *node, std::string indent = "", bool isRight = false) {
    if (node != nullptr) {
        std::cout << indent;
        if (isRight) {
            std::cout << "R---- ";
            indent += "     ";
        } else {
            std::cout << "L---- ";
            indent += "|    ";
        }

        std::cout << node->value << " (Precedence: " << node->precedence << ")" << std::endl;

        printSyntaxTree(node->left, indent, false);
        printSyntaxTree(node->right, indent, true);
    }
}

void printOperatorPrecedenceTable() {
    std::cout << std::endl;
    std::cout << "Operator Precedence Table:" << std::endl;
    std::cout << std::endl;
    std::cout << "Operator  | Precedence Level" << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Variables | 1 (Highest)" << std::endl;
    std::cout << "/         | 2" << std::endl;
    std::cout << "*         | 3" << std::endl;
    std::cout << "+         | 4" << std::endl;
    std::cout << "-         | 5" << std::endl;
    std::cout << "$         | 6 (Lowest)" << std::endl;
}

std::vector<std::pair<int, int>> updatevec(std::string s) {
    std::vector<std::pair<int, int>> ans;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '%') {
            ans.push_back({1, i});
        } else if (s[i] == '/') {
            ans.push_back({2, i});
        } else if (s[i] == '*') {
            ans.push_back({3, i});
        } else if (s[i] == '+') {
            ans.push_back({4, i});
        } else if (s[i] == '-') {
            ans.push_back({5, i});
        }
    }
    sort(ans.begin(), ans.end());
    return ans;
}

int main() {
    int choice;
    do {
        std::cout<<"\n\n\n";
        std::cout<<"------------------------------------------\n";
        std::cout << "                  MENU               \n";
        std::cout<<"------------------------------------------\n";
        std::cout << "1. Lexical Analyzer\n";
        std::cout << "2. Syntax Analyzer\n";
        std::cout << "3. Intermediate Code Generator\n";
        std::cout << "4. Exit\n";
        std::cout<<"------------------------------------------\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout<<"\n";
        switch (choice) {
            case 1: {
                char str[MAX_TOKEN_LENGTH];
                std::cout << "Enter an expression: ";
                std::cin.ignore();
                std::cin.getline(str, sizeof(str));
                symbol_table table;
                table.next = nullptr;
                char delims[] = " ";
                char *token = strtok(str, delims);

                std::map<std::string, int> tokenFrequency;
                int position = 1; // Initialize the position

                while (token != nullptr) {
                    const char *token_type = nullptr;
                    if (oper(token)) {
                        token_type = "Operator";
                    } else if (no(token)) {
                        token_type = "Integer";
                    } else if (symbol(token)) {
                        token_type = "Symbol";
                    } else if (keyw(token)) {
                        token_type = "Keyword";
                    } else if (iden(token)) {
                        token_type = "Identifier";
                    } else {
                        token_type = "Constant";
                    }
                    insert_into_symbol_table(&table, token, token_type, position);
                    tokenFrequency[token_type]++;
                    token = strtok(nullptr, delims);
                    position++; // Increment the position
                }
                std::cout << "\n\nSymbol Table:\n";
                std::cout << "-------------------------------------------";
                std::cout << "\nPosition\tLexeme\t\tToken\n";
                std::cout << "-------------------------------------------\n";

                symbol_table *current_entry = table.next;
                while (current_entry != nullptr) {
                    std::cout << current_entry->position << "\t\t" << current_entry->lexeme << "\t\t" << current_entry->token << std::endl;
                    current_entry = current_entry->next;
                }

                std::cout << "\n\nToken Frequencies:\n\n";
                for (const auto &pair : tokenFrequency)
                    {
                std::cout << pair.first << ": " << pair.second << std::endl;
                }

                break;
            }

            case 2: {
                std::string expression;
                std::cout << "Enter an expression: ";
                std::cin.ignore();
                std::getline(std::cin, expression);
                TreeNode *syntaxTree = buildSyntaxTree(expression);
                std::cout << "\nSyntax Tree:\n";
                printSyntaxTree(syntaxTree);
                printOperatorPrecedenceTable();
                break;
            }

            case 3: {
                std::string s;
                std::cout << "Enter the Expression: ";
                std::cin >> s;
                int check = 0;
                std::vector<std::pair<int, int>> v;
                v = updatevec(s);
                char temptoadd = 'A';

                try {
                    while (v.size() != 0 && s.size() > 3) {
                        std::string temp = "";
                        temp.push_back(s[v[0].second - 1]);
                        if (v[0].first == 1) {
                            temp.push_back('%');
                            if (s[v[0].second + 1] == '0') {
                                throw std::runtime_error("Division by zero");
                            }
                        } else if (v[0].first == 2) {
                            temp.push_back('/');
                            if (s[v[0].second + 1] == '0') {
                                throw std::runtime_error("Division by zero");
                            }
                        } else if (v[0].first == 3) {
                            temp.push_back('*');
                        } else if (v[0].first == 4) {
                            temp.push_back('+');
                        } else {
                            temp.push_back('-');
                        }
                        temp.push_back(s[v[0].second + 1]);
                        std::string temporary = "";
                        temporary.push_back(temptoadd);
                        temporary = temporary + "=" + temp;
                        std::cout << temporary << "\t\t";
                        std::string str = "";
                        for (int i = 0; i < v[0].second - 1; i++) {
                            str.push_back(s[i]);
                        }
                        str.push_back(temptoadd);
                        for (int i = v[0].second + 2; i < s.size(); i++) {
                            str.push_back(s[i]);
                        }
                        s = str;
                        std::cout << s << std::endl;
                        v = updatevec(s);
                        temptoadd++;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 4: {
                std::cout << "Exiting the program.\n";
                break;
            }

            default: {
                std::cout << "Invalid choice. Please select a valid option.\n";
            }
        }
    } while (choice != 4);

    return 0;
}
