#pragma once

namespace SimpleEspConfigurator {
    static auto SEC_LOG_TAG = "SEC";
    static auto SEC_PREF_NS = "SimpleEspConfig";
    constexpr auto DEFAULT_TIMEOUT_MS = 10000;

    static const char *formatBool(const bool value) {
        return value ? "true" : "false";
    }


    static std::string escapeHtml(const std::string &value) {
        std::string result;
        for (std::size_t i = 0; i < value.length(); ++i) {
            char c = value[i];
            if (c == '<') {
                result += "&lt;";
            } else if (c == '&') {
                result += "&amp;";
            } else if (c == '"') {
                result += "&quot;";
            } else if (c == '\'') {
                result += "&apos;";
            } else {
                result += c;
            }
        }
        return result;
    }

    template<std::size_t N>
    struct TemplateString {
        char value[N];

        constexpr TemplateString(const char (&str)[N]) {
            for (std::size_t i = 0; i < N; ++i)
                value[i] = str[i];
        }

        constexpr TemplateString() : value{} {
        }

        constexpr operator const char *() const {
            return value;
        }

        constexpr operator const std::string &() const {
            return std::string(value);
        }

        // 1. Combine with another FixedString
        template<std::size_t M>
        constexpr auto operator+(const TemplateString<M> &rhs) const {
            // Result size is N + M - 1 (to avoid two null terminators)
            TemplateString<N + M - 1> result{};

            // Copy first string (excluding its null terminator)
            for (std::size_t i = 0; i < N - 1; ++i) {
                result.value[i] = this->value[i];
            }

            // Copy second string (including its null terminator)
            for (std::size_t i = 0; i < M; ++i) {
                result.value[N - 1 + i] = rhs.value[i];
            }

            return result;
        }

        // 2. Combine with a raw char array (string literal)
        template<std::size_t M>
        constexpr auto operator+(const char (&rhs)[M]) const {
            // Convert the raw array to a FixedString and reuse the operator above
            return *this + TemplateString<M>(rhs);
        }

        [[nodiscard]] constexpr const char *c_str() const {
            return value;
        }
    };
} // namespace SimpleEspConfigurator
