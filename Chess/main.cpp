#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

const int BOARD_SIZE = 8;
const int WINDOW_WIDTH = 602;
const int WINDOW_HEIGHT = 595;
const int SQUARE_SIZE = 75;

typedef char ChessBoard[BOARD_SIZE][BOARD_SIZE];


// Declaração e definição da variável board
ChessBoard board = {
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
};

std::map<char, sf::Vector2i> pieceOffsetMap;
sf::Vector2i selectedPiecePos(-1, -1);
sf::Vector2f offset;

// Função para mapear o deslocamento da sprite para cada peça
void initializePieceOffsetMap() {
    pieceOffsetMap['k'] = sf::Vector2i(0, 0); // Rei branco
    pieceOffsetMap['q'] = sf::Vector2i(1, 0); // Rainha branco
    pieceOffsetMap['b'] = sf::Vector2i(2, 0); // Bispo branco
    pieceOffsetMap['n'] = sf::Vector2i(3, 0); // Cavalo branco
    pieceOffsetMap['r'] = sf::Vector2i(4, 0); // Torre branco
    pieceOffsetMap['p'] = sf::Vector2i(5, 0); // Peão branco

    pieceOffsetMap['K'] = sf::Vector2i(0, 1); // Rei preto
    pieceOffsetMap['Q'] = sf::Vector2i(1, 1); // Rainha preta
    pieceOffsetMap['B'] = sf::Vector2i(2, 1); // Bispo preto
    pieceOffsetMap['N'] = sf::Vector2i(3, 1); // Cavalo preto
    pieceOffsetMap['R'] = sf::Vector2i(4, 1); // Torre preta
    pieceOffsetMap['P'] = sf::Vector2i(5, 1); // Peão preto
}

