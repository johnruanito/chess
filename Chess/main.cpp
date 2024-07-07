#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

const int BOARD_SIZE = 8;
const int WINDOW_WIDTH = 602;
const int WINDOW_HEIGHT = 595;
const int SQUARE_SIZE = 75;

typedef char ChessBoard[BOARD_SIZE][BOARD_SIZE];


// Declara��o e defini��o da vari�vel board
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

// Fun��o para mapear o deslocamento da sprite para cada pe�a
void initializePieceOffsetMap() {
    pieceOffsetMap['k'] = sf::Vector2i(0, 0); // Rei branco
    pieceOffsetMap['q'] = sf::Vector2i(1, 0); // Rainha branco
    pieceOffsetMap['b'] = sf::Vector2i(2, 0); // Bispo branco
    pieceOffsetMap['n'] = sf::Vector2i(3, 0); // Cavalo branco
    pieceOffsetMap['r'] = sf::Vector2i(4, 0); // Torre branco
    pieceOffsetMap['p'] = sf::Vector2i(5, 0); // Pe�o branco

    pieceOffsetMap['K'] = sf::Vector2i(0, 1); // Rei preto
    pieceOffsetMap['Q'] = sf::Vector2i(1, 1); // Rainha preta
    pieceOffsetMap['B'] = sf::Vector2i(2, 1); // Bispo preto
    pieceOffsetMap['N'] = sf::Vector2i(3, 1); // Cavalo preto
    pieceOffsetMap['R'] = sf::Vector2i(4, 1); // Torre preta
    pieceOffsetMap['P'] = sf::Vector2i(5, 1); // Pe�o preto
}

