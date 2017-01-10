/*************************************************************************************************************

 Limelight Framework - SystemInfo Utils


 Copyright 2016 mvd

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in
 compliance with the License. You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software distributed under the License is
 distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and limitations under the License.

*************************************************************************************************************/

#pragma once

#include "systeminfo/types.h"

#include <base/environment.h>

LL_WARNING_DISABLE_GCC( deprecated-declarations )
#include <boost/filesystem/path.hpp>
LL_WARNING_ENABLE_GCC( deprecated-declarations )


namespace ll
{
namespace systeminfo
{
namespace os
{  
  // ---------------------------------------------------------------------------------------------------------
  // Types
  // ---------------------------------------------------------------------------------------------------------

  namespace fs = boost::filesystem;


  enum class family
  {
    windows,
    osx,
    linux
  };

  enum class architecture
  {
    unknown,
    x86,
    x64
  };


  //! \todo move to base library
  struct language
  {
    // iso-639 compliant list of language codes
    //! \todo extract to separate module or to base https://github.com/meikidd/iso-639-1/blob/master/src/data.js
    enum class language_code
    {
      unknown,
      aa, /* Afar Afaraf */
      ab, /* Caucasian Abkhaz аҧсуа бызшәа, аҧсшәа */
      ae, /* Avestan avesta */ 
      af, /* Afrikaans Afrikaans */
      ak, /* Akan Akan */
      am, /* Amharic አማርኛ */
      an, /* Aragonese aragonés */
      ar, /* Arabic العربية */
      as, /* Assamese অসমীয়া */
      av, /* Caucasian Avaric авар мацӀ, магӀарул мацӀ */
      ay, /* Aymara aymar */
      az, /* Azerbaijani azərbaycan dili */
      ba, /* Bashkir башҡорт теле */
      be, /* Belarusian беларуская мова */
      bg, /* Bulgarian български език */
      bh, /* Bihari भोजपुरी */
      bi, /* Bislama Bislama */
      bm, /* Bambara bamanankan */
      bn, /* Bengali, Bangla বাংলা */
      bo, /* Tibetan Standard, Tibetan, Central བོད་ཡིག */
      br, /* Breton brezhoneg */
      bs, /* Bosnian bosanski jezik */
      ca, /* Catalan català */
      ce, /* Caucasian Chechen нохчийн мотт */
      ch, /* Chamorro Chamoru */
      co, /* Corsican corsu, lingua corsa */
      cr, /* Cree ᓀᐦᐃᔭᐍᐏᐣ */
      cs, /* Czech čeština, český jazyk */
      cu, /* Old Church Slavonic, Church Slavonic, Bulgarian ѩзыкъ словѣньскъ */
      cv, /* Chuvash чӑваш чӗлхи */
      cy, /* Welsh Cymraeg */
      da, /* Danish dansk */
      de, /* German Deutsch */
      dv, /* Divehi, Dhivehi, Maldivian ދިވެހި */
      dz, /* Dzongkha རྫོང་ཁ */
      ee, /* Ewe Eʋegbe */
      el, /* Greek (modern) ελληνικά */
      en, /* English English */
      eo, /* Esperanto Esperanto */
      es, /* Spanish español */
      et, /* Estonian eesti, eesti keel */
      eu, /* isolate Basque euskara, euskera */
      fa, /* Persian (Farsi) فارسی */
      ff, /* Fula, Fulah, Pulaar, Pular Fulfulde, Pulaar, Pular */
      fi, /* Finnish suomi, suomen kieli */
      fj, /* Fijian vosa Vakaviti */
      fo, /* Faroese føroyskt */
      fr, /* French français, langue française */
      fy, /* Western Frisian Frysk */
      ga, /* Irish Gaeilge */
      gd, /* Scottish Gaelic, Gaelic Gàidhlig */
      gl, /* Galician galego */
      gn, /* Guaraní Avañe'ẽ */
      gu, /* Gujarati ગુજરાતી */
      gv, /* Manx Gaelg, Gailck */
      ha, /* Hausa (Hausa) هَوُسَ */
      he, /* Hebrew (modern) עברית */
      hi, /* Hindi हिन्दी, हिंदी */
      ho, /* Hiri Motu Hiri Motu */
      hr, /* Croatian hrvatski jezik */
      ht, /* Haitian, Haitian Creole Kreyòl ayisyen */
      hu, /* Hungarian magyar */
      hy, /* Armenian Հայերեն */
      hz, /* Herero Otjiherero */
      id, /* Indonesian Bahasa Indonesia*/
      ig, /* Igbo Asụsụ Igbo */
      ii, /* Nuosu ꆈꌠ꒿ Nuosuhxop */
      ik, /* Inupiaq Iñupiaq, Iñupiatun */
      is, /* Icelandic Íslenska */
      it, /* Italian italiano */
      iu, /* Inuktitut ᐃᓄᒃᑎᑐᑦ */
      ja, /* Japanese 日本語 (にほんご) */
      jv, /* Javanese ꦧꦱꦗꦮ, Basa Jawa */
      ka, /* Caucasian Georgian ქართული */
      kg, /* Kongo Kikongo */
      ki, /* Kikuyu, Gikuyu Gĩkũyũ */
      kj, /* Kwanyama, Kuanyama Kuanyama */
      kk, /* Kazakh қазақ тілі */
      kl, /* Kalaallisut, Greenlandic kalaallisut, kalaallit oqaasii */
      km, /* Khmer ខ្មែរ, ខេមរភាសា, ភាសាខ្មែរ */
      kn, /* Kannada ಕನ್ನಡ */
      ko, /* Korean 한국어 */
      kr, /* Kanuri Kanuri */
      ks, /* Kashmiri कश्मीरी, كشميري‎ */
      ku, /* Kurdish Kurdî, كوردی‎ */
      kv, /* Komi коми кыв */
      kw, /* Cornish Kernewek */
      ky, /* Kyrgyz Кыргызча, Кыргыз тили */
      la, /* Latin latine, lingua latina */
      lb, /* Luxembourgish, Letzeburgesch Lëtzebuergesch */
      lg, /* Ganda Luganda */
      li, /* Limburgish, Limburgan, Limburger Limburgs */
      ln, /* Lingala Lingála */
      lo, /* ພາສາລາວ */
      lt, /* Lithuanian lietuvių kalba */
      lu, /* Luba-Katanga Tshiluba */
      lv, /* Latvian latviešu valoda */
      mg, /* Malagasy fiteny malagasy */  
      mh, /* Marshallese Kajin M̧ajeļ */
      mi, /* Māori te Māori */
      mk, /* Macedonian македонски јазик */
      ml, /* Malayalam മലയാളം */
      mn, /* Mongolian Монгол хэл */
      mr, /* Marathi (Marāṭhī) मराठी */
      ms, /* Malay bahasa Melayu, بهاس ملايو‎ */
      mt, /* Maltese Malti */
      my, /* Burmese ဗမာစာ */
      na, /* Nauruan Dorerin Naoero */
      nb, /* Norwegian Bokmål Norsk bokmål */
      nd, /* Northern Ndebele isiNdebele */
      ne, /* Nepali नेपाली */
      ng, /* Ndonga Owambo */
      nl, /* Dutch Nederlands, Vlaams */
      nn, /* Norwegian Nynorsk Norsk nynorsk */
      no, /* Norwegian Norsk */
      nr, /* Southern Ndebele isiNdebele */
      nv, /* Dené–Yeniseian Navajo, Navaho Diné bizaad */
      ny, /* Chichewa, Chewa, Nyanja chiCheŵa, chinyanja */
      oc, /* Occitan occitan, lenga d'òc */
      oj, /* Ojibwe, Ojibwa ᐊᓂᔑᓈᐯᒧᐎᓐ */
      om, /* Oromo Afaan Oromoo */
      or_, /* Oriya ଓଡ଼ିଆ */ // or is a c++ keyword
      os, /* Ossetian, Ossetic ирон æвзаг */
      pa, /* Eastern Punjabi, Eastern Panjabi ਪੰਜਾਬੀ */
      pi, /* Pāli पाऴि */
      pl, /* Polish język polski, polszczyzna */
      ps, /* Pashto, Pushto پښتو */
      pt, /* Portuguese Português */
      qu, /* Quechua Runa Simi, Kichwa */  
      rm, /* Romansh rumantsch grischun */
      rn, /* Kirundi Ikirundi */
      ro, /* Romanian Română */
      ru, /* Russian Русский */
      rw, /* Kinyarwanda Ikinyarwanda */
      sa, /* Sanskrit (Saṁskṛta) संस्कृतम् */
      sc, /* Sardinian sardu */
      sd, /* Sindhi सिन्धी, سنڌي، سندھی‎ */
      se, /* Northern Sami Davvisámegiella */
      sg, /* Sango yângâ tî sängö */
      si, /* Sinhalese, Sinhala සිංහල */
      sk, /* Slovak slovenčina, slovenský jazyk */
      sl, /* Slovene slovenski jezik, slovenščina */
      sm, /* Samoan gagana fa'a Samoa */
      sn, /* Shona chiShona */
      so, /* Somali Soomaaliga, af Soomaali */
      sq, /* Albanian Shqip */
      sr, /* Serbian српски језик */
      ss, /* Swati SiSwati */
      st, /* Southern Sotho Sesotho */
      su, /* Sundanese Basa Sunda */
      sv, /* Swedish svenska */
      sw, /* Swahili Kiswahili */
      ta, /* Tamil தமிழ் */
      te, /* Telugu తెలుగు */
      tg, /* Tajik тоҷикӣ, toçikī, تاجیکی‎ */
      th, /* Thai ไทย */
      ti, /* Tigrinya ትግርኛ */
      tk, /* Turkmen Türkmen, Түркмен */
      tl, /* Tagalog Wikang Tagalog */
      tn, /* Tswana Setswana */
      to, /* Tonga (Tonga Islands) faka Tonga */
      tr, /* Turkish Türkçe */
      ts, /* Tsonga Xitsonga */
      tt, /* Tatar татар теле, tatar tele */
      tw, /* Twi */
      ty, /* Tahitian Tahiti */ 
      ug, /* Uyghur ئۇيغۇرچە‎, Uyghurche */
      uk, /* Ukrainian Українська */
      ur, /* Urdu اردو */
      uz, /* Uzbek Oʻzbek, Ўзбек, أۇزبېك‎ */
      ve, /* Venda Tshivenḓa */
      vi, /* Vietnamese Tiếng Việt */
      vo, /* Volapük Volapük vo */
      wa, /* Walloon walon */
      wo, /* Wolof Wollof */
      xh, /* Xhosa isiXhosa */
      yi, /* Yiddish ייִדיש */
      yo, /* Yoruba Yorùbá */
      za, /* Zhuang, Chuang Saɯ cueŋƅ, cuengh */  
      zh, /* Chinese 中文 (Zhōngwén), 汉语, 漢語 */  
      zu, /* Zulu isiZulu */  
      other
    };

