#include <framework.hpp>
#include <r69-driver/r69_driver.hpp>

int main() {
    std::printf("(+) main\n");

    r69->attach(L"notepad.exe");
    
    auto* data = r69->get_attached_process();

    std::printf("(+) process header -> %x\n", r69->read<uint16_t>((uint64_t)data->base_address));

    r69->write<uint16_t>((uint64_t)data->base_address, 0xDEADBEEF);

    std::printf("(+) process header -> %x\n", r69->read<uint16_t>((uint64_t)data->base_address));

    r69->detach();
    
    std::cin.get();

    return 0;
}