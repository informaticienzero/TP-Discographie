module;

#include <algorithm>
#include <cassert>
#include <exception>
#include <iostream>
#include <format>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

export module Discographie;

import String;

export struct Artiste 
{ 
  std::string nom; 
}; 
 
export struct Album 
{ 
  std::string nom; 
}; 
 
export struct Morceau 
{ 
  std::string nom; 
  Artiste compositeur; 
  Album album; 
};

// Une discographie n'est rien d'autre qu'un ensemble de morceaux. 
export using Discographie = std::vector<Morceau>;

// Si utilisation de std::println
template <>
struct std::formatter<Artiste> : std::formatter<std::string>
{
  auto format(const Artiste& artiste, std::format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}", artiste.nom);
  }
};

template <>
struct std::formatter<Album> : std::formatter<std::string>
{
  auto format(const Album& album, std::format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}", album.nom);
  }
};

template <>
struct std::formatter<Morceau> : std::formatter<std::string>
{
  auto format(const Morceau& morceau, std::format_context& ctx) const
  {
    return std::format_to(
      ctx.out(),
      "{} [{}] (Album {})",
      morceau.nom,
      morceau.compositeur,
      morceau.album
    );
  }
};

// On surcharge l'opérateur << pour pouvoir afficher le type Artiste. 
std::ostream& operator<<(std::ostream& sortie, const Artiste& artiste) 
{ 
  sortie << artiste.nom; 
  return sortie; 
} 
 
// On surcharge l'opérateur << pour pouvoir afficher le type Album. 
std::ostream& operator<<(std::ostream& sortie, const Album& album) 
{ 
  sortie << album.nom; 
  return sortie; 
} 
 
// On surcharge l'opérateur << pour pouvoir afficher le type Morceau. 
std::ostream& operator<<(std::ostream& sortie, const Morceau& morceau) 
{ 
  sortie
    << morceau.nom
    << " ["
    << morceau.album
    << "] (Album "
    << morceau.compositeur
    << ")";
  return sortie; 
} 

// Conversion d'un flux d'entrée (std::cin, chaîne, fichier) en type Morceau.
// Exportée car utilisée par le module de test.
export std::istream& operator>>(std::istream& flux, Morceau& morceau)
{
  std::string ligne;

  // On n'arrive pas à lire l'entrée.
  if (!std::getline(flux, ligne))
  {
    return flux;
  }

  std::istringstream iss(ligne);
  auto champ_ou_défaut = [](const std::string& champ, std::string défaut) {
    auto nettoyé = trim(champ);
    if (std::empty(nettoyé))
    {
      return défaut;
    }
    return nettoyé;
  };

  std::string nom_morceau;
  std::getline(iss, nom_morceau, '|');
  morceau.nom = champ_ou_défaut(nom_morceau, "Morceau inconnu");

  std::string nom_album;
  std::getline(iss, nom_album, '|');
  morceau.album.nom = champ_ou_défaut(nom_album, "Album inconnu");

  std::string nom_artiste;
  std::getline(iss, nom_artiste, '|');
  morceau.compositeur.nom = champ_ou_défaut(nom_artiste, "Artiste inconnu");

  return flux;
}

void enregistrement(const Discographie& discographie, const std::string& nom_fichier) 
{ 
  std::ofstream fichier(nom_fichier); 
  if (!fichier) 
  { 
    throw std::runtime_error("Impossible d'ouvrir le fichier en écriture."); 
  } 
  
  for (const auto& morceau : discographie) 
  { 
    std::println(fichier, "{}|{}|{}", morceau.nom, morceau.album, morceau.compositeur);
  } 
}

void chargement(Discographie& discographie, const std::string& nom_fichier) 
{ 
  std::ifstream fichier(nom_fichier); 
  if (!fichier) 
  { 
    throw std::runtime_error("Impossible d'ouvrir le fichier en lecture."); 
  }

  std::string ligne;
  while (std::getline(fichier, ligne)) 
  { 
    Morceau morceau;
    std::istringstream flux(ligne); 
    flux >> morceau; 
    discographie.emplace_back(morceau); 
  } 
}

void tri_par_morceau(Discographie& discographie)
{
  std::ranges::sort(discographie, [](const Morceau& lhs, const Morceau& rhs) {
    return lhs.nom < rhs.nom;
  });
}

// Affichage par morceau, dans l'ordre alphabétique. 
void affichage_par_morceau(const Discographie& discographie) 
{ 
  for (const auto& morceau : discographie) 
  { 
    std::println("--> {}", morceau); 
  } 
}

void tri_par_album(Discographie& discographie) 
{ 
  std::ranges::sort(discographie, [](const Morceau& lhs, const Morceau& rhs) 
  {
    // Tri par nom d'album.
    if (lhs.album.nom < rhs.album.nom)
    {
      return true; 
    }

    // Si l'album est le même, on trie par nom de Morceau.
    return lhs.album.nom == rhs.album.nom && lhs.nom < rhs.nom; 
  }); 
}

// Affichage par album, dans l'ordre alphabétique. 
void affichage_par_album(const Discographie& discographie)
{
  std::string album_précédent;
  for (const auto& morceau : discographie) 
  { 
    if (morceau.album.nom != album_précédent) 
    { 
      std::println("--> {} [{}]", morceau.album, morceau.compositeur); 
      album_précédent = morceau.album.nom; 
    }

    std::println("\t/--> {}", morceau.nom); 
  }
}

