#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

// Function to check if a string is a keyword
bool isKeyword(const std::string& word) {
	std::vector<std::string> keywords = { "Read", "Print", "If", "Then", "Else", "Endif", "While", "Do", "Endwhile", "Break", "Continue", "Integer", "String", "Main", "Return", "Decl", "Enddecl", "Create", "Open", "Write", "Seek", "Close", "Delete" };
	return std::find(keywords.begin(), keywords.end(), word) != keywords.end();
}

// Function to check if a character is a valid identifier character
bool isValidIdentifierChar(char c) {
	return isalpha(c) || isdigit(c) || c == '_';
}

// Function to tokenize the input file
void tokenizeInput(const std::string& inputFileName, const std::string& outputFileName) {
	std::ifstream inputFile(inputFileName);
	std::ofstream outputFile(outputFileName);

	if (!inputFile.is_open()) {
		std::cerr << "Error opening input file." << std::endl;
		return;
	}

	if (!outputFile.is_open()) {
		std::cerr << "Error opening output file." << std::endl;
		return;
	}

	std::string line;
	int lineNumber = 1;

	while (std::getline(inputFile, line)) {
		std::string token;
		std::string lexeme;

		for (char c : line) {
			if (isspace(c)) {
				if (!token.empty()) {
					if (isKeyword(token)) {
						outputFile << token << std::endl;
					}
					else {
						if (token == "Decl" || token == "Enddecl" || token == "String" || token == "Integer") {
							outputFile << token << std::endl;
						}
						else {
							outputFile << "Identifier, lexeme= " << token << std::endl;
						}
					}
					token.clear();
				}
			}
			else if (c == '#') {
				if (token.empty()) {
					token += c;
				}
				else {
					outputFile << "Error: invalid token before #" << " at line " << lineNumber << std::endl;
					return;
				}
			}
			else if (c == '=') {
				if (!token.empty()) {
					outputFile << "Identifier, lexeme= " << token << std::endl;
					token.clear();
				}
				outputFile << "= " << std::endl;
			}
			else if (c == ';') {
				if (!token.empty()) {
					outputFile << "Identifier, lexeme= " << token << std::endl;
					token.clear();
				}
				outputFile << ";" << std::endl;
			}
			else if (c == '\'') {
				if (!token.empty()) {
					outputFile << "Error: invalid token before ' " << " at line " << lineNumber << std::endl;
					return;
				}
				outputFile << "String_Value, lexeme= " << c;
				while (inputFile.get(c) && c != '\'') {
					outputFile << c;
				}
				outputFile << c << std::endl;
			}
			else if (isValidIdentifierChar(c)) {
				token += c;
			}
			else {
				outputFile << "Error: invalid token " << c << " at line " << lineNumber << std::endl;
				return;
			}
		}

		if (!token.empty()) {
			if (isKeyword(token)) {
				outputFile << token << std::endl;
			}
			else {
				if (token == "Decl" || token == "Enddecl" || token == "String" || token == "Integer") {
					outputFile << token << std::endl;
				}
				else {
					outputFile << "Identifier, lexeme= " << token << std::endl;
				}
			}
		}

		lineNumber++;
	}

	inputFile.close();
	outputFile.close();
}

int main() {
	tokenizeInput("input.txt", "output.txt");
	return 0;
}