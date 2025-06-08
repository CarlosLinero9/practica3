#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

# include "../../include/model/Attributes.h"
# include "../../include/model/Player.h"
# include "../../include/model/Heuristic.h"


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///////////////
/*Heuristicas*/
///////////////

class ValoracionTest: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class miValoracion3: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class miValoracion2: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};

class miValoracion1: public Heuristic{
   protected:
      virtual float getHeuristic(const Parchis &estado, int jugador) const;
};
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////
/*AIPLAYER*/
////////////
class AIPlayer : public Player{
protected:
   //Id identificativo del jugador
   const int id;

public:
   /**
    * @brief Constructor de un objeto AIPlayer
    *
    * @param name Nombre del jugador
    */
   inline AIPlayer(const string& name): Player(name), id(1){
   };


   /**
    * @brief Constructor de un objeto AIPlayer
    *
    * @param name Nombre  del jugador
    * @param id Id del jugador
    */
   inline AIPlayer(const string& name, const int id): Player(name), id(id){
   };


   /**
    * @brief Función que percibe el el parchís y al jugador actual.
    * Asigna el tablero en actual y el id del jugador.
    *
    * @param p Instancia Parchis que se quiere percibir
    */
   inline virtual void perceive(Parchis& p){ Player::perceive(p); }


