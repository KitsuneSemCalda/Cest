// Repare que isso é A ÚNICA VEZ que cest_result() será chamado na branch dessa suíte de teste.
// Outros arquivos como test_math.c podem declarar seus testes tranquilamente e compartilhar
// cest_stats_t com esse main global por conta do _CEST_WEAK gerado no cest.h.

#include "../../cest.h"

int main() {
    printf("--- Multi-file Test Runner Started ---\n");
    // Todos os testes que forem linkados neste binário rodarão automaticamente durante a construção 
    // global (se tiverem atributos constructor no GCC), mas em C padrão os métodos teste 
    // costumam ser invocados de um caller central, ou auto-executados caso se usem atributos extras.
    
    // No entanto o Cest requer que invoquemos blocos explicitamente se não usarmos macros estendidas:
    
    // Declarações dos métodos runner de outros arquivos
    void run_math_tests();
    void run_string_tests();

    // Rodando as suítes
    run_math_tests();
    run_string_tests();

    printf("\n--- Test Suite Ended ---\n");
    return cest_result(); // O cest_result() tem a informação GLOBAL de todo Cest compilado.
}
