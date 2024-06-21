#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    int codigo;
    char nome[100];
    char endereco[200];
    char telefone[20];
} Cliente;

typedef struct {
    int codigo;
    char nome[100];
    char telefone[20];
    char cargo[50];
    float salario;
} Funcionario;

typedef struct {
    int codigo_estadia;
    struct tm data_entrada;
    struct tm data_saida;
    int quantidade_diarias;
    int codigo_cliente;
    int numero_quarto;
} Estadia;

typedef struct {
    int numero_quarto;
    int quantidade_hospedes;
    float valor_diaria;
    char status[20]; // OCUPADO - DESOCUPADO
} Quarto;

void cadastrar_cliente(Cliente clientes[], int *num_clientes) {
    Cliente novo_cliente;
    printf("\nDigite o código do cliente: ");
    scanf("%d", &novo_cliente.codigo);

    int i = 0;
    while (i < *num_clientes) {
        if (clientes[i].codigo == novo_cliente.codigo) {
            printf("Erro: Já existe um cliente com esse código.\n");
            return;
        }
        i++;
    }

    printf("Digite o nome do cliente: ");
    scanf(" %[^\n]s", novo_cliente.nome);
    printf("Digite o endereço do cliente: ");
    scanf(" %[^\n]s", novo_cliente.endereco);
    printf("Digite o telefone do cliente: ");
    scanf(" %[^\n]s", novo_cliente.telefone);

    clientes[*num_clientes] = novo_cliente;
    (*num_clientes)++;
    printf("Cliente cadastrado com sucesso.\n");
}

void cadastrar_funcionario(Funcionario funcionarios[], int *num_funcionarios) {
    Funcionario novo_funcionario;
    printf("\nDigite o código do funcionário: ");
    scanf("%d", &novo_funcionario.codigo);

    int i = 0;
    while (i < *num_funcionarios) {
        if (funcionarios[i].codigo == novo_funcionario.codigo) {
            printf("Erro: Já existe um funcionário com esse código.\n");
            return;
        }
        i++;
    }

    printf("Digite o nome do funcionário: ");
    scanf(" %[^\n]s", novo_funcionario.nome);
    printf("Digite o telefone do funcionário: ");
    scanf(" %[^\n]s", novo_funcionario.telefone);
    printf("Digite o cargo do funcionário: ");
    scanf(" %[^\n]s", novo_funcionario.cargo);
    printf("Digite o salário do funcionário: ");
    scanf("%f", &novo_funcionario.salario);

    funcionarios[*num_funcionarios] = novo_funcionario;
    (*num_funcionarios)++;
    printf("Funcionário cadastrado com sucesso.\n");
}