   /**
    * @brief Función abstracta que define el movimiento devuelto por el jugador.
    * Llama a la función movePiece con el valor asignado a los parámetros pasados
    * por referencia.
    *
    * @return true
    * @return false
    */
   virtual bool move();


   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    *
    * @param c_piece Color de la ficha
    * @param id_piece Id de la ficha
    * @param dice Número de dado
    */
   virtual void think(color& c_piece, int& id_piece, int& dice) const;


   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero de manera aleatoria. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    *
    * @param c_piece Color de la ficha
    * @param id_piece Id de la ficha
    * @param dice Número de dado
    */
   void thinkAleatorio(color& c_piece, int& id_piece, int& dice) const;



   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    * Considera la ficha más adelantada del jugador actual y el dado que se puede usar.
    *
    * @param c_piece Color de la ficha
    * @param id_piece Id de la ficha
    * @param dice Número de dado
    */
   void thinkFichaMasAdelantada(color& c_piece, int& id_piece, int& dice) const;



   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    * Considera la ficha más adelantada del jugador actual y el dado que se puede usar 
    * de entre todas la acciones disponibles.
    *
    * @param c_piece Color de la ficha
    * @param id_piece Id de la ficha
    * @param dice Número de dado
    */
   void thinkMejorOpcion(color& c_piece, int& id_piece, int& dice) const;



   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    * Utiliza el algoritmo Minimax para calcular el mejor movimiento.
    *
    * @param actual Estado actual del juego
    * @param jugador Id del jugador actual
    * @param profundidad Profundidad actual en el árbol de búsqueda
    * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
    * @param c_piece Color de la ficha a mover
    * @param id_piece Id de la ficha a mover
    * @param dice Valor del dado a usar
    * @param heuristic Heurística utilizada para evaluar los estados
    * @return float Valor heurístico del mejor movimiento encontrado
    */
   float Minimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
      color &c_piece, int &id_piece, int &dice,Heuristic *heuristic) const;



   /**
    * @brief Función que se encarga de decidir el mejor movimiento posible a
    * partir del estado actual del tablero. Asigna a las variables pasadas por
    * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    * Utiliza el algoritmo Minimax para calcular el mejor movimiento limitado a un 
    * número de exploraciones máximo.
    *
    * @param actual Estado actual del juego
    * @param jugador Id del jugador actual
    * @param profundidad Profundidad actual en el árbol de búsqueda
    * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
    * @param c_piece Color de la ficha a mover
    * @param id_piece Id de la ficha a mover
    * @param dice Valor del dado a usar
    * @param heuristic Heurística utilizada para evaluar los estados
    * @return float Valor heurístico del mejor movimiento encontrado
    */
   float Minimax_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
      color &c_piece, int &id_piece, int &dice, Heuristic *heuristic) const;



   /**
      * @brief Función que implementa el algoritmo de poda alfa-beta para encontrar el mejor movimiento.
      * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
      * @param actual Estado actual del juego
      * @param jugador Id del jugador actual
      * @param profundidad Profundidad actual en el árbol de búsqueda
      * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
      * @param c_piece Color de la ficha a mover
      * @param id_piece Id de la ficha a mover
      * @param dice Valor del dado a usar
      * @param alpha Valor alfa para la poda
      * @param beta Valor beta para la poda
      * @param heuristic Heurística utilizada para evaluar los estados
      * @return double Valor heurístico del mejor movimiento encontrado
    */
   double Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;



   /** 
    * @brief Función que implementa una versión mejorada del algoritmo de poda alfa-beta.
    * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
    * El objetivo es ordenar los movimientos antes de expandirlos, de modo que los más prometedores se
    * examinen primero. Esto aumenta la eficacia de la poda alfa-beta y puede reducir enormemente el
    * número de nodos evaluados.
    * @param actual Estado actual del juego
    * @param jugador Id del jugador actual
    * @param profundidad Profundidad actual en el árbol de búsqueda
    * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
    * @param c_piece Color de la ficha a mover
    * @param id_piece Id de la ficha a mover
    * @param dice Valor del dado a usar
    * @param alpha Valor alfa para la poda
    * @param beta Valor beta para la poda
    * @param heuristic Heurística utilizada para evaluar los estados
    * @return double Valor heurístico del mejor movimiento encontrado
    */
   double Poda_AlfaBeta_Ordenacion(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;
   


   /** 
      * @brief Función que implementa una versión probabilística del algoritmo de poda alfa-beta.
      * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
      * Esta versión considera la probabilidad de éxito de cada movimiento y ajusta los valores alfa y beta en consecuencia.
      * El objetivo es reducir aún más el número de nodos evaluados, permitiendo podas más agresivas cuando
      * no parece que el resultado actual vaya a superar significativamente el mejor hasta el momento.
      * @param actual Estado actual del juego
      * @param jugador Id del jugador actual
      * @param profundidad Profundidad actual en el árbol de búsqueda
      * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
      * @param c_piece Color de la ficha a mover
      * @param id_piece Id de la ficha a mover
      * @param dice Valor del dado a usar
      * @param alpha Valor alfa para la poda
      * @param beta Valor beta para la poda
      * @param heuristic Heurística utilizada para evaluar los estados
      * @return double Valor heurístico del mejor movimiento encontrado
   */
   double Poda_AlfaBeta_Probabilistica(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;




   /**
      * @brief Función que implementa una versión final del algoritmo de poda alfa-beta.
      * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
      * Esta versión es una optimización de la poda alfa-beta que combina las mejoras anteriores y
      * busca maximizar la eficiencia del algoritmo al reducir el número de nodos evaluados.
      * @param actual Estado actual del juego
      * @param jugador Id del jugador actual
      * @param profundidad Profundidad actual en el árbol de búsqueda
      * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
      * @param c_piece Color de la ficha a mover
      * @param id_piece Id de la ficha a mover
      * @param dice Valor del dado a usar
      * @param alpha Valor alfa para la poda
      * @param beta Valor beta para la poda
      * @param heuristic Heurística utilizada para evaluar los estados
      * @return double Valor heurístico del mejor movimiento encontrado
   */
   double Poda_Final2DefinitivaAhoraSi(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;

   /**
   * @brief Función que implementa una versión mejorada del algoritmo de poda alfa-beta.
   * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
   * Esta versión mejora la eficiencia del algoritmo al considerar las mejores jugadas posibles en cada paso,
   * lo que permite una poda más efectiva y una búsqueda más rápida del mejor movimiento.
   * @param actual Estado actual del juego
   * @param jugador Id del jugador actual
   * @param profundidad Profundidad actual en el árbol de búsqueda
   * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
   * @param c_piece Color de la ficha a mover
   * @param id_piece Id de la ficha a mover
   * @param dice Valor del dado a usar
   * @param alpha Valor alfa para la poda
   * @param beta Valor beta para la poda
   * @param heuristic Heurística utilizada para evaluar los estados
   * @return double Valor heurístico del mejor movimiento encontrado
   */
   double Poda_AlfaBeta_Mejorada(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;

    /**
   * @brief Función que implementa una versión mejorada del algoritmo de poda alfa-beta.
   * Asigna a las variables pasadas por referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
   * Esta versión mejora la eficiencia del algoritmo al considerar las mejores jugadas posibles en cada paso,
   * lo que permite una poda más efectiva y una búsqueda más rápida del mejor movimiento.
   * @param actual Estado actual del juego
   * @param jugador Id del jugador actual
   * @param profundidad Profundidad actual en el árbol de búsqueda
   * @param profundidad_max Profundidad máxima a alcanzar en la búsqueda
   * @param c_piece Color de la ficha a mover
   * @param id_piece Id de la ficha a mover
   * @param dice Valor del dado a usar
   * @param alpha Valor alfa para la poda
   * @param beta Valor beta para la poda
   * @param heuristic Heurística utilizada para evaluar los estados
   * @return double Valor heurístico del mejor movimiento encontrado
   */
   double Poda_AlfaBeta_SegundaMejora(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;

   /*
   double Poda_AlfaBeta_Dinamica(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
      int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;*/
   /*
   bool esEstadoQuieto(const Parchis &estado) const;

   double buscarQuietud(const Parchis &actual, int jugador, int profundidad, 
                        double alpha, double beta, Heuristic *heuristic) const;

   double Poda_AlfaBeta_Quiescence(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
                                    int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const;
   */

   /**
    * @brief Método que determina si el player es inteligente (decide el mejor movimiento)
    * o no. True para AIPlayer.
    *
    * @return true
    * @return false
    */
   inline virtual bool canThink() const{ return true; }
   
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


/**
 * @brief Propuesta de declaración de la función poda alfa-beta.
 * La propuesta es solo sugerencia, los parámetros de la declaración podrían variar.
 */
// float Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, float alpha, float beta, Heuristic *heuristic);





#endif
