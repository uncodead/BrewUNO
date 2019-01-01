#include <BrewService.h>

BrewService::BrewService(AsyncWebServer *server, FS *fs) {

}

void BrewService::loop() {
    // Obter estado atual
    // se nao iniciou a brassagem, sai do programa
    //  
    // espera o sampletime
    
    // obtem o timer atual
    // se active_step for mash
    //   se o end_time não e null e o time atual é maior ou igual ao end_time(acabou o passo)
    //   sim:
    //     verifica o proximo step
    //     se existir:
    //       seta o active_step_index, start_time = vazio, end_time = vazio, targt_temperature
    //     se nao existir:
    //       seta para boil, active_steo_index = 0, start_time=null, end=null, target_temperature = temperatura configurada
    //    nao:
    //      verifica se a temperatura esta entre a target_temperature - 0.2 e target_temperature + 0.2 e se o start_time é vazio
    //        sim
    //          seta start_time, end_time



    // se for boil
    //   verifica se a temperatura esta maior ou igual a target_temperature e start_time = vazio
    //     sim:
    //       seta start_time, end_time
    //     
    //   se end_time nao e null e time atual é maior ou igual ao end_time (acabou a fervura)
    //     sim:
    //       seta start_time = null, end_time = null, target_temperature = 0, brassagem iniciada = false, step=mash
    //            
}