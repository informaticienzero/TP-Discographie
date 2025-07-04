module;

#include <cassert>
#include <sstream>

export module DiscographieTests;

import Discographie;

// Test d'une entrée complète.
export void test_création_morceau_entrée_complète()
{
  std::istringstream entrée { "La carbonade flammande | Cuisine en musique | Papa cuistot" };
  Morceau morceau;

  entrée >> morceau;

  assert(morceau.nom == "La carbonade flammande" && "Le nom du morceau doit être La carbonade flammande.");
  assert(morceau.album.nom == "Cuisine en musique" && "Le nom de l'album doit être Cuisine en musique.");
  assert(morceau.compositeur.nom == "Papa cuistot" && "Le nom du compositeur doit être Papa cuistot.");
}

// Test d'une entrée complète avec beaucoup d'espaces.
export void test_création_morceau_entrée_espaces_partout()
{
  std::istringstream entrée { "L'été il fait chaud      |  Comptines d'Alice     |  Alice la chanteuse" };
  Morceau morceau;

  entrée >> morceau;

  assert(morceau.nom == "L'été il fait chaud" && "Le nom du morceau doit être L'été il fait chaud.");
  assert(morceau.album.nom == "Comptines d'Alice" && "Le nom de l'album doit être Comptines d'Alice.");
  assert(morceau.compositeur.nom == "Alice la chanteuse" && "Le nom du compositeur doit être Alice la chanteuse.");
}

// Test d'une entrée avec seulement le nom de la chanson et l'artiste. 
export void test_création_morceau_entrée_chanson_artiste()
{
  std::istringstream entrée { "Le chat dort toujours | | Alice la chanteuse" };
  Morceau morceau;

  entrée >> morceau;

  assert(morceau.nom == "Le chat dort toujours" && "Le nom du morceau doit être Le chat dort toujours.");
  assert(morceau.album.nom == "Album inconnu" && "Le nom de l'album doit être Album inconnu.");
  assert(morceau.compositeur.nom == "Alice la chanteuse" && "Le nom du compositeur doit être Alice la chanteuse.");
}

// Test d'une entrée avec seulement le nom de la chanson. 
export void test_création_morceau_entrée_chanson_uniquement()
{
  std::istringstream entrée { "Crème de marrons | |" };
  Morceau morceau;

  entrée >> morceau;

  assert(morceau.nom == "Crème de marrons" && "Le nom du morceau doit être Crème de marrons.");
  assert(morceau.album.nom == "Album inconnu" && "Le nom de l'album doit être Album inconnu.");
  assert(morceau.compositeur.nom == "Artiste inconnu" && "Le nom du compositeur doit être Artiste inconnu.");
}

// Test d'une entrée vide. 
export void test_création_morceau_entrée_vide()
{
  std::istringstream entrée { "| |" };
  Morceau morceau;

  entrée >> morceau;

  assert(morceau.nom == "Morceau inconnu" && "Le nom du morceau doit être Morceau inconnu.");
  assert(morceau.album.nom == "Album inconnu" && "Le nom de l'album doit être Album inconnu.");
  assert(morceau.compositeur.nom == "Artiste inconnu" && "Le nom du compositeur doit être Artiste inconnu.");
}