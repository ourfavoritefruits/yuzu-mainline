// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "common/string_util.h"
#include "common/swap.h"
#include "core/file_sys/control_metadata.h"

namespace FileSys {

const std::array<const char*, 15> LANGUAGE_NAMES = {
    "AmericanEnglish", "BritishEnglish", "Japanese",
    "French",          "German",         "LatinAmericanSpanish",
    "Spanish",         "Italian",        "Dutch",
    "CanadianFrench",  "Portugese",      "Russian",
    "Korean",          "Taiwanese",      "Chinese",
};

std::string LanguageEntry::GetApplicationName() const {
    return Common::StringFromFixedZeroTerminatedBuffer(application_name.data(), 0x200);
}

std::string LanguageEntry::GetDeveloperName() const {
    return Common::StringFromFixedZeroTerminatedBuffer(developer_name.data(), 0x100);
}

NACP::NACP(VirtualFile file) : raw(std::make_unique<RawNACP>()) {
    file->ReadObject(raw.get());
}

NACP::~NACP() = default;

const LanguageEntry& NACP::GetLanguageEntry(Language language) const {
    if (language != Language::Default) {
        return raw->language_entries.at(static_cast<u8>(language));
    }

    for (const auto& language_entry : raw->language_entries) {
        if (!language_entry.GetApplicationName().empty())
            return language_entry;
    }

    // Fallback to English
    return GetLanguageEntry(Language::AmericanEnglish);
}

std::string NACP::GetApplicationName(Language language) const {
    return GetLanguageEntry(language).GetApplicationName();
}

std::string NACP::GetDeveloperName(Language language) const {
    return GetLanguageEntry(language).GetDeveloperName();
}

u64 NACP::GetTitleId() const {
    return raw->title_id;
}

std::string NACP::GetVersionString() const {
    return Common::StringFromFixedZeroTerminatedBuffer(raw->version_string.data(), 0x10);
}
} // namespace FileSys
