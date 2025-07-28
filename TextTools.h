/**
 * @file TextTools.hpp
 * @brief High performance ASCII string manipulation utilities for C++.
 *
 * This header file contains functions and classes for manipulating ASCII text,
 * including trimming spaces, replacing characters, and more.
 * 
 * For detailed usage examples, visit the documentation:
 * [TextTools Documentation](https://github.com/Ismail-Amir/cpp_ASCII)
 */

#ifndef TEXTTOOLS_H // Ensure this file is included only once in a single translation unit
#define TEXTTOOLS_H // Header guard to prevent multiple inclusions of this file

// Note: The header guard name (TEXTTOOLS_H) should be unique to this header file.
// If you rename this file, be sure to modify this guard to match the new file name.


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <array>
#include <utility>

namespace TextTools {

    // Type Aliases
    using CharModMap = std::unordered_map<char, std::optional<char>>;
    using ReplacementMap = std::unordered_map<char, char>;
    using LookupTable = std::array<char, 256>;

    // Constants
    namespace Constants {
        constexpr char REMOVAL_SENTINEL = '\0';
    }

    // detail namespace for internal implementation
    namespace detail {

        // --- Helper Functions ---
        
        inline LookupTable prepare_identity_table() {
            LookupTable table{};
            for (int i = 0; i < 256; ++i) {
                table[i] = static_cast<char>(i);
            }
            return table;
        }

        // Creates a lookup table based on modification rules
        inline std::pair<LookupTable, bool> create_table_checked(const CharModMap& rules) {
            auto lookup_table = prepare_identity_table();

            if (rules.empty()) {
                return {lookup_table, true};  // Identity table
            }

            for (const auto& [char_to_modify, action] : rules) {
                if (action.has_value()) {
                    lookup_table[static_cast<unsigned char>(char_to_modify)] = action.value();
                } else {
                    lookup_table[static_cast<unsigned char>(char_to_modify)] = Constants::REMOVAL_SENTINEL;
                }
            }

            return {lookup_table, false}; // Table modified
        }

        // Function to replace characters and remove them as needed
        inline void replace_and_remove(std::string& text, const LookupTable& lookup_table, bool is_identity) {
            if (text.empty() || is_identity) return;

            char* write_ptr = text.data();
            const char* const end_ptr = text.data() + text.size();

            for (const char* read_ptr = text.data(); read_ptr < end_ptr; ++read_ptr) {
                const char replacement = lookup_table[static_cast<unsigned char>(*read_ptr)];
                if (replacement != Constants::REMOVAL_SENTINEL) {
                    *write_ptr = replacement;
                    ++write_ptr;
                }
            }

            text.resize(write_ptr - text.data());
        }

        // --- New Implementations ---
        
        inline LookupTable create_replacement_table(const ReplacementMap& replacements) {
            auto replacement_map = prepare_identity_table();
            for (const auto& [char_to_replace, replacement] : replacements) {
                replacement_map[static_cast<unsigned char>(char_to_replace)] = replacement;
            }
            return replacement_map;
        }

        inline void replace_chars(std::string& input_text, const LookupTable& replacement_map) {
            if (input_text.empty()) return;

            for (char& c : input_text) {
                c = replacement_map[static_cast<unsigned char>(c)];
            }
        }
    }  // namespace detail

    // --- Public APIs ---

    // A. Replace and removal objects:
    
    // A.1. Function for one-time character replacement or removal
    inline void ascii_char_replace_remove_once(std::string& text, const CharModMap& rules) {
        auto [lookup_table, is_identity] = detail::create_table_checked(rules);
        detail::replace_and_remove(text, lookup_table, is_identity);
    }

    // A.2. Class for multiple-usage character replacement or removal
    class ReusableASCIICharEditor {
    public:
        explicit ReusableASCIICharEditor(const CharModMap& rules) {
            auto result = detail::create_table_checked(rules);
            m_lookup_table = result.first;
            m_is_identity = result.second;
        }

        void apply(std::string& text) const {
            detail::replace_and_remove(text, m_lookup_table, m_is_identity);
        }

        void operator()(std::string& text) const {
            apply(text);
        }

    private:
        LookupTable m_lookup_table;
        bool m_is_identity;
    };

    // B. Replacement Functions:

    // B.1. Function for one-time character replacement without removal
    /**
     * @brief Replaces ASCII characters from a string without deletion
     * @param[in]   text     The text to be modified
     * @param[in]   replacements    unordered_map of char(s) to be swapped
     * @param 
     * @note convenient for single calls but has performance overhead
     * due to the creation of a temporary lookup table from the map.
     * For performance-critical code, prefer using the ReusableCharReplacer class
     * then .apply(std::string)
     * @warning Do not use it if you want to remove a character. For that purpose,
     * use ascii_char_replace_remove_once function or ReusableASCIICharEditor class
     */
    inline void ascii_replace_once(std::string& text, const ReplacementMap& replacements) {
        if (replacements.empty()) return;

        // Create lookup table and perform replacement
        auto replacement_table = detail::create_replacement_table(replacements);
        detail::replace_chars(text, replacement_table);
    }

    // B.2. Class for multiple-usage character replacement without removal
    /**
     * @brief Replaces ASCII characters from a string for repeated usages
     * 
     * Takes a precreated lookup array as input to replace a character(s) from
     * an ASCII string.
     */
    class ReusableCharReplacer {
    public:
        explicit ReusableCharReplacer(const ReplacementMap& replacements)
                : m_is_empty(replacements.empty()), m_replacement_table(detail::create_replacement_table(replacements)) {}

        void apply(std::string& text) const {
            if (m_is_empty) return;
            detail::replace_chars(text, m_replacement_table);
        }

        void operator()(std::string& text) const {
            apply(text);
        }

    private:
        bool m_is_empty;
        LookupTable m_replacement_table;
    };

    // C Trim Functions
    
    // C.1. TrimmAll Function
    inline void trim_all(std::string& input_text) {
        if (input_text.empty()) return;

        // Compile-time lookup table for trimmable characters
        static constexpr auto make_trimmable_lookup = []() {
            std::array<bool, 256> table{};
            table[' '] = true;
            table['\t'] = true;
            table['\n'] = true;
            table['\r'] = true;
            table['\f'] = true;
            table['\v'] = true;
            table['`'] = true;
            return table;
        };
        static constexpr auto IS_TRIMMABLE = make_trimmable_lookup();

        const char* const first_letter = input_text.data();
        const char* const last_letter = first_letter + input_text.size();

        // Find first non-trimmable character
        const char* first = first_letter;
        while (first < last_letter && IS_TRIMMABLE[static_cast<unsigned char>(*first)]) {
            ++first;
        }

        if (first == last_letter) {
            input_text.clear();
            return;
        }

        // Find last non-trimmable character
        const char* last = last_letter;
        while (last > first && IS_TRIMMABLE[static_cast<unsigned char>(*(last - 1))]) {
            --last;
        }

        // In-place trimming
        char* write_ptr = &input_text[0];
        bool previous_was_trimmable = false;

        for (const char* read_ptr = first; read_ptr < last; ++read_ptr) {
            if (IS_TRIMMABLE[static_cast<unsigned char>(*read_ptr)]) {
                if (!previous_was_trimmable) {
                    *write_ptr++ = ' ';
                    previous_was_trimmable = true;
                }
            } else {
                *write_ptr++ = *read_ptr;
                previous_was_trimmable = false;
            }
        }

        input_text.resize(write_ptr - first_letter);
    }

}  // namespace TextTools




#endif //TEST_ZONE_TEXTTOOLS_H
