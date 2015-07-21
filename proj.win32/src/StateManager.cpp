#include "StateManager.h"
#include <sstream>
#include "debug/Errors.h"
#include "network/MessageRecv.h"
#include "network/SocketManager.h"

using state::State;

#define SSTR(x) dynamic_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str()

SceneManager* state::scene;

State state::s = state::TCP_SERVER_CONNECT;
cc::LabelBMFont* state::label;
float state::time_since_startup = 0;

void create_state(State c_state) {
    using namespace state;

    s = c_state;
    switch (s) {
        case TCP_SERVER_CONNECT:
            label = cc::LabelBMFont::create("", "fonts/lucida.fnt");
            label->setString("connecting...");
            scene->addChild(label, 1);

            scene->scheduleUpdate();

            sock::setup_tcp_sock();
            break;
    }
}

void remove_state(State r_state) {
    using namespace state;

    s = r_state;
    switch (s) {
        case TCP_SERVER_CONNECT:
            break;
    }
}

void state::switch_state(State new_state) {
    remove_state(s);
    create_state(new_state);
}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;

    sock::init();

    create_state(s);
}

void state::update(float dt) {
    switch (s) {
        case TCP_SERVER_CONNECT:
            label->setString("connecting to server");
            if (sock::done_connecting) {
                if (sock::connect_result == NO_ERROR) {
                    label->setString("connected!");
                    sock::done_connecting = false;
                }else {
                    label->setString("an error occurred while trying to connect: " + SSTR(sock::connect_result));
                }
            }

            time_since_startup += dt;
            label->setPosition(cc::Vec2((cos(time_since_startup) * 40.0f) + 400, 200));
            break;
        case UDP_SERVER_CONNECT:
            label->setString("verifying udp connection...");
            if (sock::done_connecting) {
                if (sock::connect_result == NO_ERROR) {
                    label->setString("connected!");
                    sock::done_connecting = false;
                }else {
                    label->setString("an error occurred while trying to verify udp connection: " + SSTR(sock::connect_result));
                }
            }

            time_since_startup += dt;
            label->setPosition(cc::Vec2((cos(time_since_startup) * 40.0f) + 400, 200));
            break;
    }
}