void tri_par_artiste(Discographie& discographie) 
{ 
  std::ranges::sort(discographie, [](const Morceau& lhs, const Morceau& rhs)
  {
    // Tri par artiste.
    if (lhs.compositeur.nom < rhs.compositeur.nom) 
    {
      return true; 
    }
    
    // Si ce sont les mêmes artistes, on trie par album.
    if (lhs.compositeur.nom == rhs.compositeur.nom) 
    { 
      if (lhs.album.nom < rhs.album.nom) 
      {
        return true; 
      }
        
      // Si ce sont les mêmes albums, tri par morceau.
      return lhs.album.nom == rhs.album.nom && lhs.nom < rhs.nom; 
    } 

    return false; 
  }); 
}

// Affichage par artiste, dans l'ordre alphabétique. 
void affichage_par_artiste(const Discographie& discographie) 
{ 
  std::string artiste_précédent; 
  std::string album_précédent; 
  
  for (const auto& morceau : discographie) 
  { 
    if (morceau.compositeur.nom != artiste_précédent) 
    { 
      std::println("--> {}", morceau.compositeur);
      artiste_précédent = morceau.compositeur.nom;
    } 

    if (morceau.album.nom != album_précédent) 
    {
      std::println("\t--> {}", morceau.album);
      album_précédent = morceau.album.nom;
    }

    std::println("\t\t/--> {}", morceau.nom);
  } 
}

// L'ensemble des possibilités d'affichage de la discographie. 
enum class Affichage { Artiste, Album, Morceau }; 
 
// Fonction liant ce qui précède pour gérer toute la logique d'affichage. 
void affichage(Discographie& discographie, Affichage type_affichage) 
{ 
  if (type_affichage == Affichage::Album) 
  { 
    tri_par_album(discographie);
    affichage_par_album(discographie); 
  } 
  else if (type_affichage == Affichage::Artiste) 
  { 
    tri_par_artiste(discographie); 
    affichage_par_artiste(discographie); 
  } 
  else if (type_affichage == Affichage::Morceau) 
  { 
    tri_par_morceau(discographie);
    affichage_par_morceau(discographie); 
  } 
  else 
  { 
    // Par exemple si on met à jour l'énumération
    // mais qu'on oublie d'ajouter la fonction correspondante. 
    assert(false && "Commande d'affichage inconnue."); 
  } 
}

// Récupération de ce que l'utilisateur écrit. 
export std::string récuperer_commande() 
{ 
  std::print("> "); 
 
  std::string commande; 
  std::getline(std::cin, commande); 
  return commande; 
}

// L'ensemble des actions qu'il est possible de faire. 
export enum class Commande { Afficher, Ajouter, Enregistrer, Charger, Quitter }; 
 
// Analyse du texte reçu en entrée. 
export std::tuple<Commande, std::string> analyser_commande(const std::string& commande_texte) 
{ 
  // Pour traiter la chaîne comme étant un flux, 
  // afin d'en extraire les données. 
  std::istringstream flux(commande_texte);
  std::string premier_mot; 
  flux >> premier_mot; 
  premier_mot = trim(premier_mot); 
  
  std::string instructions; 
  std::getline(flux, instructions); 
  instructions = trim(instructions); 
  
  if (premier_mot == "afficher") 
  { 
    // Le mode d'affichage. 
    return { Commande::Afficher, instructions }; 
  } 
  else if (premier_mot == "ajouter") 
  { 
    // Les informations à ajouter. 
    return { Commande::Ajouter, instructions }; 
  } 
  else if (premier_mot == "enregistrer") 
  { 
    // Le fichier à écrire. 
    return { Commande::Enregistrer, instructions }; 
  } 
  else if (premier_mot == "charger") 
  { 
    // Le fichier à lire. 
    return { Commande::Charger, instructions }; 
  } 
  else if (premier_mot == "quitter") 
  { 
    // Chaîne vide, car on quitte le programme sans autre instruction. 
    return { Commande::Quitter, "" }; 
  } 
  else 
  { 
    // On a reçu du texte qui est incorrect. 
    throw std::runtime_error("Commande invalide."); 
  } 
}

export bool exécuter_commande(Discographie& discographie, Commande commande, const std::string& instructions) 
{ 
  if (commande == Commande::Afficher) 
  { 
    if (instructions == "artistes") 
    { 
      affichage(discographie, Affichage::Artiste); 
    } 
    else if (instructions == "albums") 
    { 
      affichage(discographie, Affichage::Album); 
    }
    else if (instructions == "morceaux") 
    { 
      affichage(discographie, Affichage::Morceau); 
    } 
    else 
    { 
      // Si on se trouve ici, alors c'est qu'il y a
      // une erreur dans les instructions d'affichage. 
      throw std::runtime_error("Commande d'affichage inconnue."); 
    } 
  } 
  else if (commande == Commande::Ajouter) 
  { 
    std::istringstream flux(instructions);
    Morceau morceau; 
    flux >> morceau;
    discographie.push_back(morceau); 
  } 
  else if (commande == Commande::Charger) 
  { 
    chargement(discographie, instructions); 
  } 
  else if (commande == Commande::Enregistrer) 
  { 
    enregistrement(discographie, instructions); 
  } 
  else if (commande == Commande::Quitter) 
  { 
    // Plus rien à faire, on quitte. 
    return false; 
  } 
  
  return true; 
}
