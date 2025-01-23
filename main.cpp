#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

using namespace std::chrono;

bool is_on_ = false;
bool takeof_on_ = false;
std::vector<std::vector<int>> gates = {{0,0,1}, {0,0,2}, {0,0,3}, {0,0,4}};
int actual_gate = 0;


void arm(){
    std::cout << "moteres girando..." << std::endl;
    seconds timer_(5);
    auto inicio = steady_clock::now();

    while (duration_cast<seconds>(steady_clock::now() - inicio) < timer_)
    {
        std::cout << "girando a 15rpm" << std::endl;
        is_on_ = true;

        if(takeof_on_){
            std::cout << "takeof ativado" << std::endl;
            return;
        }
    }
    
    std::cout << "desarmando..." << std::endl;
    is_on_ = false;
    return;
}

void going_to(std::vector<int> gate){
    std::cout << "Gate: " << "'" << gate[0] << ", " << gate[1] << ", "<< gate[2] << "'"<< std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if(actual_gate == 3){
        std::this_thread::sleep_for(std::chrono::seconds(7));
    }
    is_on_ = true;
}

void take_of(){
    std::this_thread::sleep_for(std::chrono::seconds(3));
    takeof_on_ = true;
    if(is_on_){
        std::cout << "levantando o drone" << std::endl;
        while (true){
            std::thread t_going_to(going_to, gates[actual_gate]);
            t_going_to.detach();
            is_on_ = false;

            seconds timer_(5);
            auto inicio = steady_clock::now();

            while (duration_cast<seconds>(steady_clock::now() - inicio) < timer_){
                //std::cout << "esperando estado anterior" << std::endl;

                if(is_on_){
                    std::cout << "estado alcançado" << std::endl;
                    break;
                }
            }

            if(!is_on_){
                std::cout << "erro ao ir para gate: " << "'" << gates[actual_gate][0] << ", " << gates[actual_gate][1] << ", "<< gates[actual_gate][2] << "'"<< std::endl;
                break;
            }

            actual_gate++;
            if(actual_gate >= 4){
                std::cout << "todos os gates alcançados" << std::endl;
                break;
            }

            std::cout << std::endl;
        }

        std::cout << "voltando para o ponto inicial" << std::endl;
        return;
    }

    std::cout << "arm não inicializado" << std::endl;
}

int main(){
    std::thread t_arm(arm);
    std::thread t_take_of(take_of);

    t_take_of.join();
    t_arm.join();

    return 0;
}