// Fun��es auxiliares para verificar o tipo de pe�a
//Black
bool isBlackPiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}
//Whiter
bool isWhitePiece(char piece) {
    return piece >= 'a' && piece <= 'z';
}
// Fun��o para verificar se o caminho est� livre
bool isPathClear(int startRow, int startCol, int destRow, int destCol) {
    // Verificar se o caminho est� livre na horizontal (mesma linha)
    if (startRow == destRow) {
        int step = (destCol > startCol) ? 1 : -1;
        for (int col = startCol + step; col != destCol; col += step) {
            if (board[startRow][col] != ' ') {
                // H� uma pe�a no caminho
                return false;
            }
        }
        return true;
    }

    // Verificar se o caminho est� livre na vertical (mesma coluna)
    if (startCol == destCol) {
        int step = (destRow > startRow) ? 1 : -1;
        for (int row = startRow + step; row != destRow; row += step) {
            if (board[row][startCol] != ' ') {
                // H� uma pe�a no caminho
                return false;
            }
        }
        return true;
    }

    // Verificar se o caminho est� livre na diagonal
    int rowStep = (destRow > startRow) ? 1 : -1;
    int colStep = (destCol > startCol) ? 1 : -1;
    int row = startRow + rowStep;
    int col = startCol + colStep;
    while (row != destRow && col != destCol) {
        if (board[row][col] != ' ') {
            // H� uma pe�a no caminho
            return false;
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}
// Fun��o para validar movimentos do pe�o
bool isValidPawnMove(int startRow, int startCol, int destRow, int destCol) {
    // L�gica de movimenta��o do pe�o 
    if (board[startRow][startCol] == 'p') {
        if (destRow == startRow - 1 && destCol == startCol && board[destRow][destCol] == ' ') {
            // Avan�ar uma casa
            return true;
        }
        else if (startRow == 6 && destRow == startRow - 2 && destCol == startCol && board[destRow][destCol] == ' ' &&
            board[startRow - 1][startCol] == ' ') {
            // Avan�ar duas casas na primeira jogada
            return true;
        }
        else if (destRow == startRow - 1 && (destCol == startCol - 1 || destCol == startCol + 1) &&
            isBlackPiece(board[destRow][destCol])) {
            // Capturar diagonalmente
            return true;
        }
    }
    else if (board[startRow][startCol] == 'P') {  // Pe�o preto
        if (destRow == startRow + 1 && destCol == startCol && board[destRow][destCol] == ' ') {
            // Avan�ar uma casa
            return true;
        }
        else if (startRow == 1 && destRow == startRow + 2 && destCol == startCol && board[destRow][destCol] == ' ' &&
            board[startRow + 1][startCol] == ' ') {
            // Avan�ar duas casas na primeira jogada
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
// Fun��o para validar movimentos de cada tipo de pe�a
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
        // Pe�o
        return isValidPawnMove(startRow, startCol, destRow, destCol) &&
            (board[destRow][destCol] == ' ' || (isBlackPiece(piece) && isWhitePiece(board[destRow][destCol])) ||
                (isWhitePiece(piece) && isBlackPiece(board[destRow][destCol])));

    default:
        return false;
    }
}


 //Fun��o para verificar se um rei est� em xeque
bool isCheck(bool isWhiteTurn) {
    // Encontrar a posi��o do rei do jogador atual
    int kingRow = -1, kingCol = -1;
    char kingPiece = (isWhiteTurn) ? 'K' : 'k';

    // Procurar a posi��o do rei
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

    // Verificar se o rei est� sob ataque
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            char piece = board[i][j];
            if ((isWhiteTurn && piece >= 'a' && piece <= 'z') ||  // Pe�a preta
                (!isWhiteTurn && piece >= 'A' && piece <= 'Z')) {  // Pe�a branca
                // Verificar se essa pe�a pode atacar o rei
                if (isValidMove(i, j, kingRow, kingCol) &&
                    isPathClear(i, j, kingRow, kingCol)) {  // Verificar se o caminho est� livre
                    return true;  // O rei est� em xeque
                }
            }
        }
    }

    return false;  // O rei n�o est� em xeque
}



// L�gica para verificar se o rei est� em xeque-mate
// Function to check if a player is in checkmate


// L�gica para verificar se h� empate por afogamento
//bool isStalemate(bool isWhiteTurn) {

    // Retorna true se houver afogamento, false caso contr�rio
//}
// L�gica para verificar se h� tr�plice repeti��o
//bool isThreefoldRepetition() {

    // Retorna true se houver repeti��o, false caso contr�rio
//}
// L�gica para verificar se a regra dos cinquenta lances se aplica
//bool isFiftyMoveRule() {

    // Retorna true se houver empate por cinquenta lances, false caso contr�rio
//}

// Fun��o para realizar o movimento no tabuleiro

void handleMove(int destRow, int destCol, bool isWhiteTurn) {
    // Simular o movimento no tabuleiro
    
    char originalPiece = board[selectedPiecePos.y][selectedPiecePos.x];
    char originalDestPiece = board[destRow][destCol];
    // Verificar se h� uma pe�a selecionada
    
        
    if (selectedPiecePos.x != -1 && selectedPiecePos.y != -1) {
        
            // Verificar se o movimento � v�lido e se o caminho est� livre
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
        



    // Redefinir a posi��o da pe�a selecionada
    selectedPiecePos = sf::Vector2i(-1, -1);
}



int main() {
    // Configura��o da janela
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "chess", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    // Carregamento de texturas
    sf::Texture pieceTexture, boardTexture;
    if (!pieceTexture.loadFromFile("sprite/chessfigure.png") || !boardTexture.loadFromFile("sprite/chessTab.jpg")) {
        std::cerr << "Erro ao carregar texturas." << std::endl;
        return 1;
    }
    // Cria��o do sprite do tabuleiro
    sf::Sprite boardSprite(boardTexture);

    // Inicializa��o do mapeamento de deslocamento das pe�as
    initializePieceOffsetMap();

    // Posi��es iniciais das pe�as
    sf::Vector2f piecePositions[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            piecePositions[i][j] = sf::Vector2f(j * SQUARE_SIZE, i * SQUARE_SIZE);
        }
    }

    // Vari�vel para controle de turno
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

                        // Verificar se a pe�a que est� sendo movida pertence ao jogador atual
                        if ((isWhiteTurn && isWhitePiece(board[selectedPiecePos.y][selectedPiecePos.x])) ||
                            (!isWhiteTurn && isBlackPiece(board[selectedPiecePos.y][selectedPiecePos.x]))) {
                            // Pe�a v�lida para movimento
                        }
                        else {
                            selectedPiecePos = sf::Vector2i(-1, -1);  // Resetar se a pe�a n�o pertencer ao jogador atual
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

        // Desenho das pe�as no tabuleiro
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != ' ') {
                    sf::Sprite pieceSprite(pieceTexture);
                    sf::Vector2i pieceOffset = pieceOffsetMap[board[i][j]];
                    int pieceX = pieceOffset.x * SQUARE_SIZE;
                    int pieceY = pieceOffset.y * SQUARE_SIZE;
                    pieceSprite.setTextureRect(sf::IntRect(pieceX, pieceY, SQUARE_SIZE, SQUARE_SIZE));
                    pieceSprite.setPosition(piecePositions[i][j]);

                    // Atualiza��o da posi��o da pe�a se estiver sendo arrastada
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