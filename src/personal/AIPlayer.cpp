# include "AIPlayer.h"
# include "../../include/model/Parchis.h"

const float masinf = 9999999999.0, menosinf = -9999999999.0;
const float gana = masinf / 10.f, pierde = menosinf / 10.f;
const int num_pieces = 2;
const int PROFUNDIDAD_MINIMAX = 4; // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta
const double EPSILON = 0.5; // Umbral para la poda probabilística

bool AIPlayer::move(){
   cout << COUT_ORANGE_BOLD << "Realizo un movimiento automatico" << COUT_NOCOLOR << endl;

   color c_piece;
   int id_piece;
   int dice;
   think(c_piece, id_piece, dice);

   cout << COUT_ORANGE_BOLD << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << COUT_NOCOLOR << endl;

   actual->movePiece(c_piece, id_piece, dice);
   return true;
}

void AIPlayer::think(color& c_piece, int& id_piece, int& dice) const{
  
   // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

   float valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
   float alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
   // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
   ValoracionTest valoracionTest;
   miValoracion3 miValoracion3;
   miValoracion2 miValoracion2;
   miValoracion1 miValoracion1;


   // ----------------------------------------------------------------- //

   // Si quiero poder manejar varios comportamientos, puedo usar la variable id del agente para usar una u otra.
   switch (id)
   {
   case 0:
      // Mi implementación base de la poda con ValoracionTest
      valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracionTest);
      break;
   case 1:
      // Mi implementación definitiva con la que gano a todos los ninjas.
      valor = Poda_Final2DefinitivaAhoraSi(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion3);
      break;
   case 2:
      // Las distintas pruebas que he realizado (primera prueba)
      valor = Poda_AlfaBeta_Mejorada(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion1);
      break;
   case 3:
      // Las distintas pruebas que he realizado (segunda prueba)
      valor = Poda_AlfaBeta_SegundaMejora(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion2);
      break;
   case 4:
      valor = Poda_AlfaBeta_Ordenacion(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &miValoracion2);
      break;
   case 5:
      valor = Poda_AlfaBeta_Probabilistica(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, &valoracionTest);
      break;
    case 6:
      thinkAleatorio(c_piece, id_piece, dice);
      break;
    case 7:
      thinkFichaMasAdelantada(c_piece, id_piece, dice);
      break;
    case 8:
      thinkMejorOpcion(c_piece, id_piece, dice);
      break;
    case 9:
      valor = Minimax(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &miValoracion3);
      break;
    case 10:
      valor = Minimax(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracionTest);
      break;
    case 11:
      valor = Minimax_Limitado(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &valoracionTest);
      break;
    case 12:
      valor = Minimax_Limitado(*actual, jugador, 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, &miValoracion3);
      break;
   }

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////
/*Tutorial*/
////////////

