# TextTools: Lightweight C++ ASCII Text Manipulation

`TextTools` is a header-only C++ module providing efficient, in-place ASCII string manipulation utilities. It offers functionalities for character replacement, character removal, and comprehensive whitespace trimming and normalization, optimized for performance through the use of lookup tables.

## Features

*   **In-Place Modification**: All functions and classes modify the input `std::string` directly, minimizing memory allocations.
*   **Character Replacement & Removal**:
    *   Replace specific characters with others.
    *   Remove specific characters entirely from a string.
*   **One-Time vs. Reusable Operations**:
    *   Functions for single-use operations where rules are defined on the fly.
    *   Classes for reusable operations, pre-compiling rules into efficient lookup tables for repeated application to multiple strings.
*   **Whitespace Trimming & Normalization**: A powerful `trim_all` function to strip leading/trailing whitespace and consolidate internal multiple whitespace characters into single spaces.
*   **Header-Only**: Easy integration by simply including the header file.
*   **ASCII-Focused**: Designed for efficient manipulation of ASCII characters (0-255).

## Getting Started

Since `TextTools` is a header-only library, there's no need for compilation. Just include the `TEST_ZONE_TEXTTOOLS_H` header file in your C++ project.

1.  **Download**: Clone this repository or copy the `TEST_ZONE_TEXTTOOLS_H` file into your project's include path.
2.  **Include**: Add `#include "TEST_ZONE_TEXTTOOLS_H"` to your C++ source files where you want to use the utilities.

## API Overview & Public Objects Comparison

The `TextTools` namespace provides several public functions and classes, along with type aliases for defining rules. All operations are designed to be efficient, especially the reusable classes which pre-compute transformations into `std::array<char, 256>` lookup tables for O(1) character mapping.

### Key Type Aliases

These define the structure for the rules you pass to the various functions and classes:

*   `TextTools::CharModMap`: `std::unordered_map<char, std::optional<char>>`
    *   **Purpose**: Used to define rules for `ascii_char_replace_remove_once` and `ReusableASCIICharEditor`.
    *   **Usage**: Each entry maps an original character (`char`) to its desired transformation.
        *   `map['a'] = 'A';` : Replaces 'a' with 'A'.
        *   `map['b'] = std::nullopt;` : Removes 'b' from the string.
*   `TextTools::ReplacementMap`: `std::unordered_map<char, char>`
    *   **Purpose**: Used to define rules for `ascii_replace_once` and `ReusableCharReplacer`.
    *   **Usage**: Each entry maps an original character (`char`) to its replacement character (`char`). No character removal is possible with this map.

### Comparison of Public Objects and Their Usage

