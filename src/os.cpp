﻿/*************************************************************************************************************

 Limelight Framework - SystemInfo Utils


 Copyright 2016 mvd

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software distributed under the License is
 distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and limitations under the License.

*************************************************************************************************************/

#include "systeminfo/os.h"

#include <array>


namespace
{
  using namespace ll::systeminfo::os;
  
  static const 
  std::array< std::string, static_cast<size_t>( language::language_code::other ) + 1 > language_codes = {
    "?",
    "aa",
    "ab",
    "ae",
    "af",
    "ak",
    "am",
    "an",
    "ar",
    "as",
    "av",
    "ay",
    "az",
    "ba",
    "be",
    "bg",
    "bh",
    "bi",
    "bm",
    "bn",
    "bo",
    "br",
    "bs",
    "ca",
    "ce",
    "ch",
    "co",
    "cr",
    "cs",
    "cu",
    "cv",
    "cy",
    "da",
    "de",
    "dv",
    "dz",
    "ee",
    "el",
    "en",
    "eo",
    "es",
    "et",
    "eu",
    "fa",
    "ff",
    "fi",
    "fj",
    "fo",
    "fr",
    "fy",
    "ga",
    "gd",
    "gl",
    "gn",
    "gu",
    "gv",
    "ha",
    "he",
    "hi",
    "ho",
    "hr",
    "ht",
    "hu",
    "hy",
    "hz",
    "id",
    "ig",
    "ii",
    "ik",
    "is",
    "it",
    "iu",
    "ja",
    "jv",
    "ka",
    "kg",
    "ki",
    "kj",
    "kk",
    "kl",
    "km",
    "kn",
    "ko",
    "kr",
    "ks",
    "ku",
    "kv",
    "kw",
    "ky",
    "la",
    "lb",
    "lg",
    "li",
    "ln",
    "lo",
    "lt",
    "lu",
    "lv",
    "mg",
    "mh",
    "mi",
    "mk",
    "ml",
    "mn",
    "mr",
    "ms",
    "mt",
    "my",
    "na",
    "nb",
    "nd",
    "ne",
    "ng",
    "nl",
    "nn",
    "no",
    "nr",
    "nv",
    "ny",
    "oc",
    "oj",
    "om",
    "or",
    "os",
    "pa",
    "pi",
    "pl",
    "ps",
    "pt",
    "qu",
    "rm",
    "rn",
    "ro",
    "ru",
    "rw",
    "sa",
    "sc",
    "sd",
    "se",
    "sg",
    "si",
    "sk",
    "sl",
    "sm",
    "sn",
    "so",
    "sq",
    "sr",
    "ss",
    "st",
    "su",
    "sv",
    "sw",
    "ta",
    "te",
    "tg",
    "th",
    "ti",
    "tk",
    "tl",
    "tn",
    "to",
    "tr",
    "ts",
    "tt",
    "tw",
    "ty",
    "ug",
    "uk",
    "ur",
    "uz",
    "ve",
    "vi",
    "vo",
    "wa",
    "wo",
    "xh",
    "yi",
    "yo",
    "za",
    "zh",
    "zu",
    "?"
  };

