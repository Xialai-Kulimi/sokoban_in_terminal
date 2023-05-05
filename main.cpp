#include "Screen.h"

int main(){
    Screen screen;
    screen.init_menu(
        "Example title", 
        "Example description, this is an example description. Hope this sentence is long enough.",
        "Please pick an option."
    );
    screen.add_option("option1");
    screen.add_option("not an option");
    screen.add_option("not very good option");
    screen.render();
    screen.get_key();
    screen.set_align("center");
    screen.get_key();
}