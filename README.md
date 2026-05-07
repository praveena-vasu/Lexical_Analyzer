# 🔍 Lexical Analyzer


📜Description

The Lexical Analyzer is a C-based application designed to scan and tokenize C source code into meaningful components such as keywords, identifiers, operators, constants, literals, and special symbols. It simulates the first phase of a compiler by analyzing source code line by line and classifying tokens based on predefined patterns.

The project also detects lexical errors such as invalid characters, unterminated strings, and unmatched parentheses/braces.

🔷Features

  - Tokenize C source code.

  - Identify Keywords, Identifiers, Constants, Operators, and Literals.

  - Handle Single-line (//) and Multi-line (/* */) Comments.

  - Support Multi-character Operators (==, >=, ++, &&, etc.).

  - Detect Lexical Errors and Unmatched Symbols.
  
⚙️Technologies Used

  - C Programming

  - File Handling

  - Structures & Enums

  - String Manipulation

  - Stack Concepts

📍Validation
  
    - Checks for invalid characters.

    - Detects unterminated string and character literals.

    - Validates matching parentheses () and braces {}.

▶️How to Run
  
      gcc main.c lexer.c -

      ./a.out source_code.c


🔶Sample Output

    ------ TOKEN DETAILS ------

    1 | int                  : Keyword
    1 | main                 : Identifier
    1 | (                    : Special Character
    1 | )                    : Special Character
    1 | {                    : Special Character
    2 | int                  : Keyword
    2 | a                    : Identifier
    2 | =                    : Operator
    2 | 10                   : Constant
    2 | ;                    : Special Character

👩‍💻Author
Praveena