  static const 
  std::array< std::string, static_cast<size_t>( language::country_code::other ) + 1 > country_codes = {
    "?",      
    "AD",
    "AE",
    "AF",
    "AG",
    "AI",
    "AL",
    "AM",
    "AO",
    "AQ",
    "AR",
    "AS",
    "AT",
    "AU",
    "AW",
    "AX",
    "AZ",
    "BA",
    "BB",
    "BD",
    "BE",
    "BF",
    "BG",
    "BH",
    "BI",
    "BJ",
    "BL",
    "BM",
    "BN",
    "BO",
    "BQ",
    "BR",
    "BS",
    "BT",
    "BV",
    "BW",
    "BY",
    "BZ",
    "CA",
    "CC",
    "CD",
    "CF",
    "CG",
    "CH",
    "CI",
    "CK",
    "CL",
    "CM",
    "CN",
    "CO",
    "CR",
    "CU",
    "CV",
    "CW",
    "CX",
    "CY",
    "CZ",
    "DE",
    "DJ",
    "DK",
    "DM",
    "DO",
    "DZ",
    "EC",
    "EE",
    "EG",
    "EH",
    "ER",
    "ES",
    "ET",
    "FI",
    "FJ",
    "FK",
    "FM",
    "FO",
    "FR",
    "GA",
    "GB",
    "GD",
    "GE",
    "GF",
    "GG",
    "GH",
    "GI",
    "GL",
    "GM",
    "GN",
    "GP",
    "GQ",
    "GR",
    "GS",
    "GT",
    "GU",
    "GW",
    "GY",
    "HK",
    "HM",
    "HN",
    "HR",
    "HT",
    "HU",
    "ID",
    "IE",
    "IL",
    "IM",
    "IN",
    "IO",
    "IQ",
    "IR",
    "IS",
    "IT",
    "JE",
    "JM",
    "JO",
    "JP",
    "KE",
    "KG",
    "KH",
    "KI",
    "KM",
    "KN",
    "KP",
    "KR",
    "KW",
    "KY",
    "KZ",
    "LA",
    "LB",
    "LC",
    "LI",
    "LK",
    "LR",
    "LS",
    "LT",
    "LU",
    "LV",
    "LY",
    "MA",
    "MC",
    "MD",
    "ME",
    "MF",
    "MG",
    "MH",
    "MK",
    "ML",
    "MM",
    "MN",
    "MO",
    "MP",
    "MQ",
    "MR",
    "MS",
    "MT",
    "MU",
    "MV",
    "MW",
    "MX",
    "MY",
    "MZ",
    "NA",
    "NC",
    "NE",
    "NF",
    "NG",
    "NI",
    "NL",
    "NO",
    "NP",
    "NR",
    "NU",
    "NZ",
    "OM",
    "PA",
    "PE",
    "PF",
    "PG",
    "PH",
    "PK",
    "PL",
    "PM",
    "PN",
    "PR",
    "PS",
    "PT",
    "PW",
    "PY",
    "QA",
    "RE",
    "RO",
    "RS",
    "RU",
    "RW",
    "SA",
    "SB",
    "SC",
    "SD",
    "SE",
    "SG",
    "SH",
    "SI",
    "SJ",
    "SK",
    "SL",
    "SM",
    "SN",
    "SO",
    "SR",
    "SS",
    "ST",
    "SV",
    "SX",
    "SY",
    "SZ",
    "TC",
    "TD",
    "TF",
    "TG",
    "TH",
    "TJ",
    "TK",
    "TL",
    "TM",
    "TN",
    "TO",
    "TR",
    "TT",
    "TV",
    "TW",
    "TZ",
    "UA",
    "UG",
    "UM",
    "US",
    "UY",
    "UZ",
    "VA",
    "VC",
    "VE",
    "VG",
    "VI",
    "VN",
    "VU",
    "WF",
    "WS",
    "YE",
    "YT",
    "ZA",
    "ZM",
    "ZW",
    "other"
    };
}


namespace ll
{
  using namespace ll::systeminfo;

  std::string to_string( os::family f_ )
  {
    switch ( f_ )
    {
    case os::family::windows:
      return "Windows";
    case os::family::osx:
      return "OSX";
    case os::family::linux:
      return "Linux";
    default:
      return "Unknown";
    }
  }

  
  // ---------------------------------------------------------------------------------------------------------

  std::string to_string( os::architecture a_ )
  {
    return ( a_ == os::architecture::x64 ) ? "x64" : "x86";
  }


  // ---------------------------------------------------------------------------------------------------------

  std::string to_string( const version_t& v_ )
  {
    auto result =
      std::to_string( std::get< 0 >( v_ ) ) +
      "." +
      std::to_string( std::get< 1 >( v_ ) ) +
      "." +
      std::to_string( std::get< 2 >( v_ ) )
      ;

    if ( !std::get< 3 >( v_ ).empty() )
    {
      result += ".";
      result += std::get< 3 >( v_ );
    }

    return result;
  }

  
  // ---------------------------------------------------------------------------------------------------------

