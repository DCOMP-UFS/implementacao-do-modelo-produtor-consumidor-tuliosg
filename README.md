[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=16175234&assignment_repo_type=AssignmentRepo)
# Etapa 2 - Projeto PPC
Implementação do Modelo Produtor-Consumidor

## Equipe responsável:
 * Nayla Sahra Santos das Chagas - 202000024525  
 * Túlio Sousa de Gois - 202000024599

---

## Solução
A nossa solução partiu da implementação da estrutura do Relógio Vetorial, de um buffer compartilhado, das variávies de condição e das funções para produtor e consumidor.

* **Relógio Vetorial**   
    O relógio é um vetor de inteiros que tem o tamanho igual ao número de threads produtoras;
* **Buffer**  
    O buffer é compartilhado entre produtores e consumidores, e possui tamanho fixo para poder guardar os relógios;
* **Sincronização**    
    A sincronização foi implementada usando um mutex e variáveis de condição para acessar o buffer, evitando as condições de corrida;
  
### Cenários de teste
Os cenários de teste estão diretamente nas funções de produtor e consumidor, o cenário atual é o de fila cheia. Para trocar o cenário, basta comentar o trecho de código de um, e remover o comentário do outro.