// Funções auxiliares para verificar o tipo de peça
//Black
bool isBlackPiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}
//Whiter
bool isWhitePiece(char piece) {
    return piece >= 'a' && piece <= 'z';
}
// Função para verificar se o caminho está livre
bool isPathClear(int startRow, int startCol, int destRow, int destCol) {
    // Verificar se o caminho está livre na horizontal (mesma linha)
    if (startRow == destRow) {
        int step = (destCol > startCol) ? 1 : -1;
        for (int col = startCol + step; col != destCol; col += step) {
            if (board[startRow][col] != ' ') {
                // Há uma peça no caminho
                return false;
            }
        }
        return true;
    }

    // Verificar se o caminho está livre na vertical (mesma coluna)
    if (startCol == destCol) {
        int step = (destRow > startRow) ? 1 : -1;
        for (int row = startRow + step; row != destRow; row += step) {
            if (board[row][startCol] != ' ') {
                // Há uma peça no caminho
                return false;
            }
        }
        return true;
    }

    // Verificar se o caminho está livre na diagonal
    int rowStep = (destRow > startRow) ? 1 : -1;
    int colStep = (destCol > startCol) ? 1 : -1;
    int row = startRow + rowStep;
    int col = startCol + colStep;
    while (row != destRow && col != destCol) {
        if (board[row][col] != ' ') {
            // Há uma peça no caminho
            return false;
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}
// Função para validar movimentos do peão
bool isValidPawnMove(int startRow, int startCol, int destRow, int destCol) {
    // Lógica de movimentação do peão 
    if (board[startRow][startCol] == 'p') {
        if (destRow == startRow - 1 && destCol == startCol && board[destRow][destCol] == ' ') {
            // Avançar uma casa
            return true;
        }
        else if (startRow == 6 && destRow == startRow - 2 && destCol == startCol && board[destRow][destCol] == ' ' &&
            board[startRow - 1][startCol] == ' ') {
            // Avançar duas casas na primeira jogada
            return true;
        }
        else if (destRow == startRow - 1 && (destCol == startCol - 1 || destCol == startCol + 1) &&
            isBlackPiece(board[destRow][destCol])) {
            // Capturar diagonalmente
            return true;
        }
    }
    else if (board[startRow][startCol] == 'P') {  // Peão preto
        if (destRow == startRow + 1 && destCol == startCol && board[destRow][destCol] == ' ') {
            // Avançar uma casa
            return true;
        }
        else if (startRow == 1 && destRow == startRow + 2 && destCol == startCol && board[destRow][destCol] == ' ' &&
            board[startRow + 1][startCol] == ' ') {
            // Avançar duas casas na primeira jogada
            return true;
        }
        else if (destRow == startRow + 1 && (destCol == startCol - 1 || destCol == startCol + 1) &&
            isWhitePiece(board[destRow][destCol])) {
            // Capturar diagonalmente
            return true;
        }
    }

    return false;
}
// Função para validar movimentos de cada tipo de peça
bool isValidMove(int startRow, int startCol, int destRow, int destCol) {
    char piece = board[startRow][startCol];

    switch (piece) {
    case 'R':
    case 'r':
        // Torre
        return (startRow == destRow || startCol == destCol) &&
            isPathClear(startRow, startCol, destRow, destCol) &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    case 'n':
        // Cavalo branco
        return ((abs(destRow - startRow) == 2 && abs(destCol - startCol) == 1) ||
            (abs(destRow - startRow) == 1 && abs(destCol - startCol) == 2)) &&
            (board[destRow][destCol] == ' ' || isBlackPiece(board[destRow][destCol]));

    case 'N':
        // Cavalo preto
        return ((abs(destRow - startRow) == 2 && abs(destCol - startCol) == 1) ||
            (abs(destRow - startRow) == 1 && abs(destCol - startCol) == 2)) &&
            (board[destRow][destCol] == ' ' || isWhitePiece(board[destRow][destCol]));

    case 'B':
    case 'b':
        // Bispo
        return abs(destRow - startRow) == abs(destCol - startCol) &&
            isPathClear(startRow, startCol, destRow, destCol) &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    case 'K':
    case 'k':
        // Rei
        return abs(destRow - startRow) <= 1 && abs(destCol - startCol) <= 1 &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    case 'Q':
    case 'q':
        // Rainha
        return ((startRow == destRow || startCol == destCol) ||
            (abs(destRow - startRow) == abs(destCol - startCol))) &&
            isPathClear(startRow, startCol, destRow, destCol) &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    case 'P':
    case 'p':
        // Peão
        return isValidPawnMove(startRow, startCol, destRow, destCol) &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    default:
        return false;
    }
}


 //Função para verificar se um rei está em xeque
bool isCheck(bool isWhiteTurn) {
    // Encontrar a posição do rei do jogador atual
    int kingRow = -1, kingCol = -1;
    char kingPiece = (isWhiteTurn) ? 'K' : 'k';

    // Procurar a posição do rei
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == kingPiece) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) {
            break;
        }
    }

    // Verificar se o rei está sob ataque
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            char piece = board[i][j];
            if ((isWhiteTurn && piece >= 'a' && piece <= 'z') ||  // Peça preta
                (!isWhiteTurn && piece >= 'A' && piece <= 'Z')) {  // Peça branca
                // Verificar se essa peça pode atacar o rei
                if (isValidMove(i, j, kingRow, kingCol) &&
                    isPathClear(i, j, kingRow, kingCol)) {  // Verificar se o caminho está livre
                    return true;  // O rei está em xeque
                }
            }
        }
    }

    return false;  // O rei não está em xeque
}



// Lógica para verificar se o rei está em xeque-mate
// Function to check if a player is in checkmate


// Lógica para verificar se há empate por afogamento
//bool isStalemate(bool isWhiteTurn) {

    // Retorna true se houver afogamento, false caso contrário
//}
// Lógica para verificar se há tríplice repetição
//bool isThreefoldRepetition() {

    // Retorna true se houver repetição, false caso contrário
//}
// Lógica para verificar se a regra dos cinquenta lances se aplica
//bool isFiftyMoveRule() {

    // Retorna true se houver empate por cinquenta lances, false caso contrário
//}

// Função para realizar o movimento no tabuleiro

