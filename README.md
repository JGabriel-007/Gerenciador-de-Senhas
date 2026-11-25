# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Aplicação em linguagem **C** desenvolvida para triagem, emissão de senhas sequenciais por prioridade, controle de filas em ordem FIFO e exibição de painel de chamada para atendimento clínico.

## Objetivo

Organizar o fluxo de atendimento de pacientes por meio de regras de prioridade definidas para a atividade, garantindo rastreabilidade das chamadas, atualização das filas sem retorno de senhas já atendidas e separação modular das responsabilidades do sistema.

## Funcionalidades

- **Registro e emissão de senha:** coleta nome, gênero, idade e condições de prioridade (gestante quando aplicável, lactante, necessidade especial e obesidade).
- **Classificação automática e numeração independente:**
  - **Urgente (`U001`, `U002`...):** idade superior a 80 anos.
  - **Preferencial (`P001`, `P002`...):** gestantes, lactantes, pessoas com necessidades especiais, pessoas obesas, mulheres com 60 anos ou mais e homens com 65 anos ou mais.
  - **Normal (`N001`, `N002`...):** demais pacientes sem condições de prioridade.
- **Controle de atendimento e precedência (`Urgente > Preferencial > Normal`):** consulta da próxima senha, registro de chamada, identificação de quem está em atendimento, finalização e atualização da fila respeitando FIFO.
- **Painel de chamada:** exibe senha atualmente chamada, categoria do atendimento, próximas senhas aguardando chamada e quantidade de pacientes em espera por categoria.

## Compilação e Execução

Com o compilador `gcc`:

```bash
gcc -Wall -Wextra src/gerenciador_senhas.c -o gerenciador_senhas
./gerenciador_senhas
```

## Principais Funções Implementadas

- `inicializarSistema`: inicializa as estruturas das filas (`U`, `P`, `N`) e o estado de atendimento.
- `classificarPaciente`: determina a categoria de prioridade do paciente conforme idade, gênero e condições informadas.
- `gerarCodigoSenha` / `enfileirarPaciente` / `desenfileirarFrente`: emitem códigos sequenciais independentes e manipulam as filas em ordem FIFO.
- `cadastrarPaciente`: valida entradas e registra o paciente na fila correspondente.
- `consultarProximaSenha`, `registrarChamada`, `identificarSenhaEmAtendimento` e `finalizarAtendimento`: controlam o ciclo de chamada e atualização da fila.
- `exibirPainelChamada`: apresenta o resumo visual da chamada atual, próximas senhas e quantidades em espera.

## Exemplo de Utilização

1. Cadastrar um paciente masculino de **82 anos** -> recebe senha **`U001`** (Urgente).
2. Cadastrar uma paciente feminina de **63 anos** -> recebe senha **`P001`** (Preferencial).
3. Cadastrar um paciente masculino de **30 anos** sem condições prioritárias -> recebe senha **`N001`** (Normal).
4. Ao registrar a chamada, o sistema chama primeiro **`U001`**, depois **`P001`** e por último **`N001`**, atualizando a fila e o painel de chamada após finalizar cada atendimento.