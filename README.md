# Chess

Project: Chess in C++ with SFML

This project is a simple implementation of a chess game in C++ using the SFML graphics library. The code is contained in the file chess.cpp, and some key features are highlighted below.

Key Features:
Chess Board:

Utilizes an 8x8 ChessBoard matrix to represent the state of the chessboard.
Each chess piece is represented by a character on the board.
Piece Mapping:

A pieceOffsetMap mapping is used to associate each type of piece with a specific position in the texture containing the images of the chess pieces.
Piece Movement:

The isValidMove function checks if a piece's movement is valid, taking into account specific rules for each type of chess piece.
Turn Logic:

The game implements turn logic to switch between players.
Mouse Interaction:

Pieces can be selected and moved using the mouse.
Texture Loading:

The code loads textures for the pieces and the board, contributing to the visual representation of the game.
How to Run:
Clone the repository.
Ensure you have the SFML library installed in your development environment.
Compile and execute the file using the appropriate build commands.
Controls:
Click on a piece to select it.
Move the mouse to the desired position and click again to release the piece.
Movement rules follow standard chess rules.
Notes:
The project uses images to represent the pieces and the board. Make sure to have the "sprite" folder in the same directory as the executable, containing the necessary images.
The game logic is implemented for a single player, allowing pieces to move according to standard chess rules.
It is a basic implementation and can be expanded with additional features such as checkmate logic, special moves (castling, en passant), etc.
Enjoy playing chess!