    //! iso 3166-1 compliant countries
    enum class country_code
    {
      unknown,      
      AD, /* Andorra */
      AE, /* United Arab Emirates */
      AF, /* Afghanistan */
      AG, /* Antigua and Barbuda */
      AI, /* Anguilla */
      AL, /* Albania */
      AM, /* Armenia */
      AO, /* Angola */
      AQ, /* Antarctica */
      AR, /* Argentina */
      AS, /* American Samoa */
      AT, /* Austria */
      AU, /* Australia */
      AW, /* Aruba */
      AX, /* Åland Islands */	
      AZ, /* Azerbaijan */
      BA, /* Bosnia and Herzegovina */
      BB, /* Barbados */
      BD, /* Bangladesh */
      BE, /* Belgium */
      BF, /* Burkina Faso */
      BG, /* Bulgaria */
      BH, /* Bahrain */
      BI, /* Burundi */
      BJ, /* Benin */
      BL, /* Saint Barthélemy */
      BM, /* Bermuda */
      BN, /* Brunei Darussalam */
      BO, /* Bolivia, Plurinational State of	*/
      BQ, /* Bonaire, Sint Eustatius and Saba */
      BR, /* Brazil */
      BS, /* Bahamas */
      BT, /* Bhutan */
      BV, /* Bouvet Island */
      BW, /* Botswana */
      BY, /* Belarus */
      BZ, /* Belize */
      CA, /* Canada */
      CC, /* Cocos (Keeling) Islands */
      CD, /* Congo, the Democratic Republic of the */
      CF, /* Central African Republic */
      CG, /* Congo */
      CH, /* Switzerland */
      CI, /* Côte d'Ivoire */
      CK, /* Cook Islands */
      CL, /* Chile */
      CM, /* Cameroon */
      CN, /* China */
      CO, /* Colombia */
      CR, /* Costa Rica */
      CU, /* Cuba */
      CV, /* Cabo Verde */
      CW, /* Curaçao*/
      CX, /* Christmas Island */
      CY, /* Cyprus */
      CZ, /* Czechia */
      DE, /* Germany */
      DJ, /* Djibouti */
      DK, /* Denmark */
      DM, /* Dominica */
      DO, /* Dominican Republic */
      DZ, /* Algeria */
      EC, /* Ecuador */
      EE, /* Estonia */
      EG, /* Egypt */
      EH, /* Western Sahara */
      ER, /* Eritrea */
      ES, /* Spain */
      ET, /* Ethiopia */
      FI, /* Finland */
      FJ, /* Fiji */
      FK, /* Falkland Islands (Malvinas) */
      FM, /* Micronesia, Federated States of */
      FO, /* Faroe Islands */
      FR, /* France */
      GA, /* Gabon */
      GB, /* United Kingdom of Great Britain and Northern Ireland */
      GD, /* Grenada */
      GE, /* Georgia */
      GF, /* French Guiana */
      GG, /* Guernsey */
      GH, /* Ghana */
      GI, /* Gibraltar */
      GL, /* Greenland */
      GM, /* Gambia */
      GN, /* Guinea */
      GP, /* Guadeloupe */
      GQ, /* Equatorial Guinea */
      GR, /* Greece */
      GS, /* South Georgia and the South Sandwich Islands */
      GT, /* Guatemala */
      GU, /* Guam */
      GW, /* Guinea-Bissau*/
      GY, /* Guyana */
      HK, /* Hong Kong */
      HM, /* Heard Island and McDonald Islands */
      HN, /* Honduras */
      HR, /* Croatia */
      HT, /* Haiti */
      HU, /* Hungary */
      ID, /* Indonesia */
      IE, /* Ireland */
      IL, /* Israel */
      IM, /* Isle of Man */
      IN, /* India */
      IO, /* British Indian Ocean Territory */
      IQ, /* Iraq */
      IR, /* Iran, Islamic Republic of */
      IS, /* Iceland */
      IT, /* Italy */
      JE, /* Jersey */
      JM, /* Jamaica */
      JO, /* Jordan */
      JP, /* Japan */
      KE, /* Kenya */
      KG, /* Kyrgyzstan */
      KH, /* Cambodia */
      KI, /* Kiribati */
      KM, /* Comoros */
      KN, /* Saint Kitts and Nevis */
      KP, /* Korea, Democratic People's Republic of */
      KR, /* Korea, Republic of */
      KW, /* Kuwait */
      KY, /* Cayman Islands */
      KZ, /* Kazakhstan */
      LA, /* Lao People's Democratic Republic */
      LB, /* Lebanon */
      LC, /* Saint Lucia */
      LI, /* Liechtenstein */
      LK, /* Sri Lanka */
      LR, /* Liberia */
      LS, /* Lesotho */
      LT, /* Lithuania */
      LU, /* Luxembourg */
      LV, /* Latvia */
      LY, /* Libya */
      MA, /* Morocco */
      MC, /* Monaco */
      MD, /* Moldova, Republic of*/
      ME, /* Montenegro */
      MF, /* Saint Martin (French part)*/
      MG, /* Madagascar */
      MH, /* Marshall Islands */
      MK, /* Macedonia, the former Yugoslav Republic of*/
      ML, /* Mali */
      MM, /* Myanmar */
      MN, /* Mongolia */
      MO, /* Macao */
      MP, /* Northern Mariana Islands */
      MQ, /* Martinique */
      MR, /* Mauritania */
      MS, /* Montserrat */
      MT, /* Malta */
      MU, /* Mauritius */
      MV, /* Maldives */
      MW, /* Malawi */
      MX, /* Mexico */
      MY, /* Malaysia */
      MZ, /* Mozambique */
      NA, /* Namibia */
      NC, /* New Caledonia */
      NE, /* Niger */
      NF, /* Norfolk Island */
      NG, /* Nigeria */
      NI, /* Nicaragua */
      NL, /* Netherlands */
      NO, /* Norway */
      NP, /* Nepal */
      NR, /* Nauru */
      NU, /* Niue */
      NZ, /* New Zealand */
      OM, /* Oman */
      PA, /* Panama */
      PE, /* Peru */
      PF, /* French Polynesia */
      PG, /* Papua New Guinea */
      PH, /* Philippines */
      PK, /* Pakistan */
      PL, /* Poland */
      PM, /* Saint Pierre and Miquelon */
      PN, /* Pitcairn */
      PR, /* Puerto Rico */
      PS, /* Palestine, State of*/
      PT, /* Portugal */
      PW, /* Palau */
      PY, /* Paraguay */
      QA, /* Qatar */
      RE, /* Réunion*/
      RO, /* Romania */
      RS, /* Serbia */
      RU, /* Russian Federation */
      RW, /* Rwanda */
      SA, /* Saudi Arabia */
      SB, /* Solomon Islands */
      SC, /* Seychelles */
      SD, /* Sudan */
      SE, /* Sweden */
      SG, /* Singapore */
      SH, /* Saint Helena, Ascension and Tristan da Cunha*/
      SI, /* Slovenia */
      SJ, /* Svalbard and Jan Mayen */
      SK, /* Slovakia */
      SL, /* Sierra Leone */
      SM, /* San Marino */
      SN, /* Senegal */
      SO, /* Somalia */
      SR, /* Suriname */
      SS, /* South Sudan */
      ST, /* Sao Tome and Principe */
      SV, /* El Salvador */
      SX, /* Sint Maarten (Dutch part)*/
      SY, /* Syrian Arab Republic */
      SZ, /* Swaziland */
      TC, /* Turks and Caicos Islands */
      TD, /* Chad */
      TF, /* French Southern Territories */
      TG, /* Togo */
      TH, /* Thailand */
      TJ, /* Tajikistan */
      TK, /* Tokelau */
      TL, /* Timor-Leste*/
      TM, /* Turkmenistan */
      TN, /* Tunisia */
      TO, /* Tonga */
      TR, /* Turkey */
      TT, /* Trinidad and Tobago */
      TV, /* Tuvalu */
      TW, /* Taiwan, Province of China*/
      TZ, /* Tanzania, United Republic of*/
      UA, /* Ukraine */
      UG, /* Uganda */
      UM, /* United States Minor Outlying Islands */
      US, /* United States of America */
      UY, /* Uruguay */
      UZ, /* Uzbekistan */
      VA, /* Holy See */
      VC, /* Saint Vincent and the Grenadines */
      VE, /* Venezuela, Bolivarian Republic of */
      VG, /* Virgin Islands, British */
      VI, /* Virgin Islands, U.S. */
      VN, /* Viet Nam */
      VU, /* Vanuatu */
      WF, /* Wallis and Futuna */
      WS, /* Samoa */
      YE, /* Yemen */
      YT, /* Mayotte */
      ZA, /* South Africa */
      ZM, /* Zambia */
      ZW, /* Zimbabwe */
      other
    };
      