void cadastrar_estadia(Estadia estadias[], int *num_estadias,
                       Cliente clientes[], int num_clientes, Quarto quartos[],
                       int num_quartos) {
    Estadia nova_estadia;
    int codigo_cliente, quantidade_hospedes;
    printf("\nDigite o código do cliente: ");
    scanf("%d", &codigo_cliente);

    int cliente_encontrado = 0;
    int i = 0;
    while (i < num_clientes) {
        if (clientes[i].codigo == codigo_cliente) {
            cliente_encontrado = 1;
            break;
        }
        i++;
    }
    if (!cliente_encontrado) {
        printf("Erro: Cliente não cadastrado.\n");
        return;
    }

    printf("Digite a quantidade de hóspedes: ");
    scanf("%d", &quantidade_hospedes);

    int quarto_encontrado = 0;
    i = 0;
    while (i < num_quartos) {
        if (quartos[i].quantidade_hospedes >= quantidade_hospedes &&
            strcmp(quartos[i].status, "desocupado") == 0) {
            nova_estadia.numero_quarto = quartos[i].numero_quarto;
            strcpy(quartos[i].status, "ocupado");
            quarto_encontrado = 1;
            break;
        }
        i++;
    }
    if (!quarto_encontrado) {
        printf("Erro: Não há quartos disponíveis para a quantidade de hóspedes "
               "desejada.\n");
        return;
    }

    printf("Digite a data de entrada (dd mm yyyy hh mm): ");
    scanf("%d %d %d %d %d", &nova_estadia.data_entrada.tm_mday,
          &nova_estadia.data_entrada.tm_mon, &nova_estadia.data_entrada.tm_year,
          &nova_estadia.data_entrada.tm_hour, &nova_estadia.data_entrada.tm_min);
    nova_estadia.data_entrada.tm_mon -= 1;
    nova_estadia.data_entrada.tm_year -= 1900;

    printf("Digite a data de saída (dd mm yyyy hh mm): ");
    scanf("%d %d %d %d %d", &nova_estadia.data_saida.tm_mday,
          &nova_estadia.data_saida.tm_mon, &nova_estadia.data_saida.tm_year,
          &nova_estadia.data_saida.tm_hour, &nova_estadia.data_saida.tm_min);
    nova_estadia.data_saida.tm_mon -= 1;
    nova_estadia.data_saida.tm_year -= 1900;

    time_t entrada = mktime(&nova_estadia.data_entrada);
    time_t saida = mktime(&nova_estadia.data_saida);
    double segundos = difftime(saida, entrada);
    nova_estadia.quantidade_diarias = (segundos / (60 * 60 * 24)) + 1;

    nova_estadia.codigo_cliente = codigo_cliente;
    nova_estadia.codigo_estadia = *num_estadias + 1;
    estadias[*num_estadias] = nova_estadia;
    (*num_estadias)++;
    printf("Estadia cadastrada com sucesso.\n");
}

void dar_baixa_estadia(Estadia estadias[], int *num_estadias, Quarto quartos[],
                       int num_quartos) {
    int codigo_estadia;
    printf("\nDigite o código da estadia: ");
    scanf("%d", &codigo_estadia);

    int i = 0;
    while (i < *num_estadias) {
        if (estadias[i].codigo_estadia == codigo_estadia) {
            int numero_quarto = estadias[i].numero_quarto;
            int quantidade_diarias = estadias[i].quantidade_diarias;
            float valor_diaria = 0;

            int j = 0;
            while (j < num_quartos) {
                if (quartos[j].numero_quarto == numero_quarto) {
                    valor_diaria = quartos[j].valor_diaria;
                    strcpy(quartos[j].status, "desocupado");
                    break;
                }
                j++;
            }

            float valor_total = quantidade_diarias * valor_diaria;
            printf("Valor total a ser pago: R$%.2f\n", valor_total);

            int k = i;
            while (k < *num_estadias - 1) {
                estadias[k] = estadias[k + 1];
                k++;
            }
            (*num_estadias)--;
            printf("Estadia finalizada com sucesso.\n");
            return;
        }
        i++;
    }
    printf("Erro: Estadia não encontrada.\n");
}

void pesquisar_cliente(Cliente clientes[], int num_clientes) {
    int codigo;
    char nome[100];
    int opcao;

    printf("\nPesquisar por: \n1. Código \n2. Nome \n\nEscolha: ");
    scanf("%d", &opcao);

    int i = 0;
    if (opcao == 1) {
        printf("Digite o código do cliente: ");
        scanf("%d", &codigo);
        while (i < num_clientes) {
            if (clientes[i].codigo == codigo) {
                printf("Cliente encontrado:\n");
                printf("Código: %d\nNome: %s\nEndereço: %s\nTelefone: %s\n",
                       clientes[i].codigo, clientes[i].nome, clientes[i].endereco,
                       clientes[i].telefone);
                return;
            }
            i++;
        }
    } else if (opcao == 2) {
        printf("Digite o nome do cliente: ");
        scanf(" %[^\n]s", nome);
        while (i < num_clientes) {
            if (strcmp(clientes[i].nome, nome) == 0) {
                printf("Cliente encontrado:\n");
                printf("Código: %d\nNome: %s\nEndereço: %s\nTelefone: %s\n",
                       clientes[i].codigo, clientes[i].nome, clientes[i].endereco,
                       clientes[i].telefone);
                return;
            }
            i++;
        }
    }
    printf("Cliente não encontrado.\n");
}

