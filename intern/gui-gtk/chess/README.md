## Chess GUI

A GTK+ GUI for playing chess. The GUI is based on GTK+ 3.0.

### Build
As the GUI is based on GTK+-3.0, the library is required for the building the
GUI.

Typically to install the GTK+3.0 library, `sudo apt-get install libgtk-3-dev`

To build the GUI, `make`.

### Usage
* The users can move the chess coins by means of drag and drop. Only valid moves
  are allowed and invalid moves have no effect.
* The White coin has the first turn.
* Only moves supported by the [Chess move validation library](../../chess_move_validate)
  used by the GUI are supported.