  std::string to_string( os::language l_ )
  {
    return to_language_name( l_.code ) + " (" + to_country_name( l_.country ) + ")";
  }


  // ---------------------------------------------------------------------------------------------------------

  std::string to_string( os::language::language_code l_ )
  {
    return ::language_codes[static_cast<size_t>( l_ )];
  }

  
  // ---------------------------------------------------------------------------------------------------------

  std::string to_string( os::language::country_code l_ )
  {
    return ::country_codes[static_cast<size_t>( l_ )];
  }


  // ---------------------------------------------------------------------------------------------------------

  std::string to_ietf_code( os::language l_ )
  {
    return to_string( l_.code ) + "-" + to_string( l_.country );
  }
  

  
  // ---------------------------------------------------------------------------------------------------------

  std::string to_language_name( os::language::language_code l_ )
  {
    static std::array< std::string, static_cast<size_t>( os::language::language_code::other ) + 1 > strings = {
      "Unknown",
      "Afar",
      "Caucasian",
      "Avestan",
      "Afrikaans",
      "Akan",
      "Amharic",
      "Aragonese",
      "Arabic",
      "Assamese",
      "Caucasian",
      "Aymara",
      "Azerbaijani",
      "Bashkir",
      "Belarusian",
      "Bulgarian",
      "Bihari",
      "Bislama",
      "Bambara",
      "Bengali",
      "Tibetan",
      "Breton",
      "Bosnian",
      "Catalan",
      "Caucasian",
      "Chamorro",
      "Corsican",
      "Cree",
      "Czech",
      "Church Slavonic",
      "Chuvash",
      "Welsh",
      "Danish",
      "German",
      "Divehi",
      "Dzongkha",
      "Ewe",
      "Greek",
      "English",
      "Esperanto",
      "Spanish",
      "Estonian",
      "isolate",
      "Persian",
      "Fula",
      "Finnish",
      "Fijian",
      "Faroese",
      "French",
      "Western",
      "Irish",
      "Scottish",
      "Galician",
      "Guaran",
      "Gujarati",
      "Manx",
      "Hausa",
      "Hebrew",
      "Hindi",
      "Hiri",
      "Croatian",
      "Haitian",
      "Hungarian",
      "Armenian",
      "Herero",
      "Indonesian",
      "Igbo",
      "Nuosu",
      "Inupiaq",
      "Icelandic",
      "Italian",
      "Inuktitut",
      "Japanese",
      "Javanese",
      "Caucasian",
      "Kongo",
      "Kikuyu",
      "Kwanyama",
      "Kazakh",
      "Kalaallisut",
      "Khmer",
      "Kannada",
      "Korean",
      "Kanuri",
      "Kashmiri",
      "Kurdish",
      "Komi",
      "Cornish",
      "Kyrgyz",
      "Latin",
      "Luxembourgish",
      "Ganda",
      "Limburgish",
      "Lingala",
      "",
      "Lithuanian",
      "Luba",
      "Latvian",
      "Malagasy",
      "Marshallese",
      "Maori",
      "Macedonian",
      "Malayalam",
      "Mongolian",
      "Marathi",
      "Malay",
      "Maltese",
      "Burmese",
      "Nauruan",
      "Norwegian",
      "Northern",
      "Nepali",
      "Ndonga",
      "Dutch",
      "Norwegian",
      "Norwegian",
      "Southern",
      "Den",
      "Chichewa",
      "Occitan",
      "Ojibwe",
      "Oromo",
      "Oriya",
      "Ossetian",
      "Eastern Punjabi",
      "Pali",
      "Polish",
      "Pashto",
      "Portuguese",
      "Quechua",
      "Romansh",
      "Kirundi",
      "Romanian",
      "Russian",
      "Kinyarwanda",
      "Sanskrit",
      "Sardinian",
      "Sindhi",
      "Northern",
      "Sango",
      "Sinhalese",
      "Slovak",
      "Slovene",
      "Samoan",
      "Shona",
      "Somali",
      "Albanian",
      "Serbian",
      "Swati",
      "Southern",
      "Sundanese",
      "Swedish",
      "Swahili",
      "Tamil",
      "Telugu",
      "Tajik",
      "Thai",
      "Tigrinya",
      "Turkmen",
      "Tagalog",
      "Tswana",
      "Tonga",
      "Turkish",
      "Tsonga",
      "Tatar",
      "Twi",
      "Tahitian",
      "Uyghur",
      "Ukrainian",
      "Urdu",
      "Uzbek",
      "Venda",
      "Vietnamese",
      "Volap",
      "Walloon",
      "Wolof",
      "Xhosa",
      "Yiddish",
      "Yoruba",
      "Zhuang",
      "Chinese",
      "Zulu",
      "Other",
    };

    return strings[static_cast<size_t>( l_ )];
  }

  
  // ---------------------------------------------------------------------------------------------------------

