#include <iostream>
#include "Authenticator.h"
#include "StoryEngine.h"

using namespace std;

int main(int argc, char* argv[]) {
    UserData user;
    user.playerState = new PlayerState();

    // Parse command-line arguments (create or login)
    if (!processArguments(argc, argv, user)) {
        delete user.playerState;
        return 1;
    }

    bool success = false;

    if (user.isNewUser) {
        success = createUser(user);
    }
    else {
        success = loginUser(user);
    }

    if (!success) {
        cout << "\nAuthentication failed! Press try again";
        delete user.playerState;
        return 1;
    }

    showGameWindow(user);

    delete user.playerState;
    return 0;
}
