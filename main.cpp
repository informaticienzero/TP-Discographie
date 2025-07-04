#include <exception>
#include <print>
#include <string>

import Discographie;
import DiscographieTests;
import String;

int main()
{
  test_création_morceau_entrée_complète(); 
  test_création_morceau_entrée_espaces_partout(); 
  test_création_morceau_entrée_chanson_artiste(); 
  test_création_morceau_entrée_chanson_uniquement(); 
  test_création_morceau_entrée_vide(); 

  // On préremplit la discographie.
  const Artiste bob_le_chanteur { "Bob le chanteur" };
  const Artiste alice_la_chanteuse { "Alice la chanteuse" };
  const Artiste papa_cuistot { "Papa cuistot" };
  const Album mes_supers_chansons { "Mes supers chansons" };
  const Album comptines_alice { "Comptines d'Alice" };
  const Album cuisine_en_musique { "Cuisine en musique" };

  const Morceau chanson_a {
    "Chanson A",
    bob_le_chanteur,
    mes_supers_chansons
  };
  const Morceau chanson_b {
    "Chanson B",
    bob_le_chanteur,
    mes_supers_chansons
  };
  const Morceau un_chat_qui_dormait {
    "Un chat qui dormait",
    alice_la_chanteuse,
    comptines_alice
  };
  const Morceau amour {
    "L'amour",
    alice_la_chanteuse,
    comptines_alice
  };
  const Morceau mousse_au_chocolat {
    "La mousse au chocolat",
    papa_cuistot,
    cuisine_en_musique
  };

  Discographie discographie {
    chanson_a,
    chanson_b,
    un_chat_qui_dormait,
    amour,
    mousse_au_chocolat
  };

  bool continuer = true;

  do
  {
    try
    {
      std::string entrée = récuperer_commande();
      auto[commande, instructions] = analyser_commande(entrée);
      instructions = trim(instructions);
      continuer = exécuter_commande(discographie, commande, instructions);
    }
    catch(const std::runtime_error& e)
    {
      std::println("{}", e.what());
    }
      
  } while (continuer);
    
  return 0;
}