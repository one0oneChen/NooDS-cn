/*
    NooDS-cn i18n module
    Bilingual (Chinese/English) translation support for the GUI.

    The module is intentionally lightweight: it only depends on the C++
    standard library, so it can be linked from the desktop, console, and
    Android front-ends without extra dependencies.
*/

#pragma once

#include <string>
#include <unordered_map>

class I18n {
public:
    enum Language {
        LANG_EN = 0,
        LANG_ZH = 1
    };

    static I18n& instance();

    void setLanguage(Language lang);
    Language getLanguage() const { return currentLang; }
    void setLanguageFromInt(int value);

    const char* t(const char* key) const;
    std::string ts(const std::string &key) const;

private:
    I18n();
    I18n(const I18n&) = delete;
    I18n& operator=(const I18n&) = delete;

    Language currentLang;
    std::unordered_map<std::string, std::string> en;
    std::unordered_map<std::string, std::string> zh;

    void registerTranslations();
    void add(const char* key, const char* english, const char* chinese);
};

#define TR_I18N(key) (I18n::instance().t(key))