| Object/Function Name               | Description                                                                                                                                      | Purpose / Best Use Case                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         | Configuration / Input Types                                                                                                                                                                                                 | Example Usage                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| :--------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `TextTools::ascii_char_replace_remove_once` | Modifies an input string in-place by replacing specified characters or removing them entirely, based on a `CharModMap`. Optimized for single use. | When you need to perform a character transformation (replacement and/or removal) on a string, and this specific set of rules will only be applied once or rarely. It avoids the overhead of object creation if not applying rules repeatedly. | `std::string& text`: The string to modify.<br>`const CharModMap& rules`: Rules for replacement/removal.                                                                                                                   | ```cpp<br>std::string s = "hello world";<br>TextTools::CharModMap rules;<br>rules['l'] = std::nullopt; // Remove 'l'<br>rules['o'] = '0'; // Replace 'o' with '0'<br>TextTools::ascii_char_replace_remove_once(s, rules);<br>// s is now "he0 w0rd"<br>```                                                                                                                                                                 |
| `TextTools::ReusableASCIICharEditor`       | A class that pre-compiles character transformation rules (replacement and/or removal) into an efficient lookup table. Its `apply` method (or call operator) can be invoked multiple times. | When you have a fixed set of character transformation rules that need to be applied to many different strings, or to the same string repeatedly. This class pre-calculates the lookup table once, saving computation time on subsequent applications. | Constructor: `explicit ReusableASCIICharEditor(const CharModMap& rules)`<br>Method: `void apply(std::string& text) const` or `operator()(std::string& text) const`.                                                            | ```cpp<br>TextTools::CharModMap editor_rules;<br>editor_rules['e'] = '3';<br>editor_rules['s'] = '$';<br>editor_rules[' '] = std::nullopt; // Remove spaces<br>TextTools::ReusableASCIICharEditor editor(editor_rules);<br><br>std::string s1 = "hello world";<br>editor.apply(s1); // s1 is now "h3ll0w0rld"<br><br>std::string s2 = "test string";<br>editor(s2); // s2 is now "t3$t$tring"<br>```                                                                                              |
| `TextTools::ascii_replace_once`            | Modifies an input string in-place by replacing specified characters with others, based on a `ReplacementMap`. Characters not in the map are unchanged. Optimized for single use.                  | When you need to perform simple character-to-character substitutions on a string without any removal, and this specific set of rules will only be applied once or rarely. Simpler interface than `ascii_char_replace_remove_once` if removal isn't needed. | `std::string& text`: The string to modify.<br>`const ReplacementMap& replacements`: Rules for replacement.                                                                                                                  | ```cpp<br>std::string email = "user@example.com";<br>TextTools::ReplacementMap map_chars;<br>map_chars['.'] = '-';<br>map_chars['@'] = '_';<br>TextTools::ascii_replace_once(email, map_chars);<br>// email is now "user_example-com"<br>```                                                                                                                                                 |
| `TextTools::ReusableCharReplacer`          | A class that pre-compiles character replacement rules into an efficient lookup table. Its `apply` method (or call operator) can be invoked multiple times. Includes no removal functionality.  | When you have a fixed set of simple character replacement rules (no removal) that need to be applied to many different strings, or to the same string repeatedly. This is a more specialized and potentially slightly more efficient version of `ReusableASCIICharEditor` when only replacements are needed. | Constructor: `explicit ReusableCharReplacer(const ReplacementMap& replacements)`<br>Method: `void apply(std::string& text) const` or `operator()(std::string& text) const`.                                                  | ```cpp<br>TextTools::ReplacementMap replacer_rules;<br>replacer_rules['a'] = '@';<br>replacer_rules['s'] = '$';<br>TextTools::ReusableCharReplacer replacer(replacer_rules);<br><br>std::string quote = "This is a simple test string.";<br>replacer.apply(quote);<br>// quote is now "Thi$ i$ @ $imple te$t $tring."<br>```                                                                                              |
| `TextTools::trim_all`                      | Efficiently removes leading/trailing whitespace and replaces any sequence of internal whitespace characters with a single space. Works in-place.     | When you need to normalize spacing in a string. This is useful for user input, parsing, or preparing text for display/storage where consistent internal spacing and removal of external spacing is desired.             | `std::string& input_text`: The string to trim.                                                                                                                                                                                              | <pre><code><br>std::string s1 = "   Hello    World   ";<br>TextTools::trim_all(s1); // s1 is now "Hello World"<br><br>std::string s2 = "   \tMultiple\n\n\t  Spaces\fHere ";<br>TextTools::trim_all(s2); // s2 is now "Multiple Spaces Here"<br><br>std::string s3 = "   ";<br>TextTools::trim_all(s3); // s3 is now ""<br></pre></code>                                                                                                                                                             |

## Code Examples

Here's a comprehensive example demonstrating the use of various `TextTools` functionalities.

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "TEST_ZONE_TEXTTOOLS_H" // Your header file