void AIPlayer::thinkAleatorio(color& c_piece, int& id_piece, int& dice) const{
   // IMPLEMENTACIÓN INICIAL DEL AGENTE
   // Esta implementación realiza un movimiento aleatorio.
   // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
   //como lo que se muestran al final de la función.

   // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
   //respectivamente, de:
   // - color de ficha a mover
   // - identificador de la ficha que se va a mover
   // - valor del dado con el que se va a mover la ficha.

   // El id de mi jugador actual.
   int player = actual->getCurrentPlayerId();

   // Vector que almacenará los dados que se pueden usar para el movimiento
   vector<int> current_dices;
   // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
   vector<tuple<color, int>> current_pieces;

   // Se obtiene el vector de dados que se pueden usar para el movimiento
   current_dices = actual->getAvailableNormalDices(player);
   // Elijo un dado de forma aleatoria.
   dice = current_dices[rand() % current_dices.size()];

   // Se obtiene el vector de fichas que se pueden mover para el dado elegido
   current_pieces = actual->getAvailablePieces(player, dice);

   // Si tengo fichas para el dado elegido muevo una al azar.
   if (current_pieces.size() > 0){
      int random_id = rand() % current_pieces.size();
      id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
      c_piece = get<0>(current_pieces[random_id]); // elemento de la tupla
   }
   else{
      // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }
}

void AIPlayer::thinkFichaMasAdelantada(color& c_piece, int& id_piece, int& dice) const{
   
   // Elijo el dado haciendo lo mismo que el jugador aleatorio.
   
   thinkAleatorio(c_piece, id_piece, dice);

   
   // Tras llamar a esta función, ya tengo en dice el número de dado que quiero usar.
   // Ahora, en vez de mover una ficha al azar, voy a mover (o a aplicar
   // el dado especial a) la que esté más adelantada
   // (equivalentemente, la más cercana a la meta).

   int player = actual->getCurrentPlayerId();
   vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
   int id_ficha_mas_adelantada = -1;
   color col_ficha_mas_adelantada = none;
   int min_distancia_meta = 9999;

   for (int i = 0; i < current_pieces.size(); i++)
   {
      // distanceToGoal(color, id) devuelve la distancia a la meta de la ficha [id] del color que le indique.
      color col = get<0>(current_pieces[i]);
      int id = get<1>(current_pieces[i]);
      int distancia_meta = actual->distanceToGoal(col, id);
      if (distancia_meta < min_distancia_meta)
      {
         min_distancia_meta = distancia_meta;
         id_ficha_mas_adelantada = id;
         col_ficha_mas_adelantada = col;
      }
   }
   // Si no he encontrado ninguna ficha, paso turno.
   if (id_ficha_mas_adelantada == -1)
   {
      id_piece = SKIP_TURN;
      c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
   }
   // En caso contrario, moveré la ficha más adelantada.
   else
   {
      id_piece = id_ficha_mas_adelantada;
      c_piece = col_ficha_mas_adelantada;
   }

}

void AIPlayer::thinkMejorOpcion(color& c_piece, int& id_piece, int& dice) const{

   // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual.
   // Cuando ya he recorrido todos los hijos, la función devuelve el estado actual. De esta forma puedo saber
   // cuándo paro de iterar.
   // Para ello, vamos a iterar sobre los hijos con la función de Parchis getChildren().
   // Esta función devuelve un objeto de la clase ParchisBros, que es una estructura iterable
   // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama.

   ParchisBros hijos = actual->getChildren();

   bool me_quedo_con_esta_accion = false;


   // La clase ParchisBros viene con un iterador muy útil y sencillo de usar.
   // Al hacer begin() accedemos al primer hijo de la rama,
   // y cada vez que hagamos ++it saltaremos al siguiente hijo.
   // Comparando con el iterador end() podemos consultar cuándo hemos terminado de visitar los hijos.

   for (ParchisBros::Iterator it = hijos.begin();
      it != hijos.end() and !me_quedo_con_esta_accion; ++it)
   {
      Parchis siguiente_hijo = *it;

      // Si en el movimiento elegido comiera ficha, llegara a la meta o ganara, me quedo con esa acción.
      // Termino el bucle en este caso.
      if (siguiente_hijo.isEatingMove() or
      siguiente_hijo.isGoalMove() or
      (siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador))
      {
         me_quedo_con_esta_accion = true;
         c_piece = it.getMovedColor();
         id_piece = it.getMovedPieceId();
         dice = it.getMovedDiceValue();
      }
   }


   // Si he encontrado una acción que me interesa, la guardo en las variables pasadas por referencia.
   // (Ya lo he hecho antes, cuando les he asignado los valores con el iterador).
   // Si no he encontrado ninguna acción que me interese, voy a mover la ficha más adelantada como en el caso
   //anterior.

   if(!me_quedo_con_esta_accion){
      thinkFichaMasAdelantada(c_piece, id_piece, dice);
   }

}

float AIPlayer::Minimax(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, Heuristic *heuristic) const
{
    if (profundidad == profundidad_max || actual.gameOver())
    { // Nodo terminal o profundidad límite: llamo a la función heurística
        // IMPORTANTE: USAMOS EL MÉTODO evaluate AUNQUE HAYAMOS REDEFINIDO LA CLASE HEURISTIC
        return heuristic->evaluate(actual, jugador);
    }
    // Comparo mi jugador con el actual de la partida para saber en qué tipo de nodo estoy
    else if (actual.getCurrentPlayerId() == jugador)
    { // Nodo MAX
        float valor = menosinf; // Inicialización lo más pequeña posible para ir calculando el máximo
        // Obtengo los hijos del nodo actual y los recorro
        vector<ParchisSis> rama = actual.getChildrenList();
      for (int i = 0; i < rama.size(); i++)
        {
         ParchisSis hijo_i = rama[i];  // Acceso al estado con el hijo i-ésimo del vector
         Parchis nuevo_hijo = *hijo_i; // Acceso al estado            
         // Búsqueda en profundidad (llamada recursiva)
            float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, heuristic);
            if (new_val > valor)
            {
                // Me voy quedando con el máximo
                valor = new_val;
                if (profundidad == 0)
                {
                  // Almaceno el movimiento que me ha llevado al mejor valor (solo en la raíz)
                  c_piece = hijo_i.getMovedColor();
                  id_piece = hijo_i.getMovedPieceId();
                  dice = hijo_i.getMovedDiceValue();
                }
            }
        }
        return valor;
    }
    else
    { // Nodo MIN
        float valor = masinf; // Inicialización lo más grande posible para ir calculando el mínimo
        // Obtengo los hijos del nodo actual y los recorro
        ParchisBros rama = actual.getChildren();
        for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
        {
            Parchis nuevo_hijo = *it;
            // Búsqueda en profundidad (llamada recursiva)
            float new_val = Minimax(nuevo_hijo, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, heuristic);
            // Me voy quedando con el mínimo
            if (new_val < valor)
            {
                valor = new_val;
            }
        }
        return valor;
    }
}