void pesquisar_funcionario(Funcionario funcionarios[], int num_funcionarios) {
    int codigo;
    char nome[100];
    int opcao;

    printf("\nPesquisar por: \n1. Código \n2. Nome \n\nEscolha: ");
    scanf("%d", &opcao);

    int i = 0;
    if (opcao == 1) {
        printf("Digite o código do funcionário: ");
        scanf("%d", &codigo);
        while (i < num_funcionarios) {
            if (funcionarios[i].codigo == codigo) {
                printf("Funcionário encontrado:\n");
                printf("Código: %d\nNome: %s\nTelefone: %s\nCargo: %s\nSalário: %.2f\n",
                       funcionarios[i].codigo, funcionarios[i].nome,
                       funcionarios[i].telefone, funcionarios[i].cargo,
                       funcionarios[i].salario);
                return;
            }
            i++;
        }
    } else if (opcao == 2) {
        printf("Digite o nome do funcionário: ");
        scanf(" %[^\n]s", nome);
        while (i < num_funcionarios) {
            if (strcmp(funcionarios[i].nome, nome) == 0) {
                printf("Funcionário encontrado:\n");
                printf("Código: %d\nNome: %s\nTelefone: %s\nCargo: %s\nSalário: %.2f\n",
                       funcionarios[i].codigo, funcionarios[i].nome,
                       funcionarios[i].telefone, funcionarios[i].cargo,
                       funcionarios[i].salario);
                return;
            }
            i++;
        }
    }
    printf("Funcionário não encontrado.\n");
}

void listar_estadias_cliente(Estadia estadias[], int num_estadias,
                             int codigo_cliente) {
    printf("\nEstadias do cliente %d:\n", codigo_cliente);

    int i = 0;
    while (i < num_estadias) {
        if (estadias[i].codigo_cliente == codigo_cliente) {
            printf(
                    "\nCódigo da Estadia: %d\nQuarto: %d\nData de Entrada: "
                    "%02d/%02d/%04d %02d:%02d\nData de Saída: %02d/%02d/%04d "
                    "%02d:%02d\nQuantidade de Diárias: %d\n",
                    estadias[i].codigo_estadia, estadias[i].numero_quarto,
                    estadias[i].data_entrada.tm_mday, estadias[i].data_entrada.tm_mon + 1,
                    estadias[i].data_entrada.tm_year + 1900,
                    estadias[i].data_entrada.tm_hour, estadias[i].data_entrada.tm_min,
                    estadias[i].data_saida.tm_mday, estadias[i].data_saida.tm_mon + 1,
                    estadias[i].data_saida.tm_year + 1900, estadias[i].data_saida.tm_hour,
                    estadias[i].data_saida.tm_min, estadias[i].quantidade_diarias);
        }
        i++;
    }
}

void salvar_clientes(Cliente clientes[], int num_clientes) {
    FILE *file = fopen("clientes.dat", "wb");
    fwrite(&num_clientes, sizeof(int), 1, file);
    fwrite(clientes, sizeof(Cliente), num_clientes, file);
    fclose(file);
}

void carregar_clientes(Cliente clientes[], int *num_clientes) {
    FILE *file = fopen("clientes.dat", "rb");
    if (file) {
        fread(num_clientes, sizeof(int), 1, file);
        fread(clientes, sizeof(Cliente), *num_clientes, file);
        fclose(file);
    } else {
        *num_clientes = 0;
    }
}

void salvar_funcionarios(Funcionario funcionarios[], int num_funcionarios) {
    FILE *file = fopen("funcionarios.dat", "wb");
    fwrite(&num_funcionarios, sizeof(int), 1, file);
    fwrite(funcionarios, sizeof(Funcionario), num_funcionarios, file);
    fclose(file);
}

