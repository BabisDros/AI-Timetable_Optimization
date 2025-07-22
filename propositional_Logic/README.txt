Prerequisites 

1. Download the repository

For ForwardChaining_FirstOrder:
2. Open a Command Prompt (cmd) and navigate to the folder containing the Java files using the cd command. If you haven’t renamed the folder, the path will be something like:
        ...\ForwardChaining_FirstOrder
3. Compile with 
        javac FOL.java
4. run with 
        java FOL
5. You will see the following menu:
        What would you like to do?
        0. Exit
        1. Enter a sentence to prove

6. If you choose Option 1:
        1. You will be prompted to enter a filename (without the .txt extension), or simply press Enter to load the default file: KB_FOL.txt

        2. The file must contain knowledge base (KB) rules in the following format:
                NOTMissile(x) OR NOTOwns(Nono, x) OR Sells(West, x, Nono)
                NOTMissile(x) OR Weapon(x)
                NOTEnemy(x, America) OR Hostile(x)
		...
                Rules for formatting:
                1. Each sentence must be on a separate line.
                2. OR must be written in uppercase with spaces on both sides: " OR ".
                3. NOT must be in uppercase, directly before the predicate name (no space).
                4. Arguments (constants or variables) must be comma-separated with a space after each comma, e.g.,
                        Sells(West, x, Nono)

        3. Next, you’ll be prompted to enter a query, which must follow the same format, e.g.:
		Missile(x) or with more variables, just like number 4 above.

===============================================================================================================================

For ForwardChaining_Propositional:
2. Open a Command Prompt (cmd) and navigate to the folder containing the Java files using cd. If you haven’t renamed the folder, the path will be something like:
        ...\ForwardChaining_Propositional
3. Compile with
        javac PL_FC_Entails.java
4. Run with
        java PL_FC_Entails
5. You will see the following menu:
        What would you like to do?
        0. Exit
        1. Enter a sentence to prove

6. If you choose Option 1:
        1. You can enter a filename (without the .txt extension), or press Enter to load the default file: horn_clauses.txt

        2. The file must contain propositional logic rules in the following format:
                (¬P∨Q)
                (¬L∨¬M∨P)
                ..

		Rules for formatting:
                1. Each sentence must be on a separate line.
                2. No spaces are allowed between symbols.
                3. You can copy and paste special characters (e.g., ¬, ∨) from this document for convenience.
                        (Beware V != ∨)

        3. You will then be prompted to enter a query, using the same symbol format, e.g.:
                Q
