#include "packages/ev3/ev3dev/ev3control.capnp.h"
#include <capnp/ez-rpc.h>
#include <capnp/message.h>
#include <iostream>
#include "ev3dev.h"
#include <thread>
#include <chrono>

const uint MAX_SPEED = 900;
const float TACHO_TO_SPEED = 0.25;
const float BASE_LENGHT = 0.40;
ev3dev::large_motor l_motor(ev3dev::OUTPUT_B);
ev3dev::large_motor r_motor(ev3dev::OUTPUT_C);

class Ev3ControlServer final : public Ev3Control::Server
{
public:    
    
    ::kj::Promise<void> command(CommandContext context) override
    {
        auto cmd = context.getParams().getCmd();
        // std::cout << "command " << cmd.getLinearSpeed() << " " << cmd.getAngularSpeed() << std::endl;

        float speed_diff = cmd.getAngularSpeed() * BASE_LENGHT;

        l_motor.set_speed_sp(MAX_SPEED * (cmd.getLinearSpeed() - speed_diff/2)).run_forever();  // tacho counts per second
        r_motor.set_speed_sp(MAX_SPEED * (cmd.getLinearSpeed() + speed_diff/2)).run_forever();  // tacho counts per second
        
        return kj::READY_NOW;
    }

    ::kj::Promise<void> state(StateContext context) override {
        ::capnp::MallocMessageBuilder message;

        Dynamics::Builder state = message.initRoot<Dynamics>();

        auto start = std::chrono::high_resolution_clock::now();
        int l_position_start = l_motor.position();
        int r_position_start = r_motor.position();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int l_position_end = l_motor.position();
        int r_position_end = r_motor.position();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> elapsed = end-start;

        float l_speed = (l_position_end - l_position_start)/elapsed.count();
        float r_speed = (r_position_end - r_position_start)/elapsed.count();

        state.setLinearSpeed(TACHO_TO_SPEED*(l_speed+r_speed)/2);
        state.setAngularSpeed(TACHO_TO_SPEED*(r_speed-l_speed)/BASE_LENGHT);
        
        state.setLinearAcceleration(0.0);
        state.setAngularAcceleration(0.0);
        context.getResults().setState(state);

        if(state.getLinearSpeed() || state.getAngularSpeed()) {
            std::cout << "state " << state.getLinearSpeed() << " " << state.getAngularSpeed() << std::endl;
        }

        return kj::READY_NOW;
    }
};

//---------------------------------------------------------------------------
void precondition(bool cond, const std::string &msg) {
    if (!cond) throw std::runtime_error(msg);
}

int main(int argc, const char *argv[])
{
    // We expect one argument specifying the address to which
    // to bind and accept connections.
    if (argc != 2)
    {
        std::cerr << "usage: "
                  << "ev3_control_server"
                  << " ADDRESS[:PORT]"
                  << std::endl;
        return 1;
    }

    precondition(l_motor.connected(), "Left motor not connected");
    precondition(r_motor.connected(), "Right motor not connected");

    // Set up the EzRpcServer, binding to port 5923 unless a
    // different port was specified by the user.  Note that the
    // first parameter here can be any "Client" object or anything
    // that can implicitly cast to a "Client" object.  You can even
    // re-export a capability imported from another server.
    capnp::EzRpcServer server(kj::heap<Ev3ControlServer>(), argv[1], 5923);

    auto &waitScope = server.getWaitScope();
    std::cout << "running on "
                  << argv[1]
                  << std::endl;
    //   // Run forever, accepting connections and handling requests.
    kj::NEVER_DONE.wait(waitScope);
}