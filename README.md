William Cumming WPC30

README
Below is a listed description of our spellchecker program

hashWord- takes a word as input and calculates its hash value based on the first character of the word. It is used to determind the index in our hash table "wordMap" where the word will be stored to be searched

makeWordNode- Allocates memory for each new WordNode which are structs containing the word and the location of the next word in the linked list

insertWord- inserts a word into the hash table ensuring the words are inserted in lexicographical order within their linked lists. It uses makeWordNode to create the new node then finds the correct position for the word

loadDict- Opens and reads a dictionary file specified as one of the args. It inserts both the original word case specific and its uppercase version into the hashtable using insertWord. This is used later to compare with text files for spellchecking

traverseDir- this function is how we navigate the directories starting from the given path in args. Each .txt file it encounters calls processFile to perform the main function of the code, spellchecking.

reportSpellingError- this prints an error message indicating a spelling error in a file. It provides the file path, line number, column number, and mispelled word.

matchesCapitalizationRules- this function is a boolean meant to help the code adhere to the capitalization rules given in the assignment pdf

findWord- Searches for a specific word in the hashtable, it considers capitalization using the afformentioned boolean function to check if the word matches a word from the dictionary and returns a pointer to the WordNode in the hash table if found otherwise it returns NULL.

isAlphaorHyphen- determins whether a character is alphabetical or a special character.

cleanAndCheckWord- trims the trailing non-alphabetic characters then checks if the word is spelled correctly using findWord and reports any errors with reportSpellingError

extractWords- Reads a given txt file extracts and checks the words spelling. It also handles hyphenated words, tracks lie and column numbers for error reporting, and handles comma spacing errors

processFile- This file initiates the spell check process for a single file it then prints a message indicating the file being processed and calls extractWords to perform the spell checking

main- this function initializes the hashtable, loads the dictionary and processes each file or directory specified in the args.

Test Cases

Test 1: Basic Spellchecking (test1.txt)
Description: This test verifies basic spellchecking capabilities, including handling of capital letters and punctuation.
Expected Behavior: spchk should correctly identify all words as spelled correctly, given a comprehensive dictionary. The test includes variations in capitalization to ensure case-insensitivity except for proper handling of initial capitals.

Test 2: Proper Nouns and Capitalization (test2.txt)
Description: Tests proper noun recognition and sensitivity to initial capitalization.
Expected Behavior: Proper nouns like "MacDonald" should be recognized in their exact form or in all uppercase. However, "macdonald" should not be considered correct unless explicitly included in the dictionary.

Test 3: Punctuation Handling (test3.txt)
Description: Focuses on spchk's ability to handle punctuation correctly, including quotes, parentheses, and commas.
Expected Behavior: spchk should ignore punctuation at the beginning and end of words, allowing for correct identification of words. Commas without a following space should result in an error.

Test 4: Case Sensitivity (test4.txt)
Description: Evaluates the spellchecker's handling of case sensitivity and variations in capitalization.
Expected Behavior: Words should match their dictionary entries in a case-insensitive manner, except when a word is capitalized differently in the dictionary.

Test 7: Quotation Marks (test7.txt)
Description: Tests the handling of words enclosed in quotation marks.
Expected Behavior: Words within quotes should be recognized correctly, with the quotation marks themselves being ignored.

Test 8: Special Characters (test8.txt)
Description: This test checks how spchk deals with words that contain special characters not present in the basic ASCII character set.
Expected Behavior: Given that spchk operates on ASCII text, it should ignore diacritical marks, treating "cafe" and "café" as distinct words unless both forms are present in the dictionary.

Test 9: Technical Terms (test9.txt)
Description: Evaluates the spellchecker's performance with technical terms and neologisms.
Expected Behavior: Technical terms should be checked against the dictionary, and any unrecognized terms should be reported as spelling errors.

Test 10: Common Misspellings (test10.txt)
Description: Contains commonly misspelled words to test the spellchecker's effectiveness.
Expected Behavior: Each misspelled word should be identified as incorrect, assuming the correct spellings are present in the dictionary.

Test 14: Acronyms and Abbreviations (test14.txt)
Description: Tests the recognition of acronyms and abbreviations, which are often written in all caps.
Expected Behavior: Acronyms and abbreviations should be recognized based on their presence in the dictionary, regardless of capitalization.

Test 15: Complex Sentences (test15.txt)
Description: A complex sentence with multiple punctuation marks and a high-level vocabulary to test the spellchecker's robustness.
Expected Behavior: spchk should accurately identify any misspelled words while correctly handling punctuation and capitalization.