int main() {
    // --- 1. Using ascii_char_replace_remove_once (single-use replace/remove) ---
    std::cout << "--- ascii_char_replace_remove_once ---" << std::endl;
    std::string text1 = "Hello, world! This is a test.";
    TextTools::CharModMap rules1;
    rules1['o'] = '0';          // Replace 'o' with '0'
    rules1[' '] = std::nullopt; // Remove spaces
    rules1['i'] = '1';          // Replace 'i' with '1'
    rules1['!'] = '*';          // Replace '!' with '*'

    std::cout << "Original: \"" << text1 << "\"" << std::endl;
    TextTools::ascii_char_replace_remove_once(text1, rules1);
    std::cout << "Modified: \"" << text1 << "\"" << std::endl; // Expected: "Hell0,w0rld*Th1s1saTest."
    std::cout << std::endl;

    // --- 2. Using ReusableASCIICharEditor (multi-use replace/remove) ---
    std::cout << "--- ReusableASCIICharEditor ---" << std::endl;
    TextTools::CharModMap editor_rules;
    editor_rules['A'] = '@';
    editor_rules['E'] = '3';
    editor_rules['I'] = '1';
    editor_rules['O'] = '0';
    editor_rules['U'] = '#';
    editor_rules['Z'] = std::nullopt; // Remove Z

    TextTools::ReusableASCIICharEditor case_editor(editor_rules);

    std::string s2_1 = "HELLO AMAZING WORLD ZONE";
    std::string s2_2 = "QUICK BROWN FOX JUMPS";

    std::cout << "Original 1: \"" << s2_1 << "\"" << std::endl;
    case_editor.apply(s2_1); // Using apply method
    std::cout << "Modified 1: \"" << s2_1 << "\"" << std::endl; // Expected: "H3LL0 @M@Z1NG W0RLD "

    std::cout << "Original 2: \"" << s2_2 << "\"" << std::endl;
    case_editor(s2_2); // Using operator()
    std::cout << "Modified 2: \"" << s2_2 << "\"" << std::endl; // Expected: "Q#1CK BR0WN F0X J#MP$" (if $ was in rules) -> "Q#1CK BR0WN F0X J#MPS"
    std::cout << std::endl;

    // --- 3. Using ascii_replace_once (single-use replace only) ---
    std::cout << "--- ascii_replace_once ---" << std::endl;
    std::string text3 = "The quick brown fox jumps over the lazy dog.";
    TextTools::ReplacementMap rules3;
    rules3['e'] = 'X';
    rules3['o'] = 'Y';
    rules3[' '] = '_';

    std::cout << "Original: \"" << text3 << "\"" << std::endl;
    TextTools::ascii_replace_once(text3, rules3);
    std::cout << "Modified: \"" << text3 << "\"" << std::endl; // Expected: "ThX_qXick_brYwn_fYx_jumps_YvXr_thX_l@zy_dYg."
    std::cout << std::endl;

    // --- 4. Using ReusableCharReplacer (multi-use replace only) ---
    std::cout << "--- ReusableCharReplacer ---" << std::endl;
    TextTools::ReplacementMap replacer_rules;
    replacer_rules['a'] = '@';
    replacer_rules['s'] = '$';
    replacer_rules['t'] = '+';

    TextTools::ReusableCharReplacer replacer(replacer_rules);

    std::string s4_1 = "This is a test string.";
    std::string s4_2 = "Another fantastic example.";

    std::cout << "Original 1: \"" << s4_1 << "\"" << std::endl;
    replacer.apply(s4_1);
    std::cout << "Modified 1: \"" << s4_1 << "\"" << std::endl; // Expected: "+hi$ i$ @ +e$+ $tring."

    std::cout << "Original 2: \"" << s4_2 << "\"" << std::endl;
    replacer(s4_2);
    std::cout << "Modified 2: \"" << s4_2 << "\"" << std::endl; // Expected: "@no+her f@n+@$+ic ex@mple."
    std::cout << std::endl;

    // --- 5. Using trim_all ---
    std::cout << "--- trim_all ---" << std::endl;
    std::string text5_1 = "   Hello    World   ";
    std::string text5_2 = "\t\n\r   Leading and trailing   \t\n\r";
    std::string text5_3 = "  Multiple   \t whitespace \n here   ";
    std::string text5_4 = "        "; // Only whitespace
    std::string text5_5 = "NoChange";

    std::cout << "Original 1: \"" << text5_1 << "\"" << std::endl;
    TextTools::trim_all(text5_1);
    std::cout << "Modified 1: \"" << text5_1 << "\"" << std::endl; // Expected: "Hello World"

    std::cout << "Original 2: \"" << text5_2 << "\"" << std::endl;
    TextTools::trim_all(text5_2);
    std::cout << "Modified 2: \"" << text5_2 << "\"" << std::endl; // Expected: "Leading and trailing"

    std::cout << "Original 3: \"" << text5_3 << "\"" << std::endl;
    TextTools::trim_all(text5_3);
    std::cout << "Modified 3: \"" << text5_3 << "\"" << std::endl; // Expected: "Multiple whitespace here"
    
    std::cout << "Original 4: \"" << text5_4 << "\"" << std::endl;
    TextTools::trim_all(text5_4);
    std::cout << "Modified 4: \"" << text5_4 << "\"" << std::endl; // Expected: ""

    std::cout << "Original 5: \"" << text5_5 << "\"" << std::endl;
    TextTools::trim_all(text5_5);
    std::cout << "Modified 5: \"" << text5_5 << "\"" << std::endl; // Expected: "NoChange"
    std::cout << std::endl;

    return 0;
}
