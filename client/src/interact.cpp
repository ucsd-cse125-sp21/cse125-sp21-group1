#include "interact.h"
// send user logged in

// send user name to server




int main_ui_loop(int player_num) {
  std::string msg;
  while (1) {
    draw_main_board();

	// need message from server
	// 0 or 1
	msg = Networking::receive();
    if (!msg.compare("1")) { // equals one, finish loop
	  main_game_loop();
	}
	msg = "0";
  }


}

void main_game_loop(int player_num) {
  std::string msg;
  while (1) {
    draw_game_board();

	msg = Networking::receive();
	// send message to graphics

	if (!msg.compare("stop game")) {// stop game
	  return;
	}
  }
}


/*
 * key:
 * u: move upper
 * o: move lower
 * l: move left
 * r: move right
 * s: space
 * player_num: which player it is
 */
void keyboard_input_callback(char key, int player_num){
  std::string msg = "Player ";
  switch(key) {
    case 'u':
	  msg += std::to_string(player_num);
	  msg += " wants to move upward.";
	  Networking::send(msg);
	  break;
    case 'o':
	  msg += std::to_string(player_num);
	  msg += " wants to move downward.";
	  Networking::send(msg);
	  break;
	case 'l':
	  msg += std::to_string(player_num);
	  msg += " wants to move leftward.";
	  Networking::send(msg);
	  break;
	case 'r':
	  msg += std::to_string(player_num);
	  msg += " wants to move rightward.";
	  Networking::send(msg);
	  break;
	default:
	  msg += std::to_string(player_num);
	  msg += " places a bomb.";
	  Networking::send(msg);
  }

  // TODO
  msg = Networking::receive();
}