float AIPlayer::Minimax_Limitado(const Parchis &actual, int jugador, int profundidad, int profundidad_max,
                                 color &c_piece, int &id_piece, int &dice, Heuristic *heuristic) const
{
    if (profundidad == profundidad_max || actual.gameOver())
    { // Nodo terminal o profundidad límite: llamo a la función heurística
        return heuristic->evaluate(actual, jugador);
    }
    else if (actual.getCurrentPlayerId() == jugador)
    { // Nodo MAX
        float valor = menosinf;
        ParchisBros rama = actual.getChildren();

        for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
        {
            Parchis nuevo_hijo = *it;

            if (NodeCounter::isLimitReached())
            {
                std::cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << std::endl;
                if (profundidad == 0)
                {
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();
                }
                return valor;
            }

            float new_val = Minimax_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max,
                                             c_piece, id_piece, dice, heuristic);

            if (new_val > valor)
            {
                valor = new_val;
                if (profundidad == 0)
                {
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();
                }
            }
        }
        return valor;
    }
    else
    { // Nodo MIN
        float valor = masinf;
        ParchisBros rama = actual.getChildren();

        for (ParchisBros::Iterator it = rama.begin(); it != rama.end(); ++it)
        {
            Parchis nuevo_hijo = *it;

            if (NodeCounter::isLimitReached())
            {
                std::cout << "Límite de nodos alcanzado, devolviendo el mejor nodo parcial" << std::endl;
                return heuristic->evaluate(actual, jugador);
            }

            float new_val = Minimax_Limitado(nuevo_hijo, jugador, profundidad + 1, profundidad_max,
                                             c_piece, id_piece, dice, heuristic);

            if (new_val < valor)
            {
                valor = new_val;
            }
        }
        return valor;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////
/*Poda AlfaBeta y mejoras*/
///////////////////////////

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
   int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const{
      if(profundidad == profundidad_max || actual.gameOver()){
         return heuristic->evaluate(actual, jugador);
      }

      ParchisBros hijos = actual.getChildren();

      if(jugador == actual.getCurrentPlayerId()){
         for(ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it){
            Parchis hijo = *it;

            double valor = Poda_AlfaBeta(hijo, jugador, profundidad+1, profundidad_max, c_piece, id_piece, dice, alpha,
             beta, heuristic);

            if(alpha < valor){
               alpha = valor;

               if(profundidad == 0){
                  c_piece = it.getMovedColor();
                  id_piece = it.getMovedPieceId();
                  dice = it.getMovedDiceValue();
               }

               if(beta <= alpha) return beta;
            }
         }

         return alpha;
      }else{
         for(ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it){
            Parchis hijo = *it;

            double valor = Poda_AlfaBeta(hijo, jugador, profundidad+1, profundidad_max, c_piece, 
            id_piece, dice, alpha, beta, heuristic);

            if(valor < beta){
               beta = valor;
               if(beta <= alpha) return alpha;
            }
         }
         return beta;
      }
   }



double AIPlayer::Poda_AlfaBeta_Ordenacion(const Parchis &actual, int jugador, int profundidad, int profundidad_max, 
                                          color& c_piece, int& id_piece, int& dice, 
                                          double alpha, double beta, Heuristic *heuristic) const {
    if (profundidad == profundidad_max || actual.gameOver()) {
        return heuristic->evaluate(actual, jugador);
    }

    // Si estamos en niveles profundos, llamamos directamente a la poda Alfa-Beta básica
    if (profundidad >= profundidad_max - 2) {
        return Poda_AlfaBeta(actual, jugador, profundidad, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
    }

    // Generar todos los hijos de una vez usando ParchisSis
    std::vector<ParchisSis> hijos = actual.getChildrenList();
    std::vector<std::pair<double, ParchisSis>> valores_hijos;

    // Evaluar cada hijo y guardar su valor heurístico junto con el nodo
    for (ParchisSis hijo : hijos) {
        double valor_heuristico = heuristic->evaluate(*hijo, jugador);

        if (valor_heuristico > alpha || valor_heuristico < beta) {
            valores_hijos.push_back({valor_heuristico, hijo});
        }
        //valores_hijos.push_back({valor_heuristico, hijo});
    }

    // Ordenar los hijos según el tipo de nodo (MAX o MIN)
    if (jugador == actual.getCurrentPlayerId()) {
        // Nodo MAX: ordenar de mayor a menor
        std::sort(valores_hijos.begin(), valores_hijos.end(),
                  [](const std::pair<double, ParchisSis>& a, const std::pair<double, ParchisSis>& b) {
                      return a.first > b.first; // Comparar solo el valor heurístico
                  });
    } else {
        // Nodo MIN: ordenar de menor a mayor
        std::sort(valores_hijos.begin(), valores_hijos.end(),
                  [](const std::pair<double, ParchisSis>& a, const std::pair<double, ParchisSis>& b) {
                      return a.first < b.first; // Comparar solo el valor heurístico
                  });
    }

    // Realizar la poda Alfa-Beta
    if (jugador == actual.getCurrentPlayerId()) { // Nodo MAX
        for (const auto& [valor_heuristico, hijo] : valores_hijos) {
            double valor = Poda_AlfaBeta_Ordenacion(*hijo, jugador, profundidad + 1, profundidad_max, 
                                                    c_piece, id_piece, dice, alpha, beta, heuristic);

            if (valor > alpha) {
                alpha = valor;

                if (profundidad == 0) {
                    c_piece = hijo.getMovedColor();
                    id_piece = hijo.getMovedPieceId();
                    dice = hijo.getMovedDiceValue();
                }

                if (beta <= alpha) {
                    return beta; // Poda tradicional
                }
            }
        }
        return alpha;
    } else { // Nodo MIN
        for (const auto& [valor_heuristico, hijo] : valores_hijos) {
            double valor = Poda_AlfaBeta_Ordenacion(*hijo, jugador, profundidad + 1, profundidad_max, 
                                                    c_piece, id_piece, dice, alpha, beta, heuristic);

            if (valor < beta) {
                beta = valor;

                if (beta <= alpha) {
                    return alpha; // Poda tradicional
                }
            }
        }
        return beta;
    }
}




   double AIPlayer::Poda_AlfaBeta_Probabilistica(const Parchis &actual, int jugador, int profundidad, int profundidad_max, 
      color& c_piece, int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const {

    if (profundidad == profundidad_max || actual.gameOver()) {
        return heuristic->evaluate(actual, jugador);
    }

    ParchisBros hijos = actual.getChildren();

    if (jugador == actual.getCurrentPlayerId()) { // Nodo MAX
        for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it) {
            Parchis hijo = *it;

            double valor = Poda_AlfaBeta_Probabilistica(hijo, jugador, profundidad + 1, profundidad_max, 
                                                        c_piece, id_piece, dice, alpha, beta, heuristic);

            if (valor > alpha) {
                alpha = valor;

                if (profundidad == 0) {
                    c_piece = it.getMovedColor();
                    id_piece = it.getMovedPieceId();
                    dice = it.getMovedDiceValue();
                }

                // Poda tradicional
                if (beta <= alpha) {
                    return beta;
                }

                // Poda probabilística
                if (beta - alpha < EPSILON) {
                    return alpha; // Asumimos que no habrá mejora significativa
                }
            }
        }
        return alpha;
    } else { // Nodo MIN
        for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it) {
            Parchis hijo = *it;

            double valor = Poda_AlfaBeta_Probabilistica(hijo, jugador, profundidad + 1, profundidad_max, 
                                                        c_piece, id_piece, dice, alpha, beta, heuristic);

            if (valor < beta) {
                beta = valor;

                // Poda tradicional
                if (beta <= alpha) {
                    return alpha;
                }

                // Poda probabilística
                if (beta - alpha < EPSILON) {
                    return beta; // Asumimos que no habrá mejora significativa
                }
            }
        }
        return beta;
    }
}

double AIPlayer::Poda_Final2DefinitivaAhoraSi(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
   int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const{

      return Poda_AlfaBeta_Probabilistica(actual, jugador, profundidad, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);      
   }

double AIPlayer::Poda_AlfaBeta_Mejorada(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
    int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const{
 
       return Poda_AlfaBeta_Probabilistica(actual, jugador, profundidad, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);      
    }

double AIPlayer::Poda_AlfaBeta_SegundaMejora(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color& c_piece,
    int& id_piece, int& dice, double alpha, double beta, Heuristic *heuristic) const{
        
        return Poda_AlfaBeta_Probabilistica(actual, jugador, profundidad, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);      
    }



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////
/*Heuristicas*/
///////////////
float ValoracionTest::getHeuristic(const Parchis& estado, int jugador) const{
   // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


   int ganador = estado.getWinner();
   int oponente = (jugador + 1) % 2;

   // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
   if (ganador == jugador){
      return gana;
   }
   else if (ganador == oponente){
      return pierde;
   }
   else{
      // Colores que juega mi jugador y colores del oponente
      vector<color> my_colors = estado.getPlayerColors(jugador);
      vector<color> op_colors = estado.getPlayerColors(oponente);

      // Recorro todas las fichas de mi jugador
      int puntuacion_jugador = 0;
      // Recorro colores de mi jugador.
      for (int i = 0; i < my_colors.size(); i++){
         color c = my_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            // Valoro positivamente que la ficha esté en casilla segura o meta.
            if (estado.isSafePiece(c, j)){
               puntuacion_jugador++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal){
               puntuacion_jugador += 5;
            }
         }
      }

      // Recorro todas las fichas del oponente
      int puntuacion_oponente = 0;
      // Recorro colores del oponente.
      for (int i = 0; i < op_colors.size(); i++){
         color c = op_colors[i];
         // Recorro las fichas de ese color.
         for (int j = 0; j < num_pieces; j++){
            if (estado.isSafePiece(c, j)){
               // Valoro negativamente que la ficha esté en casilla segura o meta.
               puntuacion_oponente++;
            }
            else if (estado.getBoard().getPiece(c, j).get_box().type == goal){
               puntuacion_oponente += 5;
            }
         }
      }

      // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
      return puntuacion_jugador - puntuacion_oponente;
   }
}



float miValoracion1::getHeuristic(const Parchis& estado, int jugador) const {
    const int oponente = (jugador + 1) % 2;
 
    const float PESO_META = 100.0;
    const float PESO_DISTANCIA = -1.0;
    const float PESO_SEGURA = 5.0;
    const float PESO_BARRERA = 7.0;
    const float PESO_BONUS_GOAL = 10.0;
    const float PESO_BONUS_EAT = 20.0;
 
    float valor_jugador = 0.0, valor_oponente = 0.0;
 
    // Colores de ambos jugadores
    const std::vector<color> colores_jugador = estado.getPlayerColors(jugador);
    const std::vector<color> colores_oponente = estado.getPlayerColors(oponente);
 
    // Fichas del jugador
    for (color c : colores_jugador) {
        const auto& piezas = estado.getBoard().getPieces(c);
        for (int i = 0; i < piezas.size(); ++i) {
            const Box& casilla = piezas[i].get_box();
 
            // En meta
            if (casilla.type == goal)
                valor_jugador += PESO_META;
 
            // Distancia a la meta
            valor_jugador += PESO_DISTANCIA * estado.distanceToGoal(c, i);
 
            // Ficha en casilla segura
            if (estado.isSafePiece(c, i))
                valor_jugador += PESO_SEGURA;
        }
 
        // Barrera si las dos fichas están en la misma casilla
        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_jugador += PESO_BARRERA;
    }
 
    // Bonus por comer o llegar a meta
    if (estado.isEatingMove())
        valor_jugador += PESO_BONUS_EAT;
    if (estado.isGoalMove())
        valor_jugador += PESO_BONUS_GOAL;
 
    // Fichas del oponente
    for (color c : colores_oponente) {
        const auto& piezas = estado.getBoard().getPieces(c);
        for (int i = 0; i < piezas.size(); ++i) {
            const Box& casilla = piezas[i].get_box();
 
            if (casilla.type == goal)
                valor_oponente += PESO_META;
 
            valor_oponente += PESO_DISTANCIA * estado.distanceToGoal(c, i);
 
            if (estado.isSafePiece(c, i))
                valor_oponente += PESO_SEGURA;
        }
 
        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_oponente += PESO_BARRERA;
    }
 
    if (estado.isEatingMove())
        valor_oponente += PESO_BONUS_EAT;
    if (estado.isGoalMove())
        valor_oponente += PESO_BONUS_GOAL;
 
    return valor_jugador - valor_oponente;
 }




float miValoracion2::getHeuristic(const Parchis& estado, int jugador) const {
    const int oponente = (jugador + 1) % 2;
    const int ganador = estado.getWinner();

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador){
        return gana;
    }
    else if (ganador == oponente){
        return pierde;
    }


    int turno = estado.getTurn();
 
    float PESO_META = 100.0;
    float PESO_DISTANCIA = -1.0;
    float PESO_SEGURA = 5.0;
    float PESO_BARRERA = 7.0;
    float PESO_BONUS_GOAL = 10.0;
    float PESO_BONUS_EAT = 20.0;
    float PESO_CASA = -60.0;
    float PESO_REBOTES = -5.0;
    float PESO_MOVILIDAD = 8.0;
    float PESO_TURNO_DOBLE = 20.0;
    float PESO_COMER_POSIBLE = 100.0;
    float PESO_VULNERABLE = -100.0;
 

    // Ajuste en primeros turnos
    if (turno < 5) {
        PESO_COMER_POSIBLE = 60.0;
        PESO_DISTANCIA = -0.6;
    }
 
    float valor_jugador = 0.0, valor_oponente = 0.0;
 
    const std::vector<color> colores_jugador = estado.getPlayerColors(jugador);
    const std::vector<color> colores_oponente = estado.getPlayerColors(oponente);
 
    for (color c : colores_jugador) {
        const auto& piezas = estado.getBoard().getPieces(c);
        valor_jugador += PESO_REBOTES * estado.getBounces(c);
        valor_jugador += PESO_MOVILIDAD * estado.getAvailableNormalDices(jugador).size();
 
        for (int i = 0; i < piezas.size(); ++i) {
            const Box& b = piezas[i].get_box();
 
            if (b.type == goal)
                valor_jugador += PESO_META;
            else
                valor_jugador += PESO_DISTANCIA * estado.distanceToGoal(c, i);
 
            if (estado.isSafePiece(c, i)) valor_jugador += PESO_SEGURA;
            if (b.type == home) valor_jugador += PESO_CASA;
            if (b.type == final_queue) valor_jugador += 40;
 
            // Comer posible y vulnerabilidad
            for (color c_op : colores_oponente) {
                for (int j = 0; j < 2; ++j) {
                    int d = estado.distanceBoxtoBox(c, i, c_op, j);
                    if (d != -1 && d <= 6 && !estado.isSafePiece(c_op, j))
                        valor_jugador += PESO_COMER_POSIBLE;
 
                    int rev = estado.distanceBoxtoBox(c_op, j, c, i);
                    if (rev != -1 && rev <= 6 && !estado.isSafePiece(c, i))
                        valor_jugador += PESO_VULNERABLE;
 
                }
            }
        }
 
        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_jugador += PESO_BARRERA;
    }
 
    if (estado.isEatingMove()) valor_jugador += PESO_BONUS_EAT;
    if (estado.isGoalMove()) valor_jugador += PESO_BONUS_GOAL;
 
    // Turno doble
    bool turno_doble = (jugador == 0 && (turno % 4 == 0 || turno % 4 == 3)) ||
                       (jugador == 1 && (turno % 4 == 1 || turno % 4 == 2));
    if (turno_doble) valor_jugador += PESO_TURNO_DOBLE;
 
    // Oponente
    for (color c : colores_oponente) {
        const auto& piezas = estado.getBoard().getPieces(c);
        valor_oponente += PESO_REBOTES * estado.getBounces(c);
        valor_oponente += PESO_MOVILIDAD * estado.getAvailableNormalDices(oponente).size();
 
        for (int i = 0; i < piezas.size(); ++i) {
            const Box& b = piezas[i].get_box();
 
            if (b.type == goal)
                valor_oponente += PESO_META;
            else
                valor_oponente += PESO_DISTANCIA * estado.distanceToGoal(c, i);
 
            if (estado.isSafePiece(c, i)) valor_oponente += PESO_SEGURA;
            if (b.type == home) valor_oponente += PESO_CASA;
            if (b.type == final_queue) valor_oponente += 40;
 
            for (color c_j : colores_jugador) {
                for (int j = 0; j < 2; ++j) {
                    int d = estado.distanceBoxtoBox(c, i, c_j, j);
                    if (d != -1 && d <= 6 && !estado.isSafePiece(c_j, j))
                        valor_oponente += PESO_COMER_POSIBLE;
 
                    int rev = estado.distanceBoxtoBox(c_j, j, c, i);
                    if (rev != -1 && rev <= 6 && !estado.isSafePiece(c, i))
                        valor_oponente += PESO_VULNERABLE;
 
                }
            }
        }
 
        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_oponente += PESO_BARRERA;
    }
 
    if (estado.isEatingMove()) valor_oponente += PESO_BONUS_EAT;
    if (estado.isGoalMove()) valor_oponente += PESO_BONUS_GOAL;
 
    return valor_jugador - valor_oponente;
 }




//////////////////
/*La definitiva*/
////////////////

float miValoracion3::getHeuristic(const Parchis& estado, int jugador) const {
    const int oponente = (jugador + 1) % 2;
    const int ganador = estado.getWinner();
    const float GANA = 1e6;
    const float PIERDE = -1e6;

    if (ganador == jugador) return GANA;
    if (ganador == oponente) return PIERDE;

    const int turno = estado.getTurn();

    // Cambio la agresividad a lo largo de la partida
    float mix_agresiva = 0.0;
    if (turno < 5) mix_agresiva = 0.2;
    else if (turno < 25) mix_agresiva = 0.5;
    else mix_agresiva = 0.6;

    float PESO_META = 100.0;
    float PESO_DISTANCIA = -1.0;
    float PESO_SEGURA = 5.0;
    float PESO_BARRERA = 6.0;
    float PESO_GOAL = 30.0;
    float PESO_EAT = 40.0;
    float PESO_CASA = -60.0;
    float PESO_REBOTES = -5.0;
    float PESO_MOVILIDAD = 8.0;
    float PESO_TURNO_DOBLE = 20.0;
    float PESO_COMER_POSIBLE = 60.0;
    float PESO_VULNERABLE = -60.0;
    float PESO_PELIGRO = 15.0;

    //funcion lambda para detectar amenazas sobre alguna ficha
    auto hay_amenaza_directa = [&](const color& c1, int id1, const color& c2, int id2) {
        int d = estado.distanceBoxtoBox(c1, id1, c2, id2);
        return d != -1 && d <= 6 && !estado.isSafePiece(c2, id2);
    };

    float valor_jugador = 0.0, valor_oponente = 0.0;
    auto colores_jugador = estado.getPlayerColors(jugador);
    auto colores_oponente = estado.getPlayerColors(oponente);

    for (color c : colores_jugador) {
        const auto& piezas = estado.getBoard().getPieces(c);
        valor_jugador += PESO_REBOTES * estado.getBounces(c);

        for (int i = 0; i < piezas.size(); ++i) {
            const Box& b = piezas[i].get_box();

            if (b.type == goal) valor_jugador += PESO_META;
            else valor_jugador += PESO_DISTANCIA * estado.distanceToGoal(c, i);

            if (estado.isSafePiece(c, i)) valor_jugador += PESO_SEGURA;
            if (b.type == home) valor_jugador += PESO_CASA;
            if (b.type == final_queue) valor_jugador += 40;

            for (color co : colores_oponente) {
                for (int j = 0; j < 2; ++j) {
                    bool amenaza = hay_amenaza_directa(c, i, co, j);
                    bool vulnerable = hay_amenaza_directa(co, j, c, i);

                    if (amenaza) {
                        float valor_comer = mix_agresiva * 100 + (1 - mix_agresiva) * PESO_COMER_POSIBLE;
                        valor_jugador += std::max(PESO_COMER_POSIBLE, valor_comer);
                    }

                    if (vulnerable) {
                        float valor_vulnerable = mix_agresiva * (-100) + (1 - mix_agresiva) * PESO_VULNERABLE;
                        valor_jugador += std::min(PESO_VULNERABLE, valor_vulnerable);
                    }

                    int d = estado.distanceBoxtoBox(c, i, co, j);
                    if (d != -1 && d < 6 && !estado.isSafePiece(co, j))
                        valor_jugador += PESO_PELIGRO;
                }
            }
        }

        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_jugador += PESO_BARRERA;
    }

    valor_jugador += PESO_MOVILIDAD * estado.getAvailableNormalDices(jugador).size();
    if (estado.getCurrentPlayerId() == jugador && estado.isEatingMove()) valor_jugador += PESO_EAT;
    if (estado.getCurrentPlayerId() == jugador && estado.isGoalMove()) valor_jugador += PESO_GOAL;

    bool turno_doble = (jugador == 0 && (turno % 4 == 0 || turno % 4 == 3)) ||
                       (jugador == 1 && (turno % 4 == 1 || turno % 4 == 2));
    if (turno_doble) valor_jugador += PESO_TURNO_DOBLE;

    // Oponente
    for (color c : colores_oponente) {
        const auto& piezas = estado.getBoard().getPieces(c);
        valor_oponente += PESO_REBOTES * estado.getBounces(c);

        for (int i = 0; i < piezas.size(); ++i) {
            const Box& b = piezas[i].get_box();

            if (b.type == goal) valor_oponente += PESO_META;
            else valor_oponente += PESO_DISTANCIA * estado.distanceToGoal(c, i);

            if (estado.isSafePiece(c, i)) valor_oponente += PESO_SEGURA;
            if (b.type == home) valor_oponente += PESO_CASA;
            if (b.type == final_queue) valor_oponente += 40;

            for (color cj : colores_jugador) {
                for (int j = 0; j < 2; ++j) {
                    bool amenaza = hay_amenaza_directa(c, i, cj, j);
                    bool vulnerable = hay_amenaza_directa(cj, j, c, i);

                    if (amenaza) {
                        float valor_comer = mix_agresiva * 100 + (1 - mix_agresiva) * PESO_COMER_POSIBLE;
                        valor_oponente += std::max(PESO_COMER_POSIBLE, valor_comer);
                    }

                    if (vulnerable) {
                        float valor_vulnerable = mix_agresiva * (-100) + (1 - mix_agresiva) * PESO_VULNERABLE;
                        valor_oponente += std::min(PESO_VULNERABLE, valor_vulnerable);
                    }

                    int d = estado.distanceBoxtoBox(c, i, cj, j);
                    if (d != -1 && d < 6 && !estado.isSafePiece(cj, j))
                        valor_oponente += PESO_PELIGRO;
                }
            }
        }

        if (piezas.size() >= 2 && piezas[0].get_box() == piezas[1].get_box())
            valor_oponente += PESO_BARRERA;
    }

    valor_oponente += PESO_MOVILIDAD * estado.getAvailableNormalDices(oponente).size();
    if (estado.getCurrentPlayerId() == oponente && estado.isEatingMove()) valor_oponente += PESO_EAT;
    if (estado.getCurrentPlayerId() == oponente && estado.isGoalMove()) valor_oponente += PESO_GOAL;

    return valor_jugador - valor_oponente;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////