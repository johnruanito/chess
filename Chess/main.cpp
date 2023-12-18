#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

const int tamanhoTabuleiro = 8;
const int tamanhoQuadrado = 75;
const int larguraJanela = 602;
const int alturaJanela = 595;

char tabuleiro[tamanhoTabuleiro][tamanhoTabuleiro] = {
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

void initializePieceOffsetMap() {
    // Mapear o deslocamento (offset) da sprite para cada peça
    pieceOffsetMap['k'] = sf::Vector2i(0, 0); // Rei
    pieceOffsetMap['q'] = sf::Vector2i(1, 0); // Rainha
    pieceOffsetMap['b'] = sf::Vector2i(2, 0); // Bispo
    pieceOffsetMap['n'] = sf::Vector2i(3, 0); // Cavalo
    pieceOffsetMap['r'] = sf::Vector2i(4, 0); // Torre
    pieceOffsetMap['p'] = sf::Vector2i(5, 0); // Peão

    pieceOffsetMap['K'] = sf::Vector2i(0, 1); // Rei preto
    pieceOffsetMap['Q'] = sf::Vector2i(1, 1); // Rainha preta
    pieceOffsetMap['B'] = sf::Vector2i(2, 1); // Bispo preto
    pieceOffsetMap['N'] = sf::Vector2i(3, 1); // Cavalo preto
    pieceOffsetMap['R'] = sf::Vector2i(4, 1); // Torre preta
    pieceOffsetMap['P'] = sf::Vector2i(5, 1); // Peão preto
}

bool isBlackPiece(char piece) {
    return piece >= 'A' && piece <= 'Z';
}

bool isWhitePiece(char piece) {
    return piece >= 'a' && piece <= 'z';
}

bool isPathClear(int startRow, int startCol, int destRow, int destCol) {
    // Verificar se o caminho está livre na horizontal (mesma linha)
    if (startRow == destRow) {
        int step = (destCol > startCol) ? 1 : -1;
        for (int col = startCol + step; col != destCol; col += step) {
            if (tabuleiro[startRow][col] != ' ') {
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
            if (tabuleiro[row][startCol] != ' ') {
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
        if (tabuleiro[row][col] != ' ') {
            // Há uma peça no caminho
            return false;
        }
        row += rowStep;
        col += colStep;
    }

    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(larguraJanela, alturaJanela), "chess", sf::Style::Titlebar | sf::Style::Close);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    sf::Texture pieceTexture, boardTexture;

    if (!pieceTexture.loadFromFile("sprite/chessfigure.png") || !boardTexture.loadFromFile("sprite/chessTab.jpg")) {
        std::cerr << "Erro ao carregar texturas." << std::endl;
        return 1;
    }

    sf::Sprite boardSprite(boardTexture);

    initializePieceOffsetMap();

    sf::Vector2f piecePositions[tamanhoTabuleiro][tamanhoTabuleiro];
    for (int i = 0; i < tamanhoTabuleiro; ++i) {
        for (int j = 0; j < tamanhoTabuleiro; ++j) {
            piecePositions[i][j] = sf::Vector2f(j * tamanhoQuadrado, i * tamanhoQuadrado);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / tamanhoQuadrado;
                int row = mousePos.y / tamanhoQuadrado;

                if (col >= 0 && col < tamanhoTabuleiro && row >= 0 && row < tamanhoTabuleiro) {
                    if (tabuleiro[row][col] != ' ') {
                        selectedPiecePos = sf::Vector2i(col, row);
                        offset = piecePositions[row][col] - sf::Vector2f(mousePos);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (selectedPiecePos.x != -1 && selectedPiecePos.y != -1) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int destCol = mousePos.x / tamanhoQuadrado;
                    int destRow = mousePos.y / tamanhoQuadrado;

                    if (destCol >= 0 && destCol < tamanhoTabuleiro && destRow >= 0 && destRow < tamanhoTabuleiro) {
                        // Lógica de movimentação do peão 
                        if (tabuleiro[selectedPiecePos.y][selectedPiecePos.x] == 'p') {
                            if (destRow == selectedPiecePos.y - 1 && destCol == selectedPiecePos.x && tabuleiro[destRow][destCol] == ' ') {
                                // Avançar uma casa
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }
                            else if (selectedPiecePos.y == 6 && destRow == selectedPiecePos.y - 2 && destCol == selectedPiecePos.x && tabuleiro[destRow][destCol] == ' ') {
                                // Avançar duas casas na primeira jogada
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }
                            else if (destRow == selectedPiecePos.y - 1 && (destCol == selectedPiecePos.x - 1 || destCol == selectedPiecePos.x + 1) && isBlackPiece(tabuleiro[destRow][destCol])) {
                                // Capturar diagonalmente
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }
                        }
                        else if (tabuleiro[selectedPiecePos.y][selectedPiecePos.x] == 'P') {  // Peão preto
                            if (destRow == selectedPiecePos.y + 1 && destCol == selectedPiecePos.x && tabuleiro[destRow][destCol] == ' ') {
                                // Avançar uma casa
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }
                            else if (selectedPiecePos.y == 1 && destRow == selectedPiecePos.y + 2 && destCol == selectedPiecePos.x && tabuleiro[destRow][destCol] == ' ') {
                                // Avançar duas casas na primeira jogada
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }
                            else if (destRow == selectedPiecePos.y + 1 && (destCol == selectedPiecePos.x - 1 || destCol == selectedPiecePos.x + 1) && isWhitePiece(tabuleiro[destRow][destCol])) {
                                // Capturar diagonalmente
                                tabuleiro[destRow][destCol] = tabuleiro[selectedPiecePos.y][selectedPiecePos.x];
                                tabuleiro[selectedPiecePos.y][selectedPiecePos.x] = ' ';
                            }

                        }

                    }

                    selectedPiecePos = sf::Vector2i(-1, -1);
                }
            }
        }

        window.clear();
        window.draw(boardSprite);

        for (int i = 0; i < tamanhoTabuleiro; ++i) {
            for (int j = 0; j < tamanhoTabuleiro; ++j) {
                if (tabuleiro[i][j] != ' ') {
                    sf::Sprite pieceSprite(pieceTexture);
                    sf::Vector2i pieceOffset = pieceOffsetMap[tabuleiro[i][j]];
                    int pieceX = pieceOffset.x * tamanhoQuadrado;
                    int pieceY = pieceOffset.y * tamanhoQuadrado;
                    pieceSprite.setTextureRect(sf::IntRect(pieceX, pieceY, tamanhoQuadrado, tamanhoQuadrado));
                    pieceSprite.setPosition(piecePositions[i][j]);

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
