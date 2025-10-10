// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

#define MAX_TERRITORIOS 5
#define TAM_STRING 50

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

struct Territorio
{
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

void limparBufferEntrada();
void mapaMundo(struct Territorio *t, int qtd);
void cadastrar(struct Territorio *t, int quantidade);
void atacar(struct Territorio *atk, struct Territorio *def);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, struct Territorio *mapa, int tamanho);
void liberarMemoria(struct Territorio *mapa, char *missao1, char *missao2);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.
    srand(time(NULL));

    int quantidade;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &quantidade);

    struct Territorio *mapa = (struct Territorio *)calloc(quantidade, sizeof(struct Territorio));
    if (mapa == NULL)
    {
        printf("Falha ao alocar memoria.\n");
        return 1;
    }

    cadastrar(mapa, quantidade);

    char *missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas vermelhas",
        "Dominar 2 territorios com mais de 10 tropas",
        "Ter o dobro de tropas do inimigo",
        "Vencer 3 batalhas consecutivas"};
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    char *missaoJogador1 = (char *)malloc(100 * sizeof(char));
    char *missaoJogador2 = (char *)malloc(100 * sizeof(char));

    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\nMissao do Jogador 1: %s\n", missaoJogador1);
    printf("Missao do Jogador 2: %s\n", missaoJogador2);

    int opcao;

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    do
    {
        mapaMundo(mapa, quantidade);
        printf("\n1 - Atacar\n2 - Verificar Missoes\n0 - Sair\n");
        printf("Escolha a opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            int a, d;
            printf("Escolha o numero do atacante: ");
            scanf("%d", &a);
            printf("Escolha o numero do defensor: ");
            scanf("%d", &d);

            if (a < 1 || a > quantidade || d < 1 || d > quantidade)
            {
                printf(" Territorio invalido!\n");
            }
            else
            {
                atacar(&mapa[a - 1], &mapa[d - 1]);
            }
        }
        else if (opcao == 2)
        {
            if (verificarMissao(missaoJogador1, mapa, quantidade))
            {
                printf("\nJogador 1 cumpriu sua missao e venceu o jogo!\n");
                break;
            }
            if (verificarMissao(missaoJogador2, mapa, quantidade))
            {
                printf("\nJogador 2 cumpriu sua missao e venceu o jogo!\n");
                break;
            }
            printf("Nenhum jogador cumpriu a missao ainda.\n");
        }
    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    printf("\nPrograma encerrado, memoria liberada!\n");
    getchar();

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void cadastrar(struct Territorio *t, int quantidade)
{
    for (int i = 0; i < quantidade; i++)
    {
        printf("\n--- Cadastro do territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %29[^\n]", &t[i].nome);
        printf("Cor do exercito: ");
        scanf(" %9s", &t[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &t[i].tropas);
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

void liberarMemoria(struct Territorio *mapa, char *missao1, char *missao2)
{
    free(mapa);
    free(missao1);
    free(missao2);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void mapaMundo(struct Territorio *t, int qtd)
{
    printf("============================================\n");
    printf("       MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("============================================\n");

    for (int i = 0; i < qtd; i++)
    {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, t[i].nome, t[i].cor, t[i].tropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
int verificarMissao(char *missao, struct Territorio *mapa, int tamanho)
{
    int count = 0;
    for (int i = 0; i < tamanho; i++)
    {
        if (strcmp(mapa[i].cor, "azul") == 0)
            count++;
    }
    if (strstr(missao, "Conquistar 3 territorios") != NULL && count >= 3)
        return 1;
    return 0;
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void atacar(struct Territorio *atk, struct Territorio *def)
{
    if (strcmp(atk->cor, def->cor) == 0)
    {
        printf("\n Nao pode atacar um territorio da mesma cor!\n");
        return;
    }
    if (atk->tropas < 2)
    {
        printf("\n Precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }

    int dadoAtk = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;
    printf("\n---Rolagem de dados---");
    printf("\nDados: Atacante %d x Defensor %d\n", dadoAtk, dadoDef);

    if (dadoAtk > dadoDef)
    {
        printf(">> Atacante venceu e conquistou o territorio!\n");
        def->tropas = atk->tropas / 2;
        strcpy(def->cor, atk->cor);
    }
    else
    {
        printf(">> Defensor resistiu ao ataque!\n");
        atk->tropas--;
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
void atribuirMissao(char *destino, char *missoes[], int totalMissoes)
{
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