  std::string to_country_name( os::language::country_code l_ )
  {
    static std::array< std::string, static_cast<size_t>( os::language::country_code::other ) + 1 > strings = {
      "Unknown",      
      "Andorra",
      "United Arab Emirates",
      "Afghanistan",
      "Antigua and Barbuda",
      "Anguilla",
      "Albania",
      "Armenia",
      "Angola",
      "Antarctica",
      "Argentina",
      "American Samoa",
      "Austria",
      "Australia",
      "Aruba",
      "Åland Islands",	
      "Azerbaijan",
      "Bosnia and Herzegovina",
      "Barbados",
      "Bangladesh",
      "Belgium",
      "Burkina Faso",
      "Bulgaria",
      "Bahrain",
      "Burundi",
      "Benin",
      "Saint Barthélemy",
      "Bermuda",
      "Brunei Darussalam",
      "Bolivia",
      "Bonaire, Sint Eustatius and Saba",
      "Brazil",
      "Bahamas",
      "Bhutan",
      "Bouvet Island",
      "Botswana",
      "Belarus",
      "Belize",
      "Canada",
      "Cocos (Keeling) Islands",
      "Congo, the Democratic Republic of the",
      "Central African Republic",
      "Congo",
      "Switzerland",
      "Côte d'Ivoire",
      "Cook Islands",
      "Chile",
      "Cameroon",
      "China",
      "Colombia",
      "Costa Rica",
      "Cuba",
      "Cabo Verde",
      "Curacao",
      "Christmas Island",
      "Cyprus",
      "Czechia",
      "Germany",
      "Djibouti",
      "Denmark",
      "Dominica",
      "Dominican Republic",
      "Algeria",
      "Ecuador",
      "Estonia",
      "Egypt",
      "Western Sahara",
      "Eritrea",
      "Spain",
      "Ethiopia",
      "Finland",
      "Fiji",
      "Falkland Islands (Malvinas)",
      "Micronesia, Federated States of",
      "Faroe Islands",
      "France",
      "Gabon",
      "United Kingdom of Great Britain and Northern Ireland",
      "Grenada",
      "Georgia",
      "French Guiana",
      "Guernsey",
      "Ghana",
      "Gibraltar",
      "Greenland",
      "Gambia",
      "Guinea",
      "Guadeloupe",
      "Equatorial Guinea",
      "Greece",
      "South Georgia and the South Sandwich Islands",
      "Guatemala",
      "Guam",
      "Guinea-Bissau",
      "Guyana",
      "Hong Kong",
      "Heard Island and McDonald Islands",
      "Honduras",
      "Croatia",
      "Haiti",
      "Hungary",
      "Indonesia",
      "Ireland",
      "Israel",
      "Isle of Man",
      "India",
      "British Indian Ocean Territory",
      "Iraq",
      "Iran, Islamic Republic of",
      "Iceland",
      "Italy",
      "Jersey",
      "Jamaica",
      "Jordan",
      "Japan",
      "Kenya",
      "Kyrgyzstan",
      "Cambodia",
      "Kiribati",
      "Comoros",
      "Saint Kitts and Nevis",
      "Korea, Democratic People's Republic of",
      "Korea, Republic of",
      "Kuwait",
      "Cayman Islands",
      "Kazakhstan",
      "Lao People's Democratic Republic",
      "Lebanon",
      "Saint Lucia",
      "Liechtenstein",
      "Sri Lanka",
      "Liberia",
      "Lesotho",
      "Lithuania",
      "Luxembourg",
      "Latvia",
      "Libya",
      "Morocco",
      "Monaco",
      "Moldova, Republic of",
      "Montenegro",
      "Saint Martin (French part),"
      "Madagascar",
      "Marshall Islands",
      "Macedonia, the former Yugoslav Republic of",
      "Mali",
      "Myanmar",
      "Mongolia",
      "Macao",
      "Northern Mariana Islands",
      "Martinique",
      "Mauritania",
      "Montserrat",
      "Malta",
      "Mauritius",
      "Maldives",
      "Malawi",
      "Mexico",
      "Malaysia",
      "Mozambique",
      "Namibia",
      "New Caledonia",
      "Niger",
      "Norfolk Island",
      "Nigeria",
      "Nicaragua",
      "Netherlands",
      "Norway",
      "Nepal",
      "Nauru",
      "Niue",
      "New Zealand",
      "Oman",
      "Panama",
      "Peru",
      "French Polynesia",
      "Papua New Guinea",
      "Philippines",
      "Pakistan",
      "Poland",
      "Saint Pierre and Miquelon",
      "Pitcairn",
      "Puerto Rico",
      "Palestine, State of"
      "Portugal",
      "Palau",
      "Paraguay",
      "Qatar",
      "Réunion",
      "Romania",
      "Serbia",
      "Russian Federation",
      "Rwanda",
      "Saudi Arabia",
      "Solomon Islands",
      "Seychelles",
      "Sudan",
      "Sweden",
      "Singapore",
      "Saint Helena, Ascension and Tristan da Cunha",
      "Slovenia",
      "Svalbard and Jan Mayen",
      "Slovakia",
      "Sierra Leone",
      "San Marino",
      "Senegal",
      "Somalia",
      "Suriname",
      "South Sudan",
      "Sao Tome and Principe",
      "El Salvador",
      "Sint Maarten (Dutch part)",
      "Syrian Arab Republic",
      "Swaziland",
      "Turks and Caicos Islands",
      "Chad",
      "French Southern Territories",
      "Togo",
      "Thailand",
      "Tajikistan",
      "Tokelau",
      "Timor-Leste",
      "Turkmenistan",
      "Tunisia",
      "Tonga",
      "Turkey",
      "Trinidad and Tobago",
      "Tuvalu",
      "Taiwan, Province of China",
      "Tanzania, United Republic of",
      "Ukraine",
      "Uganda",
      "United States Minor Outlying Islands",
      "United States of America",
      "Uruguay",
      "Uzbekistan",
      "Holy See",
      "Saint Vincent and the Grenadines",
      "Venezuela, Bolivarian Republic of",
      "Virgin Islands, British",
      "Virgin Islands, U.S.",
      "Viet Nam",
      "Vanuatu",
      "Wallis and Futuna",
      "Samoa",
      "Yemen",
      "Mayotte",
      "South Africa",
      "Zambia",
      "Zimbabwe",
      "Other"
    };

    return strings[static_cast<size_t>( l_ )];
  }
  

  // ---------------------------------------------------------------------------------------------------------

  systeminfo::os::language::language_code to_language_code( const std::string& s_ )
  {
    auto it = std::find( language_codes.begin(), language_codes.end(), s_ );
    return 
      ( it == language_codes.end() ) ? 
      systeminfo::os::language::language_code::unknown :
      static_cast< systeminfo::os::language::language_code >( std::distance( language_codes.begin(), it ) );
    ;
  }


  // ---------------------------------------------------------------------------------------------------------

  systeminfo::os::language::country_code to_country_code( const std::string& s_ )
  {
    auto it = std::find( country_codes.begin(), country_codes.end(), s_ );
    return 
      ( it == country_codes.end() ) ? 
      systeminfo::os::language::country_code::unknown :
      static_cast< systeminfo::os::language::country_code >( std::distance( country_codes.begin(), it ) );
    ;
  }

} // namespace ll