void handleMove(int destRow, int destCol, bool isWhiteTurn) {
    // Simular o movimento no tabuleiro
    
    char originalPiece = board[selectedPiecePos.y][selectedPiecePos.x];
    char originalDestPiece = board[destRow][destCol];
    // Verificar se há uma peça selecionada
    
        
    if (selectedPiecePos.x != -1 && selectedPiecePos.y != -1) {
        
            // Verificar se o movimento é válido e se o caminho está livre
            if (isValidMove(selectedPiecePos.y, selectedPiecePos.x, destRow, destCol) &&
                isPathClear(selectedPiecePos.y, selectedPiecePos.x, destRow, destCol)) {
                board[destRow][destCol] = board[selectedPiecePos.y][selectedPiecePos.x];
                board[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                

            }
           
            if (isCheck(!isWhiteTurn)) {
                board[selectedPiecePos.y][selectedPiecePos.x] = originalPiece;
                board[destRow][destCol] = originalDestPiece;
           }
        }
        



    // Redefinir a posição da peça selecionada
    selectedPiecePos = sf::Vector2i(-1, -1);
}



int main() {
    // Configuração da janela
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "chess", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    // Carregamento de texturas
    sf::Texture pieceTexture, boardTexture;
    if (!pieceTexture.loadFromFile("sprite/chessfigure.png") || !boardTexture.loadFromFile("sprite/chessTab.jpg")) {
        std::cerr << "Erro ao carregar texturas." << std::endl;
        return 1;
    }
    // Criação do sprite do tabuleiro
    sf::Sprite boardSprite(boardTexture);

    // Inicialização do mapeamento de deslocamento das peças
    initializePieceOffsetMap();

    // Posições iniciais das peças
    sf::Vector2f piecePositions[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            piecePositions[i][j] = sf::Vector2f(j * SQUARE_SIZE, i * SQUARE_SIZE);
        }
    }

    // Variável para controle de turno
    bool isWhiteTurn = true;

    // Loop principal do jogo
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / SQUARE_SIZE;
                int row = mousePos.y / SQUARE_SIZE;

                if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE) {
                    if (board[row][col] != ' ') {
                        selectedPiecePos = sf::Vector2i(col, row);
                        offset = piecePositions[row][col] - sf::Vector2f(mousePos);

                        // Verificar se a peça que está sendo movida pertence ao jogador atual
                        if ((isWhiteTurn && isWhitePiece(board[selectedPiecePos.y][selectedPiecePos.x])) ||
                            (!isWhiteTurn && isBlackPiece(board[selectedPiecePos.y][selectedPiecePos.x]))) {
                            // Peça válida para movimento
                        }
                        else {
                            selectedPiecePos = sf::Vector2i(-1, -1);  // Resetar se a peça não pertencer ao jogador atual
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (selectedPiecePos.x != -1 && selectedPiecePos.y != -1) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int destCol = mousePos.x / SQUARE_SIZE;
                    int destRow = mousePos.y / SQUARE_SIZE;

                    if (destCol >= 0 && destCol < BOARD_SIZE && destRow >= 0 && destRow < BOARD_SIZE) {
                        if (isValidMove(selectedPiecePos.y, selectedPiecePos.x, destRow, destCol) &&
                            isPathClear(selectedPiecePos.y, selectedPiecePos.x, destRow, destCol) &&
                            !isCheck(isWhiteTurn)) {
                            handleMove(destRow, destCol, isWhiteTurn);
                            
                            
                            if(!isCheck(!isWhiteTurn)){
                            isWhiteTurn = !isWhiteTurn;
                            }
                              
                            
                            
                        }
                    }

                    selectedPiecePos = sf::Vector2i(-1, -1);
                }
            }
        }



        window.clear();
        window.draw(boardSprite);

        // Desenho das peças no tabuleiro
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != ' ') {
                    sf::Sprite pieceSprite(pieceTexture);
                    sf::Vector2i pieceOffset = pieceOffsetMap[board[i][j]];
                    int pieceX = pieceOffset.x * SQUARE_SIZE;
                    int pieceY = pieceOffset.y * SQUARE_SIZE;
                    pieceSprite.setTextureRect(sf::IntRect(pieceX, pieceY, SQUARE_SIZE, SQUARE_SIZE));
                    pieceSprite.setPosition(piecePositions[i][j]);

                    // Atualização da posição da peça se estiver sendo arrastada
                    if (selectedPiecePos.x == j && selectedPiecePos.y == i) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        pieceSprite.setPosition(static_cast<sf::Vector2f>(mousePos) + offset);
                    }

                    window.draw(pieceSprite);
                }
            }
        }



        window.display();






    }

    return 0;
}