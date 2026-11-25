#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILA 100
#define TAM_NOME 100

typedef enum {
    CAT_NENHUMA = 0,
    CAT_URGENTE = 1,
    CAT_PREFERENCIAL = 2,
    CAT_NORMAL = 3
} Categoria;

typedef struct {
    char codigo[10];
    char nome[TAM_NOME];
    char genero;
    int idade;
    int gestante;
    int lactante;
    int necessidadeEspecial;
    int obesidade;
    Categoria categoria;
} Paciente;

typedef struct {
    Paciente itens[MAX_FILA];
    int qtd;
    int proximoNumero;
    char prefixo;
} FilaCategoria;

typedef struct {
    FilaCategoria urgente;
    FilaCategoria preferencial;
    FilaCategoria normal;
    int emAtendimento;
    Paciente pacienteAtual;
} SistemaSenhas;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int lerInteiroIntervalo(const char *mensagem, int min, int max) {
    int valor;
    int lidos;
    do {
        printf("%s", mensagem);
        fflush(stdout);
        lidos = scanf("%d", &valor);
        limparBuffer();
        if (lidos != 1 || valor < min || valor > max) {
            printf("Entrada invalida! Digite um numero entre %d e %d.\n", min, max);
        }
    } while (lidos != 1 || valor < min || valor > max);
    return valor;
}

int lerSimNao(const char *mensagem) {
    char resp;
    while (1) {
        printf("%s (S/N): ", mensagem);
        fflush(stdout);
        if (scanf(" %c", &resp) == 1) {
            limparBuffer();
            resp = (char)toupper((unsigned char)resp);
            if (resp == 'S') return 1;
            if (resp == 'N') return 0;
        }
        printf("Resposta invalida! Digite S para Sim ou N para Nao.\n");
    }
}

char lerGenero(void) {
    char g;
    while (1) {
        printf("Genero do paciente (M - Masculino / F - Feminino / O - Outro): ");
        fflush(stdout);
        if (scanf(" %c", &g) == 1) {
            limparBuffer();
            g = (char)toupper((unsigned char)g);
            if (g == 'M' || g == 'F' || g == 'O') {
                return g;
            }
        }
        printf("Genero invalido! Informe M, F ou O.\n");
    }
}

void lerNomePaciente(char *destino, size_t tamanho) {
    while (1) {
        printf("Nome do paciente: ");
        fflush(stdout);
        if (fgets(destino, (int)tamanho, stdin) != NULL) {
            size_t len = strlen(destino);
            if (len > 0 && destino[len - 1] == '\n') {
                destino[len - 1] = '\0';
            } else {
                limparBuffer();
            }
            if (strlen(destino) > 0) {
                return;
            }
        }
        printf("Nome invalido! O campo nao pode ficar vazio.\n");
    }
}

void inicializarSistema(SistemaSenhas *sys) {
    sys->urgente.qtd = 0;
    sys->urgente.proximoNumero = 1;
    sys->urgente.prefixo = 'U';

    sys->preferencial.qtd = 0;
    sys->preferencial.proximoNumero = 1;
    sys->preferencial.prefixo = 'P';

    sys->normal.qtd = 0;
    sys->normal.proximoNumero = 1;
    sys->normal.prefixo = 'N';

    sys->emAtendimento = 0;
    memset(&sys->pacienteAtual, 0, sizeof(Paciente));
}

Categoria classificarPaciente(const Paciente *p) {
    if (p->idade >= 80) {
        return CAT_URGENTE;
    }
    if (p->gestante || p->lactante || p->necessidadeEspecial || p->obesidade) {
        return CAT_PREFERENCIAL;
    }
    if (p->genero == 'F' && p->idade >= 60) {
        return CAT_PREFERENCIAL;
    }
    if (p->genero == 'M' && p->idade >= 65) {
        return CAT_PREFERENCIAL;
    }
    if (p->idade >= 65) {
        return CAT_PREFERENCIAL;
    }
    return CAT_NORMAL;
}

const char *nomeCategoria(Categoria cat) {
    switch (cat) {
        case CAT_URGENTE: return "Urgente";
        case CAT_PREFERENCIAL: return "Preferencial";
        case CAT_NORMAL: return "Normal";
        default: return "Nenhuma";
    }
}

FilaCategoria *obterFilaPorCategoria(SistemaSenhas *sys, Categoria cat) {
    switch (cat) {
        case CAT_URGENTE: return &sys->urgente;
        case CAT_PREFERENCIAL: return &sys->preferencial;
        case CAT_NORMAL: return &sys->normal;
        default: return NULL;
    }
}

