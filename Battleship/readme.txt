Projeto realizado em ambiente acadêmico - CC2013, FCUP, 2019/20.

################# Compilação e execução #################################

Para compilar com as quadtrees, basta executar o comando: 'make quadtree' (ou apenas, 'make').
Para compilar com as matrizes, basta executar o comando: 'make matrix'.

Depois de compilar, para ambos os casos, para começar a execução do jogo: './game'.

Para remover os object files e o executável final: 'make clean'.

################# Regras/Funcionamento do jogo ###########################

NOTA: - Todo o input é validado. É suposto o jogo não crashar com input inválido e dá ainda feedback personalizado para os inputs inválidos.

O jogo inicia perguntando se os players vão querer escolher manualmente o setup ("manual" ou "m"), ou é para ser gerado de forma random ("random" ou "r").

Caso seja para ser escolhido manualmente, é, de seguida, pedido o tamanho dos mapas (entre 20 e 40, inclusive), depois o número de peças por tipo e, por fim, o primeiro player a atacar (1 ou 2).
Caso tenha sido escolhido a geração random, este passo não existe.
(Na geração random, todas os tipos têm pelo menos uma peça)

Posto isto, em ambos os casos, é mostrado as configurações que o jogo vai ter e se os players confirmam ("yes" ou "y") ou não ("no" ou "n").
Caso confirmem, é iniciado o processo de inserção das peças, caso não, é repetido o passo anterior, até os players, eventualmente, confirmarem.

No processo de inserção de peças:
   - Caso estejamos no modo de escolha manual, começa o player1 por inserir peça a peça, inserindo as coordenadas (dois numeros separados por um (ou mais) espaço(s), entre 1 e o tamanho do mapa, inclusive) e rotação (0, 90, 180, 270, 360), recebendo feedback da inserção.
     No final, é mostrado o seu mapa.
     Repete-se depois o processo, agora para o player2.
   - Caso estejamos em escolha random, em background, as peças obtêm coordenadas e rotações random e são inseridas, automaticamente.
     É mostrado o mapa dos players, tal como na fase anterior.
    (A ideia de mostrar o mapa das peças é para os players ganharem uma imagem mental do seu setup).
    (O mapa dos players é constituído por caracters: '.', 'I', 'P', 'T', 'X' ou 'Z',
    signficando "Sem peça", "Peça do tipo I", "Peça do tipo P", "Peça do tipo T", "Peça do tipo X" e "Peça do tipo Z", respetivamente.)

Posto isto, é iniciado, digamos, o "game play", ie, a fase dos ataques.
Começa a atacar o player que ficou atrás decidido e é-lhe pedido as coordenadas do ataque. (dois numeros separados por um (ou mais) espaço(s), entre 1 e o tamanho do mapa, inclusive)
É dado o feedback do ataque e, de seguida, mostrado o seu mapa de ataques, com os caracteres '.', 'M', 'I', 'P', 'T', 'X' ou 'Z',
signficando "Sem ataque", "Ataque falhado" ,"Ataque a uma peça do tipo I", "Ataque a uma peça do tipo P", "Ataque a uma peça do tipo T", "Ataque a uma peça do tipo X", "Ataque a uma peça do tipo Z", respetivamente.
O processo agora repete-se, mas para o outro player e, assim, sucessivamente, até um dos players ficar com as peças todas destruídas.

Quando um player fica com as peças todas destruídas, o jogo acaba e o sistema indica o player vencedor.
Por fim, é pedido se os players desejam jogar novamente, ou não.
Caso sim, todo este processo descrito repete-se, caso não, a execução do jogo termina.

############### Módulos ###################

bitmap.h
Definição do bitmap.
Existem alguns formatos disponíveis de bitmaps (I,P,T,X,Z) (que vão corresponder ao tipo de peças)
Estes formatos são guardados comprimidos, para gastar menos memória.
Ao atualizar o formato, precisamos ainda de especificar a rotação do mesmo.

piece.h
Definição da peça.
A peça tem um char para representar o seu tipo (I,P,T,X,Z), dois int's posX, posY e um bitmap para representar o estado ("sem peça", "peça não destruida", "peça destruida")
O centro do bitmap da peça está no mapa na posição (posX, posY).

cell.h
Definição da cell.
A cell tem um pointer para uma peça e um byte shot.
Se o pointer == NULL, então a cell não tem peça. Se o pointer != NULL, então tem peça.
A variável shot é respetiva aos ataques do player e tem valores entre 0 e 6.

map.h
Definição do mapa.
Tem um size e as cells.

player.h
Definição do player.
Tem um int para representar o seu hp (ie, numero de cells que têm peça) e um mapa.

game.h
Definição do jogo.
O jogo tem dois players e um int para saber que player está a atacar.

io.h
Toda a atividade de IO é aqui realizada, através de uma função.

utils.h
Utilitários.

quadtree.h
Definição da quadtree.
A ideia da implementação é que divido o espaço 2D em 4 regiões TL TR BL BR, recursivamente.
Se a região contém, pelo menos, um nó, ao adicionar um novo, adiciono na nova subdivisão do espaço.
Caso não tenha, guardo no atual, não é feita nenhuma subdivisão.

point.h
Representação de um ponto 2D.
