module;

#include <cctype>
#include <string>
#include <ranges>

export module String;

export std::string trim(const std::string& entrée)
{
  auto est_espace = [](char lettre) {
    return std::isspace(lettre);
  };

  return entrée
    | std::views::drop_while(est_espace)
    | std::views::reverse
    | std::views::drop_while(est_espace)
    | std::views::reverse
    | std::ranges::to<std::string>();
}