    language_code code = language_code::unknown;
    country_code  country = country_code::unknown;
  };
  

  enum class standard_location
  {
    home,
    temp,
    download,
    desktop,
    applications,
    applications32,
    app_data,
    documents,
    music,
    pictures,
    videos,
  };

  enum class usermode
  {
    current_user,
    shared,
  };

  
  // ---------------------------------------------------------------------------------------------------------
  // Functions
  // ---------------------------------------------------------------------------------------------------------

  family get_family();  //! \todo constexpr?

  std::string get_name(); 

  architecture get_architecture();
  
  version_t get_version();

  language get_language();


  fs::path get_standard_location( standard_location l_, usermode m_ );


}  // namespace os

}  // namespace systeminfo
}  // namespace ll


// -----------------------------------------------------------------------------------------------------------
// Utilities
// -----------------------------------------------------------------------------------------------------------

namespace ll
{
  std::string to_string( systeminfo::os::family f_ );

  std::string to_string( systeminfo::os::architecture a_ );

  std::string to_string( const systeminfo::version_t& v_ );


  std::string to_string( systeminfo::os::language l_ );

  std::string to_string( systeminfo::os::language::language_code l_ );
  
  std::string to_string( systeminfo::os::language::country_code l_ );


  std::string to_ietf_code( systeminfo::os::language l_ );

  std::string to_language_name( systeminfo::os::language::language_code l_ );

  std::string to_country_name( systeminfo::os::language::country_code l_ );


  systeminfo::os::language::language_code to_language_code( const std::string& s_ );

  systeminfo::os::language::country_code to_country_code( const std::string& s_ );

}  // namespace ll