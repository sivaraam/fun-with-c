## Chess GUI

A GTK+ GUI for playing chess. The GUI is based on GTK+ 3.0.

### Dependencies
This application depends on the [Chess move validate library](../../chess_move_validate/).
So, the shared library file for it must be generated for the build to succeed.

### Build
As the GUI is based on GTK+-3.0, the library is required for building the
GUI.

Typically to install the GTK+3.0 library, `sudo apt-get install libgtk-3-dev`.

To build the GUI, `make`.

### Usage
* The users can move the chess coins by means of drag and drop. Only valid moves
  are allowed and invalid moves have no effect.
* The White coin has the first turn.
* Only moves supported by the [Chess move validation library](../../chess_move_validate)
  used by the GUI are supported.