void gerarCodigoSenha(FilaCategoria *fila, char *codigoSaida, size_t tam) {
    snprintf(codigoSaida, tam, "%c%03d", fila->prefixo, fila->proximoNumero);
    fila->proximoNumero++;
}

int enfileirarPaciente(FilaCategoria *fila, Paciente p) {
    if (fila == NULL || fila->qtd >= MAX_FILA) {
        return 0;
    }
    gerarCodigoSenha(fila, p.codigo, sizeof(p.codigo));
    fila->itens[fila->qtd] = p;
    fila->qtd++;
    return 1;
}

int desenfileirarFrente(FilaCategoria *fila, Paciente *removido) {
    if (fila == NULL || fila->qtd <= 0) {
        return 0;
    }
    if (removido != NULL) {
        *removido = fila->itens[0];
    }
    for (int i = 0; i < fila->qtd - 1; i++) {
        fila->itens[i] = fila->itens[i + 1];
    }
    fila->qtd--;
    return 1;
}

void cadastrarPaciente(SistemaSenhas *sys) {
    Paciente p;
    memset(&p, 0, sizeof(Paciente));

    printf("\n--- REGISTRO DE PACIENTE E EMISSAO DE SENHA ---\n");
    lerNomePaciente(p.nome, sizeof(p.nome));
    p.genero = lerGenero();
    p.idade = lerInteiroIntervalo("Idade do paciente (0 a 130): ", 0, 130);

    if (p.genero == 'F') {
        p.gestante = lerSimNao("A paciente esta gestante? ");
    } else {
        p.gestante = 0;
    }
    p.lactante = lerSimNao("O paciente e lactante?");
    p.necessidadeEspecial = lerSimNao("Apresenta necessidade especial?");
    p.obesidade = lerSimNao("Possui condicao de obesidade?");

    p.categoria = classificarPaciente(&p);
    FilaCategoria *fila = obterFilaPorCategoria(sys, p.categoria);

    if (!enfileirarPaciente(fila, p)) {
        printf("Erro: fila da categoria %s esta cheia!\n", nomeCategoria(p.categoria));
        return;
    }

    Paciente *inserido = &fila->itens[fila->qtd - 1];
    printf("\nSenha emitida com sucesso!\n");
    printf("Codigo: %s | Categoria: %s | Paciente: %s\n",
           inserido->codigo, nomeCategoria(inserido->categoria), inserido->nome);
}

int obterProximaNaFila(const SistemaSenhas *sys, Paciente *proxima) {
    if (sys->urgente.qtd > 0) {
        *proxima = sys->urgente.itens[0];
        return 1;
    }
    if (sys->preferencial.qtd > 0) {
        *proxima = sys->preferencial.itens[0];
        return 1;
    }
    if (sys->normal.qtd > 0) {
        *proxima = sys->normal.itens[0];
        return 1;
    }
    return 0;
}

void consultarProximaSenha(const SistemaSenhas *sys) {
    Paciente prox;
    printf("\n--- CONSULTA DA PROXIMA SENHA ---\n");
    if (obterProximaNaFila(sys, &prox)) {
        printf("Proxima senha: %s\n", prox.codigo);
        printf("Categoria: %s\n", nomeCategoria(prox.categoria));
        printf("Paciente: %s\n", prox.nome);
    } else {
        printf("Nao ha pacientes aguardando na fila.\n");
    }
}

void registrarChamada(SistemaSenhas *sys) {
    printf("\n--- REGISTRO DE CHAMADA ---\n");
    if (sys->emAtendimento) {
        printf("Ja existe um paciente em atendimento: %s (%s - %s).\n",
               sys->pacienteAtual.codigo,
               sys->pacienteAtual.nome,
               nomeCategoria(sys->pacienteAtual.categoria));
        printf("Finalize o atendimento atual antes de chamar a proxima senha.\n");
        return;
    }

    int chamou = 0;
    if (sys->urgente.qtd > 0) {
        chamou = desenfileirarFrente(&sys->urgente, &sys->pacienteAtual);
    } else if (sys->preferencial.qtd > 0) {
        chamou = desenfileirarFrente(&sys->preferencial, &sys->pacienteAtual);
    } else if (sys->normal.qtd > 0) {
        chamou = desenfileirarFrente(&sys->normal, &sys->pacienteAtual);
    }

    if (chamou) {
        sys->emAtendimento = 1;
        printf("Senha chamada e em atendimento: %s\n", sys->pacienteAtual.codigo);
        printf("Paciente: %s | Categoria: %s\n",
               sys->pacienteAtual.nome, nomeCategoria(sys->pacienteAtual.categoria));
    } else {
        printf("Nao existem senhas aguardando chamada.\n");
    }
}