void carregar_funcionarios(Funcionario funcionarios[], int *num_funcionarios) {
    FILE *file = fopen("funcionarios.dat", "rb");
    if (file) {
        fread(num_funcionarios, sizeof(int), 1, file);
        fread(funcionarios, sizeof(Funcionario), *num_funcionarios, file);
        fclose(file);
    } else {
        *num_funcionarios = 0;
    }
}

void salvar_estadias(Estadia estadias[], int num_estadias) {
    FILE *file = fopen("estadias.dat", "wb");
    fwrite(&num_estadias, sizeof(int), 1, file);
    fwrite(estadias, sizeof(Estadia), num_estadias, file);
    fclose(file);
}

void carregar_estadias(Estadia estadias[], int *num_estadias) {
    FILE *file = fopen("estadias.dat", "rb");
    if (file) {
        fread(num_estadias, sizeof(int), 1, file);
        fread(estadias, sizeof(Estadia), *num_estadias, file);
        fclose(file);
    } else {
        *num_estadias = 0;
    }
}

void salvar_quartos(Quarto quartos[], int num_quartos) {
    FILE *file = fopen("quartos.dat", "wb");
    fwrite(&num_quartos, sizeof(int), 1, file);
    fwrite(quartos, sizeof(Quarto), num_quartos, file);
    fclose(file);
}

void carregar_quartos(Quarto quartos[], int *num_quartos) {
    FILE *file = fopen("quartos.dat", "rb");
    if (file) {
        fread(num_quartos, sizeof(int), 1, file);
        fread(quartos, sizeof(Quarto), *num_quartos, file);
        fclose(file);
    } else {
        *num_quartos = 0;
    }
}

void main_menu() {
    Cliente clientes[100];
    Funcionario funcionarios[100];
    Estadia estadias[100];
    Quarto quartos[100];
    int num_clientes = 0, num_funcionarios = 0, num_estadias = 0, num_quartos = 0;

    carregar_clientes(clientes, &num_clientes);
    carregar_funcionarios(funcionarios, &num_funcionarios);
    carregar_estadias(estadias, &num_estadias);
    carregar_quartos(quartos, &num_quartos);

    // QUARTOS GERADOS
    if (num_quartos == 0) {
        quartos[0] = (Quarto){101, 2, 150.0, "desocupado"};
        quartos[1] = (Quarto){102, 3, 200.0, "desocupado"};
        quartos[2] = (Quarto){103, 4, 250.0, "desocupado"};
        num_quartos = 3;
    }

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Cadastrar Funcionário\n");
        printf("3. Cadastrar Estadia\n");
        printf("4. Dar Baixa em Estadia\n");
        printf("5. Pesquisar Cliente\n");
        printf("6. Pesquisar Funcionário\n");
        printf("7. Listar Estadias de um Cliente\n");
        printf("8. Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_cliente(clientes, &num_clientes);
                salvar_clientes(clientes, num_clientes);
                break;
            case 2:
                cadastrar_funcionario(funcionarios, &num_funcionarios);
                salvar_funcionarios(funcionarios, num_funcionarios);
                break;
            case 3:
                cadastrar_estadia(estadias, &num_estadias, clientes, num_clientes,
                                  quartos, num_quartos);
                salvar_estadias(estadias, num_estadias);
                salvar_quartos(quartos, num_quartos);
                break;
            case 4:
                dar_baixa_estadia(estadias, &num_estadias, quartos, num_quartos);
                salvar_estadias(estadias, num_estadias);
                salvar_quartos(quartos, num_quartos);
                break;
            case 5:
                pesquisar_cliente(clientes, num_clientes);
                break;
            case 6:
                pesquisar_funcionario(funcionarios, num_funcionarios);
                break;
            case 7: {
                int codigo_cliente;
                printf("\nDigite o código do cliente: ");
                scanf("%d", &codigo_cliente);
                listar_estadias_cliente(estadias, num_estadias, codigo_cliente);
                break;
            }
            case 8:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
                break;
        }
    } while (opcao != 8);
}

int main() {
    main_menu();
    return 0;
}