void identificarSenhaEmAtendimento(const SistemaSenhas *sys) {
    printf("\n--- SENHA EM ATENDIMENTO ---\n");
    if (sys->emAtendimento) {
        printf("Senha atual: %s\n", sys->pacienteAtual.codigo);
        printf("Categoria: %s\n", nomeCategoria(sys->pacienteAtual.categoria));
        printf("Paciente: %s (Idade: %d)\n",
               sys->pacienteAtual.nome, sys->pacienteAtual.idade);
    } else {
        printf("Nenhum paciente esta em atendimento no momento.\n");
    }
}

void finalizarAtendimento(SistemaSenhas *sys) {
    printf("\n--- FINALIZACAO DE ATENDIMENTO ---\n");
    if (!sys->emAtendimento) {
        printf("Nao ha atendimento ativo para finalizar.\n");
        return;
    }

    printf("Atendimento finalizado para a senha %s (%s - %s).\n",
           sys->pacienteAtual.codigo,
           sys->pacienteAtual.nome,
           nomeCategoria(sys->pacienteAtual.categoria));
    printf("Senha registrada como atendida e removida definitivamente do fluxo.\n");

    sys->emAtendimento = 0;
    memset(&sys->pacienteAtual, 0, sizeof(Paciente));
}

void exibirPainelChamada(const SistemaSenhas *sys) {
    printf("\n======================================================\n");
    printf("                 PAINEL DE CHAMADA CLINICA            \n");
    printf("======================================================\n");

    if (sys->emAtendimento) {
        printf("Senha atualmente chamada : %s (%s)\n",
               sys->pacienteAtual.codigo, sys->pacienteAtual.nome);
        printf("Categoria do atendimento : %s\n",
               nomeCategoria(sys->pacienteAtual.categoria));
    } else {
        printf("Senha atualmente chamada : Nenhuma\n");
        printf("Categoria do atendimento : -\n");
    }

    printf("------------------------------------------------------\n");
    printf("Proximas senhas aguardando chamada:\n");

    int exibidas = 0;
    for (int i = 0; i < sys->urgente.qtd && exibidas < 5; i++) {
        printf("  [%d] %s - %s (Urgente)\n",
               exibidas + 1, sys->urgente.itens[i].codigo, sys->urgente.itens[i].nome);
        exibidas++;
    }
    for (int i = 0; i < sys->preferencial.qtd && exibidas < 5; i++) {
        printf("  [%d] %s - %s (Preferencial)\n",
               exibidas + 1, sys->preferencial.itens[i].codigo, sys->preferencial.itens[i].nome);
        exibidas++;
    }
    for (int i = 0; i < sys->normal.qtd && exibidas < 5; i++) {
        printf("  [%d] %s - %s (Normal)\n",
               exibidas + 1, sys->normal.itens[i].codigo, sys->normal.itens[i].nome);
        exibidas++;
    }
    if (exibidas == 0) {
        printf("  Nenhuma senha na fila de espera.\n");
    }

    printf("------------------------------------------------------\n");
    printf("Quantidade em espera por categoria:\n");
    printf("  Urgente      : %d\n", sys->urgente.qtd);
    printf("  Preferencial : %d\n", sys->preferencial.qtd);
    printf("  Normal       : %d\n", sys->normal.qtd);
    printf("  Total espera : %d\n",
           sys->urgente.qtd + sys->preferencial.qtd + sys->normal.qtd);
    printf("======================================================\n");
}

void exibirMenu(void) {
    printf("\n===== SISTEMA DE GERENCIAMENTO DE SENHAS CLINICAS =====\n");
    printf("1 - Cadastrar paciente\n");
    printf("2 - Consultar proxima senha\n");
    printf("3 - Registrar chamada do paciente\n");
    printf("4 - Identificar senha em atendimento\n");
    printf("5 - Finalizar atendimento e atualizar fila\n");
    printf("6 - Exibir painel de chamada\n");
    printf("0 - Sair\n");
}

int main(void) {
    SistemaSenhas sistema;
    int opcao;

    inicializarSistema(&sistema);

    do {
        exibirMenu();
        opcao = lerInteiroIntervalo("Escolha uma opcao: ", 0, 6);

        switch (opcao) {
            case 1:
                cadastrarPaciente(&sistema);
                break;
            case 2:
                consultarProximaSenha(&sistema);
                break;
            case 3:
                registrarChamada(&sistema);
                break;
            case 4:
                identificarSenhaEmAtendimento(&sistema);
                break;
            case 5:
                finalizarAtendimento(&sistema);
                break;
            case 6:
                exibirPainelChamada(&sistema);
                break;
            case 0:
                printf("\nEncerrando o sistema de senhas...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}